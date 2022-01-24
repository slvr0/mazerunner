
import zmq
import random
import time

context = zmq.Context()

# Socket with direct access to the sink: used to syncronize start of batch
sink = context.socket(zmq.PUSH)
sink.bind("tcp://127.0.0.1:5558")

# Initialize random number generator
random.seed()

def comm_mocktest() :
    return "nn_request_stateresponse{state:3,value:2}"

while True:
    workload = random.randint(1, 100)

    time.sleep(5)
    sink.send_string(comm_mocktest())