
from keras.models import Sequential
from keras.layers import Dense
from tensorflow.keras.optimizers import Adam
import tensorflow as tf

class NetworkModel :
    model = None

    def __init__(self, input_size, output_size, save_dir = "models"):
        self.input_size = input_size
        self.output_size = output_size

    def save_model(self):
        pass

def build_kerasmodel(input, output) :
    tf.keras.backend.clear_session()

    model = Sequential()

    model.add(Dense(12, activation='relu', input_dim=input))
    model.add(Dense(12, activation='relu'))
    model.add(Dense(output))


    model.compile(loss='mse',optimizer=Adam(learning_rate=0.001))



    return model

