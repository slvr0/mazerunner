import zmq
import time
import sys

from enum import Enum

import threading
from memory import Memory
from nn import NeuralNetHandler
import torch as T
import numpy as np

#this is garbage ,rework, only one channel for rec and one for send!
#then in future, we can have separate channel for simple state evaluation request! first make main loop work

topic_receive = "env_send"
topic_send = "nn_send"

#we receive {state:value} or {state:value, reward:action} based on if its a traversal request or pure evaluation request
def decode_response(msg) :
    if type(msg) == bytes :
        msg = msg.decode("utf-8")

    ex = msg[1:-1] #remove brackets
    response_keypairs = {}

    each_kv_p = ex.split(',')
    for kvp in each_kv_p :
        kv_split = kvp.split(':')
        response_keypairs[kv_split[0]] = kv_split[1]

    return response_keypairs

#we send {state:value,action:value} message
def decode_received_actionresult(msg):
    state = 1
    reward = 1
    done = False
    info = {}
    return state,reward,done,info

class CommResponse(Enum):
    Invalid = 0
    Request = 1
    Request_Noresp = 2
    Request_ActionResult = 3
    Train = 4

#bind request topicfilters on global config later.
class ZmqCommInterface(threading.Thread) :
    def __init__(self, adress, session_params = None, nn = None, reporter = None):
        super(ZmqCommInterface, self).__init__()
        self.context = zmq.Context()

        self.receiver = self.context.socket(zmq.REP)
        self.receiver.bind("tcp://127.0.0.1:{}".format(adress))

        #self.transmitter = self.context.socket(zmq.PUSH)

        #self.transmitter.connect("tcp://127.0.0.1:{}".format(push_port))

        print("[x] python zmq communication interface started")
        #print("interface receiving message from port : {} and sending on port {}".format(pull_port, push_port) )
        print("reply interface receiving message from port : {}".format(adress))

        self.nn = nn
        self.session_params = session_params
        self.reporter = reporter

    def run(self) -> None:
        if self.session_params :
            training_sessions = self.session_params['n_training_sessions']
        else :
            training_sessions = 4000

        iter = 0
        for i in range(training_sessions) :
            hx = T.zeros(1, 256)

            state = None
            action = None
            done = False
            step = -1

            while True :
                message = self.receiver.recv()
                str = decode_response(message) #decodes the actual values in message

                new_state = [int(str['state'])]
                reward = int(str['reward'])
                do_train = int(str['train'])

                step += 1

                if step == 0: #initiate process
                    state = new_state
                    # convert to tensor
                    state_tensor = T.tensor(np.array([state]), dtype=T.float)
                    action, value, log_prob, hx = self.nn.request_statemetrics(state_tensor, hx)
                    self.que_c_statechange_response(state, action)

                else :
                    # convert to tensor

                    state_tensor = T.tensor(np.array([new_state]), dtype=T.float)
                    new_action, value, log_prob, hx = self.nn.request_statemetrics(state_tensor, hx)

                    # fix sending done messages. store data
                    self.nn.memory.remember(state, action, reward, new_state, value, log_prob)

                    state = new_state
                    action = new_action  # save, for next iteration of batch memory
                    # send response
                    done = True if reward == 1 else False  # this should be set by a key element in message from c...

                    if do_train == 1:
                        print("session ended! training net and then resetting...")
                        iter += 1
                        state_tensor = T.tensor(np.array([state]), dtype=T.float)
                        self.nn.train_network(state_tensor, done, hx)

                        hx = hx.detach()
                        self.send_training_complete()
                        break
                    else :
                        self.que_c_statechange_response(state, action)

        if iter >= training_sessions :
            print("finished training session python, shutting down comm thread \n")

        self.receiver.close()
        self.transmitter.close()

    def send_training_complete(self):
        _str_msg = "{trained:" + str(1) + "}"
        print(_str_msg)
        self.receiver.send_string(_str_msg)

    def que_c_statechange_response(self, state, action):
        _str_msg = "{state:"+str(*state) + "," + "action" + ":" + str(action) + "}"

        self.receiver.send_string(_str_msg)

    def mock_test(self, msg ):
        response_type, response = self.decode_topicfilter(msg)

        if response_type == CommResponse.Request:
            print(response_type, response)
        elif response_type == CommResponse.Train:
            print(response_type, response)

    def redirect_request(self, msg):
        pass

    def redirect_train_request(self):
        pass

    def decode_topicfilter(self, msg):

        msg = msg[1:-1]#removing encoding bs


        if msg.find(topic_receive) != -1 :
            print("found topic receive")
            return CommResponse.Request, msg

        return CommResponse.Invalid, ""




