import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.utils.data import DataLoader
from tqdm import tqdm
import numpy as np

class DeepAutoEncoder(nn.Module):
    def __init__(self, input_dim_list = [784, 400], symmetry = True, device = "cuda"):
        super(DeepAutoEncoder, self).__init__()
        assert len(input_dim_list) >= 2
        self.input_dim = len(input_dim_list)
        self.symmetry = symmetry
        self.device = device
        self.encoder_layer_list = nn.ModuleList()
        #initialize the weights from the encoder part
        for i in range(len(input_dim_list) - 1):
            self.encoder_layer_list.append(nn.Linear(in_features= input_dim_list[i],
                                                      out_features= input_dim_list[i+1]))
        #if the autoencoder does not enforce symmetry, we need to initialize the weights for the decoder part
        output_dim_list = input_dim_list[::-1]
        if not symmetry:
            self.decoder_layer_list = nn.ModuleList()
            for i in range(len(output_dim_list) -1 ):
                self.decoder_layer_list.append(nn.Linear(in_features= output_dim_list[i],
                                                      out_features= output_dim_list[i+1]))
        elif symmetry:
            #initialize the decoder bias
            self.bias_list = []
            for i in range(len(output_dim_list) -1):
                self.bias_list.append(torch.nn.Parameter(torch.rand(output_dim_list[i+1])).to(self.device))


    def forward(self, x):
        weight_list = []
        for i in range(self.input_dim - 1):
            x = self.encoder_layer_list[i](x)
            weight_list.append(self.encoder_layer_list[i].weight)
            x = torch.sigmoid(x)
        if self.symmetry:
            weight_list = weight_list[::-1]
            for i in range(len(weight_list)-1):
                x = F.linear(x, torch.t(weight_list[i])) + self.bias_list[i]
                x = torch.sigmoid(x)
            output = F.linear(x, torch.t(weight_list[-1])) + self.bias_list[-1]
        elif not self.symmetry:
            for i in range(len(self.decoder_layer_list)-1):
                x = self.decoder_layer_list[i](x)
                x = torch.sigmoid(x)
            output = self.decoder_layer_list[-1](x)
        return output
    

class AEClassifier():
    
    def __init__(self, input_dim_list = [784, 400], symmetry = True, device= "cuda", lr = 1e-3, batch_size=200, epochs = 250):
        self.device = device
        self.lr = lr
        self.batch_size = batch_size
        self.model =  DeepAutoEncoder(input_dim_list, symmetry= True, device = device)
        self.model = self.model.to(self.device)
        self.optimizer = torch.optim.Adam(self.model.parameters(), lr=self.lr)
        self.epochs = epochs
        
    
    def fit(self, dataset):
        #fit the internal model
        #return losses: should be number of epochs
        loader = DataLoader(dataset, batch_size= self.batch_size, shuffle=True)
        losses = []
        for it in range(self.epochs):
            for data in loader:
                if type(dataset) == np.ndarray:
                    image_batch = data
                else:
                    image_batch, _ = data
                image_batch = image_batch.to(self.device)
                output = self.model(image_batch)
                loss = F.mse_loss(output, image_batch)
               # Backward pass
                self.optimizer.zero_grad()
                loss.backward()
                self.optimizer.step()
            losses.append(loss.item())
        return losses
    
    def predict(self, dataset):
        if type(dataset) == np.ndarray:
            test_X = torch.tensor(dataset).float().to(self.device)
        else:
            loader = DataLoader(dataset, batch_size = len(dataset), shuffle = False)
            test_X, _ = next(iter(loader))
            test_X = test_X.to(self.device)
            
        output = self.model(test_X)
        reconstruction_loss = np.mean(np.square((output- test_X).detach().cpu().numpy()), axis = 1)
        return reconstruction_loss