import numpy as np
import sys
sys.path.append('..')
from model.DeepAE import DeepAutoEncoder as DAE
import torch
from utils.shrink import l21shrink, l1shrink
import torch.nn.functional as F
import os
from tqdm import tqdm
from torch.utils.data import DataLoader


class RobustAutoencoder():

    def __init__(self, input_dim_list, lambda_=5e-6, symmetry= False,learning_rate=1e-4, 
                 inner_iteration = 30, iteration = 20,batch_size = 256, regularization = "l1",
                 device = "cuda", directory = "results", decay = 1, transductive = True):
        self.lambda_ = lambda_  #Controls the level of L1/L21 regularization to separate S and L matrices
        self.input_dim_list = input_dim_list # number of nodes at each level for the internal Autoencoder
        self.learning_rate = learning_rate #learning rate for the internal Autoencoder
        self.inner_iteration = inner_iteration #Autoencoder epochs after separation of L and S
        self.regularization = regularization #regularization type: default = "l1", optional: "l21"
        self.iteration = iteration #outer iterations
        self.batch_size = batch_size #batch size for Autoencoder
        self.directory = directory #directory for saving the results
        self.decay = decay #decay controls the number of nodes in next level. For example: decay = 2 means 784 -> 392
        self.device = device #default device = "cuda" (gpu) , can use "cpu"
        self.transductive = transductive #if the transductive setting is turned on, the prediction becomes L1-norm of the S matrix

        #set up an internal auto encoder and the optimizer
        self.AE = DAE(input_dim_list = self.input_dim_list, symmetry= symmetry, device = self.device)
        self.optimizer = torch.optim.Adam(self.AE.parameters(), lr=self.learning_rate)
        
        #set up the saving directory name
        self.hp_name = str('EncoderLayer-%d Lambda-%10.3e LearningRate-%10.5e InnerIteration-%d Iteration-%d Regularization-%s Decay %d' %
               (len(input_dim_list), self.lambda_, self.learning_rate,
                self.inner_iteration,self.iteration, self.regularization, self.decay))
            
    def fit(self, train_X: np.ndarray):
        """
        X: all of the training data
        """
        X = torch.tensor(train_X)
        self.L = torch.zeros(X.shape)
        self.S = torch.zeros(X.shape)
        
        self.AE = self.AE.to(self.device)
        assert X.shape[1] == self.input_dim_list[0]
        loss_list = []
        
        for it in range(self.iteration):
            self.L = X - self.S

            #  train for smaller iterations to update the AE
            final_loss = 0.0
            for i in range(self.inner_iteration):
                loader = DataLoader(self.L , batch_size= self.batch_size, shuffle=True)
                data = next(iter(loader))
                data = data.to(self.device)
                output = self.AE(data)
                loss = F.mse_loss(output, data)
                final_loss = loss
                self.optimizer.zero_grad()
                loss.backward()
                self.optimizer.step()
            # print("Autoencoder loss: %.15f" % final_loss.item())
            loss_list.append(final_loss.item())

            # get optmized L
            self.L = self.AE(self.L.to(self.device)).cpu()

            # alternating projection, now project to S and shrink S
            if self.regularization== "l1":
                self.S = l1shrink(self.lambda_, (X - self.L).detach().numpy())
            elif self.regularization == "l21":
                self.S = l21shrink(self.lambda_, (X - self.L).detach().numpy())
            self.S = torch.tensor(self.S)
          
        #saving the trained results, in the transductive we should directly use the L1-norm of S as the prediction
        # print("training complete!")
        # self.save_(self.L.detach().numpy(), "L", self.directory)
        # self.save_(self.S.detach().numpy(), "S", self.directory)
        # self.save_(loss_list, "loss", self.directory)
        # print("saving predictions....")
        
        return self.predict(X, "train", self.directory, transductive = self.transductive)
    
    def save_(self, val_list, val_name, directory = "results"):
        # save the results to a directory, default = results
        path = os.path.join(directory, self.hp_name)
        if not os.path.isdir(path):
            os.mkdir(path)
        file = path + "/" + val_name + ".npy"
        np.save(file, np.array(val_list))
        
            
    def predict(self, test_X:np.ndarray , data = "test", directory = "results", transductive = False):
        # in the transductive case, we need to acquire the L1 norm of the S matrix
        if transductive:
            prediction = np.linalg.norm(self.S,ord=1,axis=1)
            #self.save_(prediction, data+ "_prediction", directory)
            return prediction
        
        #otherwise, we use the internal AE to recover the testing data
        #and we identify the higher reconstruction loss -> more likely the data is from anomaly class
        X = torch.tensor(test_X)
        prediction = self.AE(X.to(self.device))
        prediction = prediction.detach().cpu().numpy()
        reconstruction_loss = np.mean(np.square(X.detach().numpy() - prediction), axis = 1)
        #self.save_(reconstruction_loss, data+ "_prediction", directory)
        return reconstruction_loss