from sklearn import metrics
import numpy as np

def max_f1_score(y_true, y_pred):
	# Explore different thresholds
	thresholds = np.arange(.1,1.,.1)
	f1_scores = []

	for threshold in thresholds:
		y_pred_binary = y_pred > threshold  # Apply threshold
		f1 = metrics.f1_score(y_true, y_pred_binary)
		f1_scores.append(f1)

	# Find the best threshold and F1-score
	# best_threshold = thresholds[np.argmax(f1_scores)]
	return np.max(f1_scores)

####################################
# Author: Shuli Jiang              #
# Email	: shulij@andrew.cmu.edu    #
####################################

# Experiments running DMCA / DMCA_0.

import argparse
import os
import numpy as np
from src.utils import load_dataset, convert_to_binary_label
from src.DMCA import DMCA_0, DMCA
from scipy.io import savemat
from src.find_clusters import determine_threshold, find_clusters
from experiment.hyperparameters import get_hyperparameter

import pandas as pd

import functools

unbuffered_print = functools.partial(print, flush=True)


parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('--method', type=str, default='DMCA',
                    help='Outlier detection method: {DMCA, or DMCA_0}')
parser.add_argument('--dataset', type=str, default='synthetic10',
                    help='Dataset')
parser.add_argument('--file', type=str, default='synthetic10',
                    help='File')
parser.add_argument('--num-exp', type=int, default=5,
                    help='# random runs (only for non-deterministic methods)')
parser.add_argument('--save-path', type=str, default=None,
                    help='locations to save the results')
args = parser.parse_args()


if args.save_path is not None and not os.path.isdir(args.save_path):
    os.mkdir(args.save_path)

# prepare dataset
# X, y = load_dataset(args.dataset)
data = pd.read_csv(args.dataset, comment='#')
X = data.iloc[:,:-1].values
y = data.iloc[:,-1].values
bin_y = convert_to_binary_label(y)

# hyperparameters
psi_list, k_list = get_hyperparameter(args.method, X.shape[0])
# t = 100

# start training
all_auc = np.zeros(args.num_exp)

for t in [2**i for i in range(1,8)]:
    for exp_idx in range(args.num_exp):
        param_auc = np.zeros(len(psi_list))
        avg_prec = np.zeros(len(psi_list))
        max_f1 = np.zeros(len(psi_list))
        for i, psi_ in enumerate(psi_list):
            if args.method == 'DMCA':
                ano_score, neighbor_recorder = DMCA(X=X, t=t, psi=psi_)
            elif args.method == 'DMCA_0':
                ano_score, neighbor_recorder = DMCA_0(X=X, t=t, psi=psi_)
            else:
                raise Exception('Which method ???')
            param_auc[i] = metrics.roc_auc_score(bin_y, ano_score)
            avg_prec[i] = metrics.average_precision_score(bin_y, ano_score)
            max_f1[i] = max_f1_score(bin_y, ano_score)
            # threshold = determine_threshold(neighbor_recorder)
            # groups = find_clusters(neighbor_recorder, True, threshold)
        unbuffered_print('%.12f,%.12f,%.12f' % (np.mean(param_auc), np.mean(avg_prec), np.mean(max_f1)))