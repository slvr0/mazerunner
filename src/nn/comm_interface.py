import zmq
from enum import Enum
from nn import NeuralNetHandler
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

        if '[' in kv_split[1] :
            arr_split = kv_split[1][1 : -1].split('/')
            response_keypairs[kv_split[0]] = [int(mv) for mv in arr_split]


        else :
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
class ZmqCommInterface :
    def __init__(self, adress, session_params = None, reporter = None):
        #super(ZmqCommInterface, self).__init__()
        self.context = zmq.Context()

        self.receiver = self.context.socket(zmq.REP)
        self.receiver.bind("tcp://127.0.0.1:{}".format(adress))

        print("[x] python zmq communication interface started")
        #print("interface receiving message from port : {} and sending on port {}".format(pull_port, push_port) )
        print("reply interface receiving message from port : {}".format(adress))

        n_actions = 4
        input_shape = 1

        self.nn = NeuralNetHandler(input_shape, n_actions)

        self.session_params = session_params
        self.reporter = reporter

    def run(self) -> None:

        if self.session_params :
            training_sessions = self.session_params['n_training_sessions']
        else :
            training_sessions = 40000

        iter = 0

        for i in range(training_sessions) :

            state = None
            action = None
            done = False
            step = -1
            scores = [0] * 100

            #for i in range(n_sessions):

                #scores[i % 100] = session_score

            session_score = 0
            while True :
                message = self.receiver.recv()
                str = decode_response(message) #decodes the actual values in message

                new_state = int(float(str['state']))
                reward = float(str['reward'])
                do_train = int(float(str['train']))

                if 'mask' in str.keys() :
                    _mask = str['mask']
                    _mask = [int(float(v)) for v in _mask]
                else :
                    _mask = None


                step += 1
                session_score += reward

                if step == 0: #initiate process
                    state = new_state
                    action = self.nn.request_stateresponse(state, _mask)
                    self.que_c_statechange_response(state, action)
                else :
                    new_action = self.nn.request_stateresponse(state, _mask)
                    done = True if reward == 1 else False

                    self.nn.store_statetransition(state, action, reward, new_state, done)

                    state = new_state
                    action = new_action

                    if do_train == 1:

                        self.nn.train_network(new_state, done)
                        iter += 1
                        self.send_training_complete()
                        break
                    else :
                        self.que_c_statechange_response(state, action)

            scores[i % 100] = session_score
            #print(" score : {:1f}".format(session_score), "Average score : ", int(np.mean(scores, axis=0)))

        #if iter >= training_sessions :
            #print("finished training session python, shutting down comm thread \n")


        self.receiver.close()

    def send_training_complete(self):
        _str_msg = "{trained:" + str(1) + "}"

        self.receiver.send_string(_str_msg)

    def que_c_statechange_response(self, state, action):
        try :
            _str_msg = "{state:"+str(*state) + "," + "action" + ":" + str(action) + "}"
        except :
            _str_msg = "{state:" + str(state) + "," + "action" + ":" + str(action) + "}"

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




