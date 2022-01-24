# -*- coding: utf-8 -*-

import numpy as np
import time
class MazeEnvironment :
    def __init__(self, client_process):

        self.start_state = [3]
        self.state = self.start_state

        self.client_process = client_process

        self.info = dict()

    def reset(self):
        print("reset state")
        self.state = self.start_state


        return np.array([self.state])

    def step(self, action):
        #sends request to c++ environment asking what are action yields
        #self.client_process.publish(f'{ {}: }'.format(self.state, action))

        str_msg = "{" + "state:"+str(*self.state) + ","  + "action" +  ":" + str(action) + "}"

        self.client_process.publish(str_msg)

        #waits until c++ gives a response
        while self.client_process.awaits_response() :

            self.client_process.publish(str_msg)
            continue

        #updates environment from new data
        state_reply = self.client_process.return_response()[0]


        self.state = [int(state_reply[0])]
        reward = int(state_reply[1])

        self.client_process.clear_response()

        return np.array([self.state]), reward, True if self.state == 16 else False, self.info








