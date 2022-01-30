from actor_critic import ActorCritic
from icm import ICM
import torch as T
import torch.nn.functional as F
import numpy as np
from memory import Memory
from agent import DQN_KerasAgent

class NeuralNetHandler :
    def __init__(self, model, input_dims, output_dims):

        batch_size = 50
        self.agent = DQN_KerasAgent(model, batch_size, input_dims, output_dims)
        self.training_epochs = 0

    def create_a3c(self, input_dims, output_dims):
        return ActorCritic(input_dims, output_dims)

    def create_icm(self, input_dims, output_dims):
        return ICM(input_dims, output_dims)

    def store_statetransition(self, obs, action, reward, obs_, done):
        self.agent.memorize(obs, action, reward, obs_, done)

    def request_stateresponse(self, state):
        return self.agent.react(state)

    def train_network(self, last_state, done):
        self.agent.train()

