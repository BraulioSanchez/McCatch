import os
import sys

script_dir = os.path.dirname(__file__)
modules_dir = os.path.join(script_dir, '..')
sys.path.append(modules_dir)

import datasets_list

import pandas as pd
import numpy as np
from sklearn import metrics
from tqdm import tqdm

mccath_vectorial = ("code/vectorial/mccatch_q5.sh", "McCatch")
mccath_metric = ("code/metric/mccatch_q5.sh", "McCatch")
     
file = open('experiments/q5_hands_off/result.csv', 'w')

output = 'Dataset,a,b,c,AUROC\n'
file.write(output)
file.flush()

_as = [13, 14, 15, 16, 17]
_bs = [0.08, 0.09, 0.1, 0.11, 0.12]
_cs = [0.08, 0.09, 0.1, 0.11, 0.12]

for dataset, sep, _, cardinality, name in tqdm(datasets_list.datasets_metric_vectorial[2:5]):
    results_path = 'code/metric/results/'
    cmd = 'rm -fr ' + results_path + '*'
    os.system(cmd)

    script, algorithm = mccath_metric
    for a in _as:
        cmd = 'sh %s %s %d %.4f %.4f "%s"' % (script, dataset, a, 0.1, 0.1, name)
        os.system(cmd)

        result = pd.read_csv('%s%s.csv' % (results_path, algorithm))
        y_true = result["gt"].squeeze().values; y_score = result["score"].squeeze().values

        output = '%s,%d,%.4f,%.4f,%.12f\n' % (name, a, 0.1, 0.1, metrics.roc_auc_score(y_true, y_score))
        file.write(output)
        file.flush()

        cmd = 'rm -fr ' + results_path + '*'
        os.system(cmd)

    for b in _bs:
        cmd = 'sh %s %s %d %.4f %.4f "%s"' % (script, dataset, 15, b, 0.1, name)
        os.system(cmd)

        result = pd.read_csv('%s%s.csv' % (results_path, algorithm))
        y_true = result["gt"].squeeze().values; y_score = result["score"].squeeze().values

        output = '%s,%d,%.4f,%.4f,%.12f\n' % (name, 15, b, 0.1, metrics.roc_auc_score(y_true, y_score))
        file.write(output)
        file.flush()

        cmd = 'rm -fr ' + results_path + '*'
        os.system(cmd)

    for c in _cs:
        cmd = 'sh %s %s %d %.4f %.4f "%s"' % (script, dataset, 15, 0.1, c, name)
        os.system(cmd)

        result = pd.read_csv('%s%s.csv' % (results_path, algorithm))
        y_true = result["gt"].squeeze().values; y_score = result["score"].squeeze().values

        output = '%s,%d,%.4f,%.4f,%.12f\n' % (name, 15, 0.1, c, metrics.roc_auc_score(y_true, y_score))
        file.write(output)
        file.flush()

        cmd = 'rm -fr ' + results_path + '*'
        os.system(cmd)

for dataset, sep, features, cardinality, name in tqdm(datasets_list.datasets_vectorial_benchmark_axioms):
    results_path = 'code/vectorial/results/'
    cmd = 'rm -fr ' + results_path + '*'
    os.system(cmd)

    script, algorithm = mccath_vectorial
    for a in _as:
        cmd = 'sh %s %s %d %d %.4f %.4f' % (script, dataset, features, a, 0.1, 0.1)
        os.system(cmd)

    for b in _bs:
        cmd = 'sh %s %s %d %d %.4f %.4f' % (script, dataset, features, 15, b, 0.1)
        os.system(cmd)

    for c in _cs:
        cmd = 'sh %s %s %d %d %.4f %.4f' % (script, dataset, features, 15, 0.1, c)
        os.system(cmd)

    cmd = 'cat %s%s.csv >> experiments/q5_hands_off/result.csv' % (results_path, algorithm)
    os.system(cmd)

    cmd = 'sh %s %s %d' % (script, dataset, features)
    os.system(cmd)

    auroc = np.round(pd.read_csv(results_path + algorithm + '.csv', header=None).squeeze().mean(), 4)
    file.write(str(auroc) + '\n')
    file.flush()

    cmd = 'rm -fr ' + results_path + '*'
    os.system(cmd)

file.close()