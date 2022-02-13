import os

from custom_tests import test_icm, test_zmq, test_comminterface, test_kerasnet

os.environ['CUDA_DEVICE_ORDER'] = 'PCI_BUS_ID'
os.environ['CUDA_VISIBLE_DEVICES'] = '0'
import numpy as np

# def asdf_test(msg) :
#     def decode_response(msg):
#         ex = msg[1:-1]  # remove brackets
#         response_keypairs = {}
#
#         each_kv_p = ex.split(',')
#
#         for kvp in each_kv_p:
#             kv_split = kvp.split(':')
#             response_keypairs[kv_split[0]] = kv_split[1]
#
#         return response_keypairs
#
#     print(decode_response(msg))
#

if __name__ == '__main__':

    #asdf_test("{state:3}")
    test_comminterface()
    #test_kerasnet()
    #test_curiousy_icm()






