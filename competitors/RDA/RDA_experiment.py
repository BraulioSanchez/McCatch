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

import numpy as np
import pandas as pd
from model.DeepAE import AEClassifier
from model.RobustAE import RobustAutoencoder as RAE
from test_tube import HyperOptArgumentParser

import functools

unbuffered_print = functools.partial(print, flush=True)

def str2bool(v):
    if isinstance(v, bool):
        return v
    if v.lower() in ('yes', 'true', 't', 'y', '1'):
        return True
    elif v.lower() in ('no', 'false', 'f', 'n', '0'):
        return False
    else:
        print('Boolean value expected.')
        
parser = HyperOptArgumentParser(strategy='grid_search')

parser.add_argument('--data', default='MNIST', help='currently support MNIST only')
parser.add_argument('--file', default='file', help='file')
parser.add_argument('--batch', type=int, default=32, help='batch size')
parser.add_argument('--normal_class', type = int, default = 0)
parser.add_argument('--transductive', type=str2bool, default = True)
parser.add_argument('--device', type = str, default = 'cpu')
parser.add_argument('--num_exp', type = int, default = 10)

args=parser.parse_args()

#LOAD TRAIN, TEST DATA
data = pd.read_csv(args.data, comment='#')
train_X = data.iloc[:,:-1].values.astype(np.float32)
train_y = data.iloc[:,-1].values.astype(np.float32)

# add PROGRAM level args for hyperparameter grid search
parser = HyperOptArgumentParser(strategy='grid_search')
parser.opt_list('--n_layer', default = 3, type = int, tunable = True, options = [2, 3, 4])
parser.opt_list('--lambda_', default = 5e1, type = float, tunable = True, options = [10e-5, 7.5e-5, 10e-4])
parser.opt_list('--symmetry', default = True, type = bool, tunable = True, options = [True])
parser.opt_list('--input_dim_decay', default = 2, type = int, tunable = True, options = [1, 2, 4])
parser.opt_list('--learning_rate', default = 0.001, type = float, tunable = True, options = [1e-3])
parser.opt_list('--inner_iteration', default = 20, type = int, tunable = True, options = [20, 50])
parser.opt_list('--iteration', default = 20, type = int, tunable = True, options = [20, 50])
parser.opt_list('--regularization', default = "l1", type = str, tunable = True, options = ["l1"])

model_hparams = parser.parse_args('')

initial_input_dim = train_X.shape[1]

for _ in range(args.num_exp):
    for hparam in model_hparams.trials(108):
        #set the autoencoder layers
        input_dim_list = []
        for i in range(hparam.n_layer):
            input_dim_list.append(int(initial_input_dim / (hparam.input_dim_decay**i)))
        
        #build the RAE model
        model = RAE(input_dim_list, lambda_=hparam.lambda_, symmetry=hparam.symmetry,
                                    learning_rate=hparam.learning_rate, inner_iteration=hparam.inner_iteration,
                                    iteration=hparam.iteration, batch_size=args.batch,
                                    regularization=hparam.regularization, decay=hparam.input_dim_decay,
                                    device=args.device, transductive=args.transductive)

        predict_y = model.fit(train_X)

        auroc = metrics.roc_auc_score(train_y, predict_y)
        avg_prec = metrics.average_precision_score(train_y, predict_y)
        max_f1 = max_f1_score(train_y, predict_y)
        unbuffered_print("%.12f,%.12f,%.12f" % (auroc, avg_prec, max_f1))
