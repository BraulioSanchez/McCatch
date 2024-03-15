import os
import sys

script_dir = os.path.dirname(__file__)
modules_dir = os.path.join(script_dir, '..')
sys.path.append(modules_dir)

import datasets_list

import pandas as pd
import numpy as np
import json
import matplotlib.pyplot as plt
from tqdm import tqdm

http_satellite_imagery_experiment = ("code/vectorial/mccatch_q4.sh", "McCatch")
last_names_fingerprints_experiment = ("code/metric/mccatch_q4.sh", "McCatch")
skeletons_experiment = ("code/metric/mccatch_q4.2.sh", "McCatch")

file = open('experiments/q4_practical/result.log', 'w')
     
for dataset, sep, features, cardinality, name in tqdm(datasets_list.datasets_metric_vectorial):
    if name == 'Shanghai' or name == 'Volcanoes':
        results_path = 'code/vectorial/results/'
        cmd = 'rm -fr ' + results_path + '*'
        os.system(cmd)

        file.write('[%s]\n' % name)
        file.flush()
        script, algorithm = http_satellite_imagery_experiment
        cmd = 'sh %s %s %d' % (script, dataset, features)
        os.system(cmd)

        data = pd.read_csv(results_path + algorithm + '.csv', names=['microcluster','indice','score'])
        microclusters = data['microcluster'].unique()
        satellite_imagery = pd.read_csv(dataset)
        fig = plt.figure(figsize = (8, 5))
        ax = fig.gca(projection='3d')
        indices = []
        for microcluster in data['microcluster'].unique():
            df = data[data['microcluster'] == microcluster]
            if len(df) > 1:
                mc = satellite_imagery.iloc[df['indice'].values]
                indices += mc.index.values.tolist()
                ax.scatter3D(
                    mc.attr1,
                    mc.attr2,
                    mc.attr3,
                    marker='s',
                    edgecolor='k',
                    linewidths=.8,
                    s=40,
                )
            else:
                mc = satellite_imagery.iloc[df['indice'].values]
                indices += mc.index.values.tolist()
                ax.scatter3D(
                    mc.attr1,
                    mc.attr2,
                    mc.attr3,
                    marker='s',
                    edgecolor='k',
                    linewidths=.6,
                    s=40,
                    color='#ffc128',
                )
        satellite_imagery.drop(index=indices, inplace=True)
        ax.scatter3D(
            satellite_imagery.attr1,
            satellite_imagery.attr2,
            satellite_imagery.attr3,
            marker='o',
            alpha=.6,
            edgecolor='k',
            linewidths=.6,
            s=20,
            color='k',
        )

        ax.set_xlim3d(0, 1)
        ax.set_ylim3d(0, 1)
        ax.set_zlim3d(0, 1)

        ax.set_xlabel('avg. red', fontsize=15)
        ax.set_ylabel('avg. green', fontsize=15)
        ax.set_zlabel('avg. blue', fontsize=15)
        ax.tick_params(axis='both', labelsize=10)

        ax.view_init(20, 300)

        fig.savefig('experiments/q4_practical/results_%s.png' % name,dpi=300)
            
        file.write('results_%s.png created!\n' % (name))
        file.flush()

    if name == 'Last Names' or name == 'Fingerprints':
        results_path = 'code/metric/results/'
        cmd = 'rm -fr ' + results_path + '*'
        os.system(cmd)

        file.write('[%s]\n' % name)
        file.flush()
        script, algorithm = last_names_fingerprints_experiment
        cmd = 'sh %s %s' % (script, dataset)
        os.system(cmd)

        last_names_fingerprints = pd.read_csv('%s%s.csv' % (results_path, algorithm))
        last_names_fingerprints.sort_values(by='score', ascending=False, inplace=True)
        
        file.write(last_names_fingerprints.to_string(index=False) + '\n')
        file.flush()

    if name == 'Skeletons':
        results_path = 'code/metric/results/'
        cmd = 'rm -fr ' + results_path + '*'
        os.system(cmd)

        file.write('[%s]\n' % name)
        file.flush()
        script, algorithm = skeletons_experiment
        cmd = 'sh %s %s' % (script, dataset)
        os.system(cmd)

        skeletons = pd.read_csv('%s%s.csv' % (results_path, algorithm))
        skeletons.sort_values(by='score', ascending=False, inplace=True)
        
        file.write(skeletons.to_string(index=False) + '\n')
        file.flush()

    if name == 'Http':
        results_path = 'code/vectorial/results/'
        cmd = 'rm -fr ' + results_path + '*'
        os.system(cmd)

        file.write('[%s]\n' % name)
        file.flush()
        script, algorithm = http_satellite_imagery_experiment
        cmd = 'sh %s %s %d' % (script, dataset, features)
        os.system(cmd)

        data = pd.read_csv(results_path + algorithm + '.csv', names=['microcluster','indice','score'])
        http = pd.read_csv('%s.origin' % dataset)
        fig = plt.figure(figsize = (8, 5))
        ax = fig.gca(projection='3d')
        for microcluster in data['microcluster'].unique():
            df = data[data['microcluster'] == microcluster]
            print(len(df))
            if len(df) == 29:
                mc = http.iloc[df['indice'].values]
                ax.scatter3D(
                    mc.duration,
                    mc.src_bytes,
                    mc.dst_bytes,
                    marker='X',
                    alpha=.7,
                    edgecolor='k',
                    linewidths=.6,
                    s=40,
                    color='#f10102',
                )
            else:
                mc = http.iloc[df['indice'].values]
                for _, member in mc.iterrows():
                    marker = '^' if int(member._class) == 4 else 'X'
                    ax.scatter3D(
                        member.duration,
                        member.src_bytes,
                        member.dst_bytes,
                        marker=marker,
                        alpha=.7,
                        edgecolor='k',
                        linewidths=.6,
                        s=40,
                        color='#ffc128',
                    )

        ax.set_xlim3d(0, 1)
        ax.set_ylim3d(0, 1)
        ax.set_zlim3d(0, 1)

        ax.set_xlabel('duration', fontsize=15)
        ax.set_ylabel('source bytes', fontsize=15)
        ax.set_zlabel('destionation bytes', fontsize=15)
        ax.tick_params(axis='both', labelsize=13)

        ax.view_init(20, 30)

        fig.savefig('experiments/q4_practical/results_%s.png' % name,dpi=300)
            
        file.write('results_%s.png created!\n' % (name))
        file.flush()

file.close()