import os.path

import numpy as np

import abc
import tensorflow as tf

from tensorflow.keras.optimizers import Adam, SGD, RMSprop
from ppo_network import build_ac_net
import scipy
from scipy.signal import lfilter
import copy

class Agent :
  def __init__(self, env_input_dims, env_output_dims, training_batch_size = 64):

    self.training_batch_size = training_batch_size

    self.input_dims = env_input_dims
    self.output_dims = env_output_dims

  @abc.abstractmethod
  def react(self, state):
    pass

  @abc.abstractmethod
  def train(self):
    pass

class PPO_AGENT(Agent) :
  def __init__(self, input_dims, output_dims, gamma_param = 0.99, lambda_param = 0.95, ppo_learning_rate = 1e-4,
               network_name='mazerunner2_actor_critic', chkpt_dir='tmp/'):
    super(PPO_AGENT, self).__init__(input_dims, output_dims)


    # PPO hyperparams
    self.learning_rate = ppo_learning_rate

    self.gamma_f = gamma_param
    self.lambda_f = lambda_param

    self.input_dims = input_dims
    self.output_dims = output_dims

    self.network_name = network_name
    self.checkpoint_dir = chkpt_dir

    self.network_savepath_full = self.checkpoint_dir  + self.network_name

    # if os.path.isdir(self.network_savepath_full) :
    #   print("loaded network from filepath {}".format(self.network_savepath_full))
    #   self.net = self.load_network()
    #
    # else :
    self.net = build_ac_net(input_dims, output_dims)
    self.net.compile(optimizer=Adam(learning_rate=self.learning_rate, ))

    self.batch_size = 16
    self.kl_div_fact = 0.1
    self.lr = 0.001
    self.max_train_iters = 3
    self.ppo_clip_ratio = 0.2
    self.normalize = True

  def save_model(self):
    tf.keras.models.save_model(self.net, self.network_savepath_full)

  def load_network(self):
    return tf.keras.models.load_model(self.network_savepath_full)

  def get_discount_sum(self, x, value, discount_fac=0.99):
    adv = np.stack(lfilter([1], [1, -discount_fac], x[::-1], axis=0)[::-1])
    adv = (adv - adv.mean()) / (adv.std() + 1e-8)

    target = adv + value
    return adv, target

  def react(self, state, _mask = None):
    state = tf.convert_to_tensor([state], dtype=tf.float32)
    policy, _ = self.net.call(state)

    policy = np.array(policy)[0]
    pol_sum = 0

    if _mask :
      __m = np.zeros_like(policy)
      __m[_mask] = 1
      policy *= __m
      policy = policy + policy * 0.1 * np.random.rand(self.output_dims)
      pol_sum = policy.sum()
      policy /= pol_sum

    action = np.random.choice(self.output_dims, p=policy)

    return action, policy.std()

  def learn(self, state, action, next_state, reward, terminals):

    # preprocessing for batch training
    probs, values = self.net.call(tf.convert_to_tensor(state, dtype=tf.float32))
    _, next_values = self.net.call(tf.convert_to_tensor(next_state, dtype=tf.float32))
    values, next_values = tf.squeeze(values), tf.squeeze(next_values)

    # get advantages and targets
    deltas = [r + self.gamma_f * (1 - d) * nv - v for r, d, nv, v in zip(reward, terminals, next_values, values)]
    adv, target = self.get_discount_sum(deltas, values, self.gamma_f * self.lambda_f)

    iter = 0
    kl_div = 0
    while iter < self.max_train_iters and kl_div < self.kl_div_fact:
      net_variables = self.net.trainable_variables

      with tf.GradientTape() as tape:
        tape.watch(net_variables)

        # feed data through network to gain updated policies
        train_probs, train_values = self.net.call(tf.convert_to_tensor(state, dtype=tf.float32))
        train_values = tf.squeeze(train_values)

        # convert data to tensors
        adv_tensor = tf.convert_to_tensor(adv, dtype=tf.float32)
        target_tensor = tf.convert_to_tensor(target, dtype=tf.float32)
        actions_tensor = tf.convert_to_tensor(action, dtype=tf.int32)
        probs_tensor = tf.convert_to_tensor(probs, dtype=tf.float32)

        entropy = tf.reduce_mean(-train_probs * tf.math.log(train_probs + 1e-8)) * 0.1

        # one hot encode the actions taken, reduce the probability tensors from this
        action_onehot = tf.one_hot(actions_tensor, self.output_dims)

        decisive_probs = tf.reduce_sum(train_probs * action_onehot, axis=1)
        old_decisive_probs = tf.reduce_sum(probs_tensor * action_onehot, axis=1)

        logged_dec_probs = tf.math.log(decisive_probs + 1e-8)
        old_logged_dec_probs = tf.math.log(old_decisive_probs + 1e-8)

        # calculate the ratio of gradient change from this
        ratio = tf.exp(logged_dec_probs - old_logged_dec_probs)

        # clip that data, extract minimum
        clipped_ratio = tf.clip_by_value(ratio, clip_value_min=1 - self.ppo_clip_ratio,
                                         clip_value_max=1 + self.ppo_clip_ratio)

        min_clipped_ratio = tf.minimum(tf.multiply(adv_tensor, clipped_ratio), tf.multiply(adv_tensor, ratio))
        actor_loss = -tf.reduce_mean(min_clipped_ratio) + entropy

        # calculate the actor / critic loss, feed to the optimizer
        critic_loss = tf.reduce_mean(tf.square(target_tensor - train_values))

        total_loss = actor_loss + critic_loss

        # calculate kl_div
        kl_div = tf.reduce_mean(logged_dec_probs - old_logged_dec_probs)
        iter += 1

      grads = tape.gradient(total_loss, net_variables)

      self.net.optimizer.apply_gradients(zip(grads, net_variables))

























