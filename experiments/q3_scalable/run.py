import os
import sys

script_dir = os.path.dirname(__file__)
modules_dir = os.path.join(script_dir, '..')
sys.path.append(modules_dir)

import datasets_list

import pandas as pd
import numpy as np
from sklearn.linear_model import LinearRegression
from tqdm import tqdm

competitors = [
    ["competitors/d.mca_q3.sh", "D.MCA"],
    ["competitors/gen2out_q3.sh", "Gen2Out"],
]
mccatch_scalable = ("code/vectorial/mccatch_q3.sh", "McCatch")
     
file_scalability = open('experiments/q3_scalable/result.log', 'w')
     
for dataset, separator, features, cardinality, name in tqdm(datasets_list.datasets_vectorial_scalability):
    if name == 'Satellite' or name == 'Http' or name == 'Speech' or (name.__contains__('I. Axiom') or name.__contains__('C. Axiom')):
        results_path = 'competitors/results/'
        cmd = 'rm -fr ' + results_path + '*'
        os.system(cmd)

        file_scalability.write('[%s]\n' % name)
        file_scalability.flush()

        for script, algorithm in competitors:
            cmd = 'sh %s %s %d' % (script, dataset, features)
            print(cmd)
            os.system(cmd)

            time = np.mean(pd.read_csv(results_path + algorithm + '.csv', header=None).squeeze().values)

            file_scalability.write('%s processed %d points in %.4f secs.\n' % (algorithm, cardinality, time/1000.))
            file_scalability.flush()

        results_path = 'code/vectorial/results/'
        cmd = 'rm -fr ' + results_path + '*'
        os.system(cmd)

        script, algorithm = mccatch_scalable
        cmd = 'sh %s %s %d' % (script, dataset, features)
        os.system(cmd)

        time = np.mean(pd.read_csv(results_path + algorithm + '.csv', header=None).squeeze().values)

        file_scalability.write('%s processed %d points in %.4f secs.\n' % (algorithm, cardinality, time/1000.))
        file_scalability.flush()

    else:
        for distribution in ['diagonal', 'uniform']:
            results_path = 'code/vectorial/results/'
            cmd = 'rm -fr ' + results_path + '*'
            os.system(cmd)

            file_scalability.write('[%s %s]\n' % (name, distribution.upper()))
            file_scalability.flush()

            times = []
            sizes = []
            script, algorithm = mccatch_scalable
            for i, perc in [('13perc',.125), ('25perc',.25), ('50perc',.5), ('100perc',1)]:
                file_scalability.write('Processing %s (~%d points)\n' % (i, int(cardinality * perc)))
                file_scalability.flush()
                cmd = 'sh %s %s %d' % (script, '%s%s-%s.csv' % (dataset, distribution, i), features)
                os.system(cmd)

                time = np.mean(pd.read_csv(results_path + algorithm + '.csv', header=None).squeeze().values)
                times.append(time)
                sizes.append(cardinality * perc)

            times = np.array(times).reshape(-1, 1)
            sizes = np.array(sizes).reshape(-1, 1)

            model = LinearRegression().fit(np.log(sizes), np.log(times))
            file_scalability.write('McCatch Slope: %.4f\n' % model.coef_)

file_scalability.close()