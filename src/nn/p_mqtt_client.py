import paho.mqtt.client as mqtt

MQTT_BROKER_HOST = 'localhost'
MQTT_BROKER_PORT = 1883
MQTT_KEEP_ALIVE_INTERVAL = 60

import threading

import torch.multiprocessing as mp
import os

os.environ['OMP_NUM_THREADS'] = '1'

def on_connect(client, userdata, flags, rc):
  print("Connected with result code " + str(rc))

def on_message(client, userdata, msg):
  print(msg.topic + " " + str(msg.payload))

def threaded_loop_forever(client):

  #client = MqttClient('nn_response_{}'.format(thread_id), on_msg_fptr=callback_method)
  client.initiate()

#for threaded requests, this broker distributes messages to separate threads
class MqttRequestHandler() :
  def __init__(self, n_threads):
    self.send_requests = [''] * n_threads
    self.received_requests = [''] * n_threads
    self.client = MqttClient('nn_response')

  def on_receive_dist(self, msg):
    pass

class MqttProcess(threading.Thread) :
  def __init__(self, thread_id,):

    self.client = MqttClient('nn_response_{}'.format(thread_id), 'nn_send_{}'.format(thread_id), on_msg_fptr=self.set_response_callback)

    super(MqttProcess, self).__init__()
    #self.client = MqttClient('nn_response_{}'.format(thread_id) , on_msg_fptr = self.set_response)


    self.response = ''

  def awaits_response(self):
    return self.response == ''

  def clear_response(self):
    self.response = ''

  def return_response(self):
    ex = self.response
    ex_split = ex.split('{')

    rest = ex_split[1][:-1]

    splitted = rest.split(":")

    key = splitted[0::2]
    value = splitted[1::2]

    response_keypairs = []

    for _k, _v in zip(key, value):
      response_keypairs.append((_k, _v))

    return response_keypairs

  def run(self):
    self.client.initiate()

  def set_response_callback(self, client, userdata, msg) :
    self.response = msg.payload.decode('utf-8')

  def publish(self, msg) :
    self.client.publish(msg)

  def get_msg(self):
    return self.response

class MqttClient() :

  def __init__(self, receive_topic, respond_topic, on_msg_fptr = on_message, host_name = MQTT_BROKER_HOST, host_port = MQTT_BROKER_PORT):
    self.client = mqtt.Client()

    self.receive_topic = receive_topic
    self.respond_topic = respond_topic
    self.host_name = host_name
    self.host_port = host_port

    self.client.on_connect = self.on_conn
    self.client.on_message = on_msg_fptr

    self.client.connect("localhost", 1883, 60)

    self.client.subscribe(self.receive_topic)



  def publish(self, msg, topic = ""):
    if topic == "" : topic = self.respond_topic
    self.client.publish(topic, str(msg).encode('utf-8'))

  def on_conn(self, client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

  def on_message(self, client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))

  def initiate(self):
    self.client.loop_forever()
