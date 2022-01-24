
import torch as T
import torch.nn.functional as F
import torch.nn as nn


class  ICMTest(nn.Module) :
    def __init__(self, input_dims, output_dims):
        super(ICMTest, self).__init__()

        self.input_layer = nn.Linear(*input_dims, output_dims)

        device = T.device('cpu')
        self.to(device)



    def forward(self, state):
        return self.input_layer(state)