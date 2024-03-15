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
# Author: Jeremy (Meng-Chieh) Lee  #
# Email	: mengchil@cs.cmu.edu      #
####################################

from cProfile import label
import numpy as np
from scipy.spatial.distance import cityblock

from sklearn.cluster import DBSCAN
from sklearn.linear_model import LinearRegression

import time
from joblib import Parallel, delayed
from tqdm import tqdm

from iforest import IsolationForest
# from utils import sythetic_group_anomaly

import pandas as pd

class gen2Out:
	def __init__(self, lower_bound=9, higher_bound=12, max_depth=7,
				 rotate=True, contamination='auto', random_state=None,
				 n_estimators=100):
		self.lower_bound = lower_bound
		self.higher_bound = higher_bound
		self.max_depth = max_depth
		self.rotate = rotate
		self.contamination = contamination
		self.random_state = random_state

		self.n_estimators = n_estimators

	def func(self, Xs, i):
		### Fit forest with full-grown trees
		clf = IsolationForest(random_state=self.random_state,
							  max_samples=len(Xs),
							  contamination=self.contamination,
							  rotate=self.rotate,
							  n_estimators=self.n_estimators).fit(Xs, max_depth=100000000)
		depths = np.mean(clf._compute_actual_depth(Xs), axis=0)
		bins = np.arange(int(depths.min()), int(depths.max() + 2))
		y, x = np.histogram(depths, bins=bins)
		return i, x[np.argmax(y)]

	def fit(self, X):
		if self.random_state:
			np.random.seed(self.random_state)
		self.n_sample = X.shape[0]

		params_arr = Parallel(n_jobs=self.higher_bound-self.lower_bound)(
								[delayed(self.func)(X[np.random.choice(self.n_sample, 2 ** i, replace=True)], i)
								for i in np.arange(self.lower_bound, self.higher_bound)])
		# params_arr = []
		# for i in np.arange(self.lower_bound, self.higher_bound):
		# 	params_arr.append(self.func(X[np.random.choice(self.n_sample, 2 ** i, replace=True)], i))
		x_arr, y_arr = np.array(params_arr).T

		self.reg = LinearRegression(fit_intercept=False).fit(x_arr.reshape(-1, 1), y_arr)
		self.clf = IsolationForest(random_state=self.random_state,
								   max_samples=len(X),
								   contamination=self.contamination,
								   rotate=self.rotate,
								   n_estimators=self.n_estimators).fit(X, max_depth=self.max_depth)

		return self

	def average_path_length(self, n):
		n = np.array(n)
		apl = self.reg.predict(np.log2([n]).T)
		apl[apl < 0] = 0
		return apl

	def decision_function(self, X):
		depths, leaves = self.clf._compute_actual_depth_leaf(X)

		new_depths = np.zeros(X.shape[0])
		for d, l in zip(depths, leaves):
			new_depths += d + self.average_path_length(l)

		scores = 2 ** (-new_depths
					   / (len(self.clf.estimators_)
						  * self.average_path_length([self.n_sample])))
		
		return scores

	def point_anomaly_scores(self, X):
		self = self.fit(X)
		return self.decision_function(X)
	
	def group_anomaly_scores(self, X, trials=10):
		### Fit a sequence of gen2Out0
		min_rate = int(np.log2(len(X)) - 8) + 1
		# print(min_rate)
		scores = np.zeros((min_rate, trials, len(X)))
		# print(scores.shape)

		for i in tqdm(range(min_rate)):
			for j in range(trials):
				X_sampled = X[np.random.choice(len(X), int(len(X) * (1 / (2 ** i))))]
				clf = self.fit(X_sampled)
				scores[i][j] = clf.decision_function(X)

		### Create X-ray plot
		smax = np.max(np.mean(scores, axis=1), axis=0)
		threshold = np.mean(smax) + 3 * np.std(smax)

		sr_list = []
		xrays = np.max(np.mean(scores, axis=1), axis=0)
		# print(xrays)
		for idx, xray in enumerate(tqdm(xrays)):
			if xray >= threshold:
				sr_list.append(idx)
		sr_list = np.array(sr_list)
		# print(sr_list)

		### Outlier grouping
		groups = DBSCAN().fit_predict(X[sr_list])
		# print(groups)

		labels = -np.ones(len(X)).astype(int)
		for idx, g in zip(sr_list, groups):
			if g != -1: # IGNORE NOISE POINTS AND INLIERS
				labels[idx] = g + 1

		### Compute iso-curves
		xline = 1 / (2 ** np.arange(0, min_rate))
		s_arr = [[] for l in np.unique(labels) if l != -1] # IGNORE NOISE POINTS AND INLIERS
		xrays_max = np.argmax(np.mean(scores, axis=1), axis=0)
		for idx in sr_list:
			if labels[idx] != -1:
				dis = cityblock([np.log2(xrays_max[idx]) / 10 + 1, xrays[idx]], [1, 1])
				s_arr[labels[idx]-1].append((2 - dis) / 2)
		# print(s_arr)
		
		ga_scores = np.array([np.median(s) for s in s_arr])
		ga_indices = np.array([np.where(labels == l)[0] for l in np.unique(labels) if l != -1]) # IGNORE NOISE POINTS AND INLIERS
		
		return ga_scores, ga_indices


if __name__ == '__main__':
	import sys
	import functools

	unbuffered_print = functools.partial(print, flush=True)

	input = pd.read_csv(sys.argv[1]).squeeze().values
	X = input[:,:-1]
	label = input[:,-1]

	for _ in range(10):
		for n_estimators in [2**i for i in range(1,8)]:
			for max_depth in [2, 3]:
					pscores = gen2Out(
						lower_bound=1,
						higher_bound=2,
						max_depth=max_depth,
						rotate=False,
						n_estimators=n_estimators
						).point_anomaly_scores(X)
					
					auc = metrics.roc_auc_score(label, pscores)
					avg_prec = metrics.average_precision_score(label, pscores)
					max_f1 = max_f1_score(label, pscores)
					unbuffered_print('%.12f,%.12f,%.12f' % (auc, avg_prec, max_f1))


