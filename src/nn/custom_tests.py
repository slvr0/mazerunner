from comm_interface import ZmqCommInterface

import numpy as np
import zmq
import time
from network import build_kerasmodel

def test_kerasnet() :

    inpd = 1
    outpd = 4

    model = build_kerasmodel(inpd, outpd)

    mock_input = np.array([1])

    out = model.predict(mock_input)

def test_comminterface():


    reply_port = "5533"
    pull_port = "5511"
    push_port = "5557"
    zmq_comm = ZmqCommInterface(adress=reply_port)
    zmq_comm.run()
    # zmq_comm.start()
    # zmq_comm.join()

def test_test_icm():
    input_dims = [1]
    output_dims = 4
    icm_test = ICMTest(input_dims, output_dims)

    mock_state = np.array([2,3,4])
    mock_state_t = T.tensor(mock_state, dtype=T.float).unsqueeze(dim=-1)
    print(mock_state_t.size())
    result = icm_test(mock_state_t)

def test_icm() :
    n_actions = 4
    input_shape = [1]

    icm = ICM(input_shape, n_actions)

    mock_state = [1]
    mock_new_state = [3]
    mock_action = [2]

    mock_states = np.array([1,2,3])
    mock_new_states = np.array([1,2,3])
    mock_actions = np.array([1,2,3])

    ##the takeaway from this experiment is that input layer of size N needs [i,N] input size
    ##whereas tf is [N,i] structured

    # states = T.tensor([mock_states], dtype=T.float)
    # new_states = T.tensor([mock_new_states], dtype=T.float)
    # actions = T.tensor([mock_actions], dtype=T.float)
    #
    # states = T.reshape(states, (-1,)).unsqueeze(dim=-1)
    # actions = T.reshape(actions, (-1,)).unsqueeze(dim=-1)
    # new_states = T.reshape(new_states, (-1,)).unsqueeze(dim=-1)

    intrinsic_reward, L_I, L_F = \
        icm.calc_loss(mock_states, mock_new_states, mock_actions)


def test_zmq():
    context = zmq.Context()
    socket = context.socket(zmq.REP)
    socket.bind("tcp://*:5555")

    while True:
        #  Wait for next request from client
        message = socket.recv()
        print("Received request: %s" % message)

        #  Do some 'work'
        time.sleep(1)

        #  Send reply back to client
        socket.send(b"World")

from keras.layers import concatenate, Lambda
from keras import Sequential, Input, Model
import keras.backend as K

