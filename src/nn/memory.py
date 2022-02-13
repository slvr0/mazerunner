import numpy as np

class Memory :
    def __init__(self):
        self.transitions = []

    def remember(self, transition_tuple):
        self.transitions.append(transition_tuple)

    def clear_memory(self):
        self.transitions.clear()

    def __len__(self):
        return len(self.transitions)

    def sample_memory(self):
        return np.array(self.transitions)