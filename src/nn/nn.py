from actor_critic import ActorCritic
from icm import ICM
import torch as T
import torch.nn.functional as F
import numpy as np
from memory import Memory

class NeuralNetHandler :
    def __init__(self, input_dims, output_dims, global_optim , global_icm_optim, global_ac, global_icm = None , use_icm = True):

        self.local_ac = self.create_a3c(input_dims, output_dims)

        if use_icm :
            self.local_icm = self.create_icm(input_dims, output_dims)

        self.memory = Memory()
        self.use_icm = use_icm

        self.global_ac = global_ac
        self.global_icm = global_icm

        self.global_optim = global_optim
        self.icm_global_optim = global_icm_optim

    def create_a3c(self, input_dims, output_dims):
        return ActorCritic(input_dims, output_dims)

    def create_icm(self, input_dims, output_dims):
        return ICM(input_dims, output_dims)

    def store_statetransition(self, obs, action, reward, obs_, value, log_prob):
        self.memory.remember(obs, action, reward, obs_, value, log_prob)

    def request_statemetrics(self, state, hx):
        return self.local_ac(state, hx)

    def train_network(self, last_state, done, hx_metrics):
        states, actions, rewards, new_states, values, log_probs = \
            self.memory.sample_memory()

        #print("training on {} samples...".format(len(self.memory)))

        if self.use_icm:
            np_states = np.array(states)
            np_new_states = np.array(new_states)
            np_actions = np.array(actions)

            intrinsic_reward, L_I, L_F = \
                self.local_icm.calc_loss(np_states, np_new_states, np_actions)

        loss = self.local_ac.calc_loss(last_state, hx_metrics, done, rewards, values,
                                     log_probs, intrinsic_reward)

        self.global_optim.zero_grad()
        hx = hx_metrics.detach_()
        if self.use_icm:
            self.icm_global_optim.zero_grad()
            (L_I + L_F).backward()

        loss.backward()
        T.nn.utils.clip_grad_norm_(self.local_ac.parameters(), 40)

        for local_param, global_param in zip(
                self.local_ac.parameters(),
                self.global_ac.parameters()):
            global_param._grad = local_param.grad
        self.global_optim.step()
        self.local_ac.load_state_dict(self.global_ac.state_dict())

        if self.use_icm:
            for local_param, global_param in zip(
                    self.local_icm.parameters(),
                    self.global_icm.parameters()):
                global_param._grad = local_param.grad
            self.icm_global_optim.step()
            self.local_icm.load_state_dict(self.global_icm.state_dict())

        self.memory.clear_memory()
        print("memory entries cleared : entries now {}".format(len(self.memory)))

        return hx
