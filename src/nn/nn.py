from ppo_agent import PPO_AGENT

class NeuralNetHandler :
    def __init__(self, input_dims, output_dims):

        gamma_param = .99
        lamda_param = .95

        self.agent = PPO_AGENT(input_dims, output_dims, gamma_param=gamma_param, lambda_param=lamda_param)

        self.training_epochs = 0

        self.states = []
        self.actions = []
        self.rewards = []
        self.next_states = []
        self.terminals = []
        self.prob_sum = 0
    def store_statetransition(self, obs, action, reward, obs_, done):
        self.states.append(obs)
        self.actions.append(action)
        self.rewards.append(reward)
        self.next_states.append(obs_)
        self.terminals.append(done)

    def clear_memory(self):
        self.states = []
        self.actions = []
        self.rewards = []
        self.next_states = []
        self.terminals = []

    def request_stateresponse(self, state, _mask = None):

        action,  self.prob_sum = self.agent.react(state, _mask)
        return action

    def train_network(self, last_state, done):
        #self.next_states.append(last_state)
        #self.terminals.append(done)

        # if done :
        #     print(self.states)
        #     print(self.actions)
        #     print(self.next_states)
        #     print(self.rewards)
        #     print(self.terminals)


        #if done : #change this to mean value of rewards > 0

        self.training_epochs +=1

        self.agent.learn(self.states, self.actions, self.next_states, self.rewards, self.terminals)

        self.clear_memory()

