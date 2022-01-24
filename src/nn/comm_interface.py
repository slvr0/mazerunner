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
    def __init__(self, pull_port, push_port, session_params = None, nn = None, reporter = None):
        super(ZmqCommInterface, self).__init__()
        self.context = zmq.Context()
        self.receiver = self.context.socket(zmq.PULL)
        self.receiver.bind("tcp://127.0.0.1:{}".format(pull_port))

        self.transmitter = self.context.socket(zmq.PUSH)
        self.transmitter.connect("tcp://127.0.0.1:{}".format(push_port))

        print("[x] python zmq communication interface started")
        print("interface receiving message from port : {} and sending on port {}".format(pull_port, push_port) )

        self.nn = nn
        self.session_params = session_params
        self.reporter = reporter

        self.memory = Memory()

    def run(self) -> None:
        if self.session_params :
            training_sessions = self.session_params['n_training_sessions']
        else :
            training_sessions = 20

        iter = 0
        for i in range(training_sessions) :
            hx = T.zeros(1, 256)

            state = [3] #hardcoded startstate, fix!
            action = None
            while True :

                message = self.receiver.recv()

                # response_type, response = self.decode_topicfilter(
                #     message.__str__())  # decodes the message id to take appropriate response


                str = decode_response(message) #decodes the actual values in message


                if 'reward' in str.keys() : #then we continue here
                    #read out the new state and reward for last action from message
                    new_state = [int(str['state'])]
                    reward = int(str['reward'] if str.__contains__('reward') else 0)

                    #convert to tensor
                    state_tensor = T.tensor(np.array([state]), dtype=T.float)
                    new_action, value, log_prob, hx = self.nn.request_statemetrics(state_tensor, hx)

                    #fix sending done messages. store data
                    self.memory.remember(state, action, reward, new_state, value, log_prob)

                    state = new_state
                    action = new_action #save, for next iteration of batch memory
                    #send response

                else : #first iteration gets here
                    # read out the new state and reward for last action from message
                    state = [int(str['state'])]

                    # convert to tensor
                    state_tensor = T.tensor(np.array([state]), dtype=T.float)
                    action, value, log_prob, hx = self.nn.request_statemetrics(state_tensor, hx)
                    # send response

                print("nn responding with {} and taking action {}".format(state, action))
                self.que_c_statechange_response(state, action)


                    #
                    #     # we send back where our current state and provide the action we choose
                    #
                    #     self.que_c_statechange_response(state, action)
                    #
                    #     while True : # now we expect the result of action from c env
                    #         _message = self.receiver.recv()
                    #
                    #         response_type, response = self.decode_topicfilter(_message.__str__())
                    #
                    #         print("this is where we expect result message", _message.__str__(), response_type, response)
                    #
                    #         if response_type == CommResponse.Request_ActionResult :
                    #
                    #             break
                # response_type , response = self.decode_topicfilter(message.__str__()) #decodes the message id to take appropriate response
                #
                # if response_type == CommResponse.Request :
                #     str = decode_response(response) #decodes the actual values in message
                #
                #     state = [int(str['state'])]
                #
                #     state_tensor = T.tensor(np.array([state]), dtype=T.float)
                #
                #     reward = int(str['reward'] if str.__contains__('reward') else 0)
                #
                #     action, value, log_prob, hx = self.nn.request_statemetrics(state_tensor, hx)
                #
                #     # we send back where our current state and provide the action we choose
                #
                #     self.que_c_statechange_response(state, action)
                #
                #     while True : # now we expect the result of action from c env
                #         _message = self.receiver.recv()
                #
                #         response_type, response = self.decode_topicfilter(_message.__str__())
                #
                #         print("this is where we expect result message", _message.__str__(), response_type, response)
                #
                #         if response_type == CommResponse.Request_ActionResult :
                #             obs_, reward, done, info = decode_received_actionresult(response)
                #             self.memory.remember(state, action, reward, obs_, value, log_prob)
                #             break
                #
                #     if done : # go directly to train or something
                #         pass
                #
                #     continue

                # elif response_type == CommResponse.Request_Noresp: # a test run or first msg, either way,
                #     # we're not expecting to loop for a result
                #     state, _ = decode_response(response)
                #     action, value, log_prob = self.nn.request_statemetrics(state)
                #     self.que_c_statechange_response(state, action)
                #
                # elif response_type == CommResponse.Train :
                #     #this occurs when the environment has been reset/finished
                #
                #     iter += 1
                #     self.nn.reset_env()
                #     self.memory.clear_memory()
                #     hx = hx.detach()
                #     break

        if iter >= training_sessions :
            print("finished training session python, shutting down comm thread \n")

    def que_c_statechange_response(self, state, action):
        _str_msg = "{state:"+str(*state) + ","  + "action" +  ":" + str(action) + "}"

        self.transmitter.send_string(_str_msg)

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




