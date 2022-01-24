class Memory :
    def __init__(self):
        self.states = []
        self.actions = []
        self.rewards = []
        self.new_states = []
        self.values = []
        self.log_probs = []

    def remember(self, state, action, reward , new_state, value, log_p):
        self.states.append(state)
        self.actions.append(action)
        self.rewards.append(reward)
        self.new_states.append(new_state)
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

    def sample_memory(self):
        return self.states, self.actions , self.rewards , self.new_states, self.values, self.log_probs