import sys
import time
import zmq

context = zmq.Context()

# Socket to receive messages on
receiver = context.socket(zmq.PULL)
receiver.bind("tcp://127.0.0.1:5511")

#use connect on server pull , use bind on publisher client side.
while True:
    print(receiver.recv())