import torch
import torch.nn as nn
import torch.nn.functional as F
import time


import torchvision
from torchvision import transforms

import numpy as np
import random


def generate_data(normal_class = 4, transductive= True, flatten = True, GCN = False):
    if transductive:
        return generate_transductive_dataset(normal_class = normal_class, data_dir= 'dataset', flatten = flatten, GCN = GCN)       
    else:
        return generate_disjoint_dataset(normal_class = normal_class, data_dir= 'dataset', flatten = flatten, GCN = GCN)
           

#MNIST precomputed min_max score
# Pre-computed min and max values (after applying GCN) from train data per class
mnist_min_max = [(-0.8826567065619495, 9.001545489292527),
                   (-0.6661464580883915, 20.108062262467364),
                   (-0.7820454743183202, 11.665100841080346),
                   (-0.7645772083211267, 12.895051191467457),
                   (-0.7253923114302238, 12.683235701611533),
                   (-0.7698501867861425, 13.103278415430502),
                   (-0.778418217980696, 10.457837397569108),
                   (-0.7129780970522351, 12.057777597673047),
                   (-0.8280402650205075, 10.581538445782988),
                   (-0.7369959242164307, 10.697039838804978)]


def global_contrast_normalization(x: torch.tensor, scale='l2'):
    """
    Apply global contrast normalization to tensor, i.e. subtract mean across features (pixels) and normalize by scale,
    which is either the standard deviation, L1- or L2-norm across features (pixels).
    Note this is a *per sample* normalization globally across features (and not across the dataset).
    """

    assert scale in ('l1', 'l2')
    n_features = int(np.prod(x.shape))
    mean = torch.mean(x)  # mean over all features (pixels) per sample
    x -= mean
    if scale == 'l1':
        x_scale = torch.mean(torch.abs(x))
    if scale == 'l2':
        x_scale = torch.sqrt(torch.sum(x ** 2)) / n_features
    x /= x_scale
    return x

def get_target_label_idx(labels, targets):
    """
    Get the indices of labels that are included in targets.
    :param labels: array of labels
    :param targets: list/tuple of target labels
    :return: list with indices of target labels
    """
    return np.argwhere(np.isin(labels, targets)).flatten().tolist()


def generate_downsampled_indices(dataset, normal_class, down_sample_rate = 0.1):
    targets = dataset.targets
    idx = np.arange(len(targets))
    # Get indices to keep
    idx_to_keep = targets[idx]== normal_class
    down_sampled_idx = targets[idx] != normal_class
    # Nomial idex consists only with 1 label
    # Abnormal idx consists of all other labels
    nomial_idx = idx[idx_to_keep]
    abnormal_idx = idx[down_sampled_idx]
    m = nomial_idx.shape[0]
    np.random.seed(4321)
    abnormal_idx = np.random.choice(abnormal_idx, size= int(down_sample_rate * m), replace = False)
    overall_idx  = np.append(nomial_idx, abnormal_idx, axis = 0)
    random.seed(4321)
    random.shuffle(overall_idx)
    return overall_idx

def generate_disjoint_dataset(normal_class, data_dir= 'dataset', flatten = True, GCN = False):  
    transform_lst = [transforms.ToTensor()]
    if GCN:
        transform_lst.extend([transforms.Lambda(lambda x: global_contrast_normalization(x, scale='l1')),
                                        transforms.Normalize([mnist_min_max[normal_class][0]],
                                                            [mnist_min_max[normal_class][1] - mnist_min_max[normal_class][0]])])
    if flatten:
        transform_lst.extend([lambda x: x.numpy().flatten()])
    transform = transforms.Compose(transform_lst)
    train_set = torchvision.datasets.MNIST(data_dir, train=True, download=True, transform = transform)
    test_set = torchvision.datasets.MNIST(data_dir, train=False, download=True, transform = transform)
    
    #Subset the training data
    idx = get_target_label_idx(train_set.targets.clone().data.cpu().numpy(), normal_class)
    #Subset of the sample
    train_set.data = train_set.data[idx]
    train_set.targets = train_set.targets[idx]  
    
    train_set = relabel_dataset(normal_class, train_set)
    test_set = relabel_dataset(normal_class, test_set)
    
    return train_set, test_set
    
    

def generate_transductive_dataset(normal_class, data_dir= 'dataset', flatten = True, GCN = False):  
    transform_lst = [transforms.ToTensor()]
    if GCN:
        transform_lst.extend([transforms.Lambda(lambda x: global_contrast_normalization(x, scale='l1')),
                                        transforms.Normalize([mnist_min_max[normal_class][0]],
                                                            [mnist_min_max[normal_class][1] - mnist_min_max[normal_class][0]])])
    if flatten:
        transform_lst.extend([lambda x: x.numpy().flatten()])
    transform = transforms.Compose(transform_lst)
    dataset = torchvision.datasets.MNIST(data_dir, train=True, download=True, transform = transform)
    
    #Downsample the dataset with the normal class
    idx = generate_downsampled_indices(dataset, normal_class)
    
    #Subset of the sample
    dataset.data = dataset.data[idx]
    dataset.targets = dataset.targets[idx]
    
    #Replacing the label, normal class = 0, abnormal class = 1
    return relabel_dataset(normal_class, dataset)


def relabel_dataset(normal_class, dataset):
    for i in range(len(dataset)):
        if dataset.targets[i] == normal_class:
            dataset.targets[i] = 0
        else:
            dataset.targets[i] = 1
    return dataset


def generate_numpy_data(dataset):    
    X = []
    y = []
    for i in range(len(dataset)):
        X.append(dataset[i][0])
        y.append(dataset[i][1])
    return np.array(X), np.array(y)

