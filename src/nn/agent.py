from collections import deque

import torch as T
import random
import math
import numpy as np
import torch.autograd as autograd
from network import build_kerasmodel

import tensorflow as tf
from memory import Memory

from tensorflow.python.framework.ops import disable_eager_execution
disable_eager_execution()

import abc

class Agent :
  def __init__(self, training_batch_size, env_input_dims, env_output_dims):
    #self.memory = deque(maxlen=10000)
    self.memory = Memory()
    self.training_batch_size = training_batch_size

    self.input_dims = env_input_dims
    self.output_dims = env_output_dims

  @abc.abstractmethod
  def react(self, state):
    pass

  def memorize(self, state, action, reward, new_state, done):
    self.memory.remember((state, action, reward, new_state, done))

  @abc.abstractmethod
  def train(self):
    pass

torch_dtype = T.cuda.FloatTensor if T.cuda.is_available() else T.FloatTensor
class Variable(autograd.Variable):
  def __init__(self, data, *args, **kwargs):
    if T.cuda.is_available():
      data = data.cuda()
    super(Variable, self).__init__(data, *args, **kwargs)

class DQN_KerasAgent(Agent) :
  def __init__(self, model, training_batch_size, env_input_dims, env_output_dims, eps_decayrate = 0.99995, eps_start = 0.9, eps_end= 0.05, gamma_f = 0.99):
    super(DQN_KerasAgent, self).__init__(training_batch_size, env_input_dims, env_output_dims)

    self.model = model #build_kerasmodel(env_input_dims, env_output_dims) # no target model even atm


    self.eps_decayrate = eps_decayrate
    self.eps_start = eps_start
    self.eps_end = eps_end
    self.eps = eps_start
    self.gamma_f = gamma_f

  def react(self, state):
    if self.eps > self.eps_end:
      self.eps *= self.eps_decayrate

    state = np.expand_dims(state, axis=0)

    return np.random.randint(0, self.output_dims) if (np.random.random() <= self.eps) \
      else np.argmax(self.model.predict(state))

  def train(self):
    x_batch, y_batch = [], []

    transitions = self.memory.sample_memory()

    for state, action, reward, next_state, done in transitions:
      #print(state, action ,reward, next_state, done)

      state = np.expand_dims(state, axis=0)
      next_state = np.expand_dims(next_state, axis=0)

      y_target = self.model.predict(state)
      y_target[0][int(action)] = reward if done else reward + self.gamma_f * np.max(self.model.predict(next_state)[0])
      x_batch.append(state[0])
      y_batch.append(y_target[0])

    self.model.fit(np.array(x_batch), np.array(y_batch), batch_size=len(x_batch), verbose=0)

    self.memory.clear_memory()












