import os
import sys

script_dir = os.path.dirname(__file__)
modules_dir = os.path.join(script_dir, '..')
sys.path.append(modules_dir)

import datasets_list

import pandas as pd
import numpy as np

from tqdm import tqdm

competitors = [
    ["competitors/abod_q1.sh", "ABOD"],
    ["competitors/aloci_q1.sh", "ALOCI"],
    ["competitors/dbout_q1.sh", "DB-Out"],
    ["competitors/d.mca_q1.sh", "D.MCA"],
    ["competitors/fastabod_q1.sh", "FastABOD"],
    ["competitors/gen2out_q1.sh", "Gen2Out"],
    ["competitors/iforest_q1.sh", "iForest"],
    ["competitors/loci_q1.sh", "LOCI"],
    ["competitors/lof_q1.sh", "LOF"],
    ["competitors/odin_q1.sh", "ODIN"],
    ["competitors/rda_q1.sh", "RDA"],
]
     
mccath_accurate_vectorial = ("code/vectorial/mccatch_q1.sh", "McCatch")
     
file_auroc = open('experiments/q1_accurate/result_auroc.csv', 'w')
file_avg_prec = open('experiments/q1_accurate/result_avg_prec.csv', 'w')
file_max_f1 = open('experiments/q1_accurate/result_max_f1.csv', 'w')

output = ''
header = ['Dataset'] + [t[1] for t in competitors]
for t in header:
    output += t + ','
output += '%s\n' % mccath_accurate_vectorial[1]

file_auroc.write(output)
file_auroc.flush()
file_avg_prec.write(output)
file_avg_prec.flush()
file_max_f1.write(output)
file_max_f1.flush()

columns = ['auroc', 'avg_prec', 'max_f1']
     
for dataset, sep, features, cardinality, name in tqdm(datasets_list.datasets_vectorial_benchmark_axioms):
    results_path = 'competitors/results/'
    cmd = 'rm -fr ' + results_path + '*'
    os.system(cmd)

    file_auroc.write(name + ',')
    file_auroc.flush()
    file_avg_prec.write(name + ',')
    file_avg_prec.flush()
    file_max_f1.write(name + ',')
    file_max_f1.flush()

    for script, algorithm in competitors:
        cmd = 'sh %s %s %d %d' % (script, dataset, features, cardinality)
        os.system(cmd)

        df = pd.read_csv(results_path + algorithm + '.csv', header=None)
        df.columns = columns
        auroc = np.round(df['auroc'].squeeze().mean(), 4)
        file_auroc.write(str(auroc) + ',')
        file_auroc.flush()
        avg_prec = np.round(df['avg_prec'].squeeze().mean(), 4)
        file_avg_prec.write(str(avg_prec) + ',')
        file_avg_prec.flush()
        max_f1 = np.round(df['max_f1'].squeeze().mean(), 4)
        file_max_f1.write(str(max_f1) + ',')
        file_max_f1.flush()

    results_path = 'code/vectorial/results/'
    cmd = 'rm -fr ' + results_path + '*'
    os.system(cmd)

    script, algorithm = mccath_accurate_vectorial
    cmd = 'sh %s %s %d' % (script, dataset, features)
    os.system(cmd)

    df = pd.read_csv(results_path + algorithm + '.csv', header=None)
    df.columns = columns
    auroc = np.round(df['auroc'].squeeze().mean(), 4)
    file_auroc.write(str(auroc) + '\n')
    file_auroc.flush()
    avg_prec = np.round(df['avg_prec'].squeeze().mean(), 4)
    file_avg_prec.write(str(avg_prec) + '\n')
    file_avg_prec.flush()
    max_f1 = np.round(df['max_f1'].squeeze().mean(), 4)
    file_max_f1.write(str(max_f1) + '\n')
    file_max_f1.flush()

file_auroc.close()