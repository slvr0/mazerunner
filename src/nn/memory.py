import numpy as np

class Memory :
    def __init__(self):
        self.states = []
        self.actions = []
        self.rewards = []
        self.new_states = []
        self.values = []
        self.log_probs = []

    def remember(self, state, action, reward , new_state, value, log_p):
        # print(type(state))
        # print(type(action))
        # print(type(reward))
        # print(type(new_state))
        # print(type(value))
        # print(type(log_p))

        self.states.append(np.array(state))
        self.actions.append(np.array(action))
        self.rewards.append(reward)
        self.new_states.append(np.array(new_state))
        self.values.append(value)
        self.log_probs.append(log_p)

    def clear_memory(self):
        self.states.clear()
        self.actions.clear()
        self.rewards.clear()
        self.new_states.clear()
        self.new_states.clear()
        self.values.clear()
        self.log_probs.clear()

    def __len__(self):
        return len(self.states)

    def sample_memory(self):
        return self.states, self.actions , self.rewards , self.new_states, self.values, self.log_probs