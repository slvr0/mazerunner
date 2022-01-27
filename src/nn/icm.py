import torch as T
import torch.nn as nn
import numpy as np
import torch.nn.functional as F

class ICM(nn.Module):
    def __init__(self, input_dims, n_actions=4, alpha=1, beta=0.2):
        super(ICM, self).__init__()
        self.alpha = alpha
        self.beta = beta

        self.inverse = nn.Linear(2, 256)
        self.pi_logits = nn.Linear(256, n_actions)

        self.dense1 = nn.Linear(1+1, 256)
        self.new_state = nn.Linear(256, *input_dims)

        device = T.device('cpu')
        self.to(device)

    def forward(self, state, new_state, action):

        inverse = F.elu(self.inverse(T.cat([state, new_state], dim=1)))

        pi_logits = self.pi_logits(inverse)

        action = action.reshape((action.size()[0], 1))

        forward_input = T.cat([state, action], dim=1)

        dense = F.elu(self.dense1(forward_input))
        state_ = self.new_state(dense)

        return pi_logits, state_

    def calc_loss(self, state : np.array, new_state : np.array, action : np.array):
        try :
            tensor_states = T.tensor([state], dtype=T.float)
            tensor_new_states = T.tensor([new_state], dtype=T.float)
            tensor_actions = T.tensor([action], dtype=T.long).squeeze()

            tensor_states = T.reshape(tensor_states, (-1,)).unsqueeze(dim=-1)
            tensor_new_states = T.reshape(tensor_new_states, (-1,)).unsqueeze(dim=-1)

            pi_logits, state_ = self.forward(tensor_states, tensor_new_states, tensor_actions)

            inverse_loss = nn.CrossEntropyLoss()
            L_I = (1-self.beta)*inverse_loss(pi_logits, tensor_actions)

            forward_loss = nn.MSELoss()
            L_F = self.beta*forward_loss(state_, tensor_new_states)

            intrinsic_reward = self.alpha*((state_ - tensor_new_states).pow(2)).mean(dim=1)
            return intrinsic_reward, L_I, L_F
        except :
            print(type(state) , state.shape, new_state.shape)