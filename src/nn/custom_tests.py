from icm_test import ICMTest
from comm_interface import ZmqCommInterface
from nn import NeuralNetHandler

from icm import ICM
import torch as T
import numpy as np
import zmq
import time

from actor_critic import ActorCritic
from icm import ICM
from shared_adam_opt import AdamOpt
def test_comminterface():
    n_actions = 4
    input_shape = [1]

    use_icm = True

    global_actor_critic = ActorCritic(input_shape, n_actions)
    global_actor_critic.share_memory()
    global_optim = AdamOpt(global_actor_critic.parameters())

    if not use_icm:
        global_icm = None
        global_icm_optim = None
    else:
        global_icm = ICM(input_shape, n_actions)
        global_icm.share_memory()
        global_icm_optim = AdamOpt(global_icm.parameters())


    nn = NeuralNetHandler(input_shape, n_actions, global_optim = global_optim, global_icm_optim =global_icm_optim,
                          global_ac = global_actor_critic, global_icm = global_icm, use_icm=use_icm   )

    pull_port = "5511"
    push_port = "5557"
    zmq_comm = ZmqCommInterface(pull_port=pull_port, push_port=push_port, nn=nn)
    zmq_comm.start()
    zmq_comm.join()

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

