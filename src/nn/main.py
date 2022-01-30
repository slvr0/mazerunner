import os
import torch.multiprocessing as mp
os.environ['OMP_NUM_THREADS'] = '1'
from parallel_env import ParallelEnvironment
from custom_tests import test_icm, test_zmq, test_comminterface, test_kerasnet

import os

os.environ['CUDA_DEVICE_ORDER'] = 'PCI_BUS_ID'
os.environ['CUDA_VISIBLE_DEVICES'] = '0'

def asdf_test(msg) :
    def decode_response(msg):
        ex = msg[1:-1]  # remove brackets
        response_keypairs = {}

        each_kv_p = ex.split(',')

        for kvp in each_kv_p:
            kv_split = kvp.split(':')
            response_keypairs[kv_split[0]] = kv_split[1]

        return response_keypairs

    print(decode_response(msg))



def initiate_training() :
    mp.set_start_method('spawn')
    env_id = 'CartPole-v0'
    n_threads = 1
    n_actions = 4
    input_shape = 1

    env = ParallelEnvironment(env_id=env_id, n_threads=n_threads,
                       n_actions=n_actions, input_shape=input_shape, icm=True)


if __name__ == '__main__':
    #asdf_test("{state:3}")
    test_comminterface()
    #test_kerasnet()




#




