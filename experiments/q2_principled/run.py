import os
import sys

script_dir = os.path.dirname(__file__)
modules_dir = os.path.join(script_dir, '..')
sys.path.append(modules_dir)

import datasets_list

import pandas as pd
import numpy as np
import scipy.stats as stats
from tqdm import tqdm

competitors = [
    ["competitors/gen2out_q2.sh", "Gen2Out"],
]
     
mccath_principled = ("code/vectorial/mccatch_q2.sh", "McCatch")
     
file = open('experiments/q2_principled/result.log', 'w')
     
for dataset, separator, features, name, indices in tqdm(datasets_list.datasets_vectorial_axioms_samples):
    mc1, mc2 = indices
    mc1 = set(mc1); mc2 = set(mc2)
    
    file.write('[%s]\n' % name)
    file.flush()

    # Competitors
    results_path = 'competitors/results/'
    cmd = 'rm -fr ' + results_path + '*'
    os.system(cmd)

    for script, algorithm in competitors:
        scores = []
        for i in tqdm(range(50)):
            cmd = 'sh %s %ssample%d.csv' % (script, dataset, i)
            os.system(cmd)

            result = pd.read_csv(results_path + algorithm + '.csv', header=None)
            result.columns = ["mc", "indice", "score"]
            result["indice"] = result["indice"].astype("int")

            hits = 0
            mcs = result["mc"].unique()
            for mc in mcs:
                mj = result.query("mc=='%s'" % mc)[["indice", "score"]]

                mj = set(mj["indice"].values)
                intersection = mc1.intersection(mj)
                if len(intersection) > int(len(mc1)*0.5) and len(mj) <= len(mc1):
                    minor_score = result[result["indice"] == intersection.pop()]["score"].squeeze()
                    hits += 1
                    continue

                intersection = mc2.intersection(mj)
                if len(intersection) > int(len(mc2)*0.5) and len(mj) <= len(mc2):
                    major_score = result[result["indice"] == intersection.pop()]["score"].squeeze()
                    hits += 1
                    continue
            if hits == 2:
                scores.append([major_score, minor_score])

            cmd = 'rm -fr ' + results_path + '*'
            os.system(cmd)
        scores = np.array(scores)

        if len(scores) > 25:
            file.write('%s - %s, %d datasets ignored\n' % (algorithm, str(stats.ttest_ind(a=scores[:, 0], b=scores[:, 1], equal_var=True)), 50 - len(scores)))
            file.flush()
        else:
            file.write('%s, %d datasets ignored\n' % (algorithm, 50 - len(scores)))
            file.flush()

    # McCatch
    results_path = 'code/vectorial/results/'
    cmd = 'rm -fr ' + results_path + '*'
    os.system(cmd)

    script, algorithm = mccath_principled
    scores = []
    for i in tqdm(range(50)):
        cmd = 'sh %s %ssample%d.csv %d' % (script, dataset, i, features)
        os.system(cmd)

        result = pd.read_csv(results_path + algorithm + '.csv', header=None)
        result.columns = ["mc", "indice", "score"]
        result["indice"] = result["indice"].astype("int")

        hits = 0
        mcs = result["mc"].unique()
        for mc in mcs:
            mj = result.query("mc=='%s'" % mc)[["indice", "score"]]

            mj = set(mj["indice"].values)
            intersection = mc1.intersection(mj)
            if len(intersection) > int(len(mc1)*0.5) and len(mj) <= len(mc1):
                minor_score = result[result["indice"] == intersection.pop()]["score"].squeeze()
                hits += 1
                continue

            intersection = mc2.intersection(mj)
            if (len(intersection) > int(len(mc2)*0.5)) and len(mj) <= len(mc2):
                major_score = result[result["indice"] == intersection.pop()]["score"].squeeze()
                hits += 1
                continue
        if hits == 2:
            scores.append([major_score, minor_score])

        cmd = 'rm -fr ' + results_path + '*'
        os.system(cmd)
    scores = np.array(scores)

    if len(scores) > 25:
        file.write('%s - %s, %d ignored datasets\n' % (algorithm, str(stats.ttest_ind(a=scores[:, 0], b=scores[:, 1], equal_var=True)), 50 - len(scores)))
        file.flush()
    else:
        file.write('%s, %d ignored datasets\n' % (algorithm, 50 - len(scores)))
        file.flush()

file.close()