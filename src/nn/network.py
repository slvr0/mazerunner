import keras
import tensorflow as tf
import numpy as np

class NetworkModel :
    model = None

    def __init__(self, input_size, output_size, save_dir = "models"):
        self.input_size = input_size
        self.output_size = output_size

    def save_model(self):
        pass

