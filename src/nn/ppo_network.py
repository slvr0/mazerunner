import tensorflow as tf
from tensorflow.keras import Model

def build_ac_net(input_dims, output_dims, hidden_layer1 = 128, hidden_layer2 = 64) :
  st = tf.keras.layers.Input(shape=input_dims)
  hdl1 = tf.keras.layers.Dense(hidden_layer1, activation='linear')(st)

  layer_actor_pipe = tf.keras.layers.Dense(hidden_layer2, activation='linear')(hdl1)
  layer_critic_pipe = tf.keras.layers.Dense(hidden_layer2, activation='linear')(hdl1)

  actor_out = tf.keras.layers.Dense(output_dims, activation='softmax')(layer_actor_pipe)
  critic_out = tf.keras.layers.Dense(1)(layer_critic_pipe)

  return Model(inputs=st, outputs=[actor_out, critic_out])
































