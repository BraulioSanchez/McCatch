import os
import pandas as pd
import numpy as np
from tqdm import tqdm

path = 'datasets/vectorial/axioms'
shapes = ['arc', 'cross', 'gaussian']
axioms = ['cardinality', 'isolation']
sample_size = 5000

for shape in tqdm(shapes, desc='Generating axioms samples'):
    for axiom in axioms:
        dataset = pd.read_csv('%s/%s_%s_axiom.csv' % (path, shape, axiom))

        sample_path = '%s/%s_%s_samples' % (path, shape, axiom)
        os.makedirs(sample_path)
        for sample in range(50):
            dataset.sample(n=sample_size).to_csv('%s/sample%d.csv' % (sample_path, sample), index=False)

###################################################################

def generate_uniform_dataset(dimension=2, size=1000000):
    lower_bound = 0  # Adjust this value as needed
    upper_bound = 1  # Adjust this value as needed

    data = np.random.uniform(low=lower_bound, high=upper_bound, size=(size, dimension))
    dataset = pd.DataFrame(data)
    columns = ['attr'+str(i) for i in range(1,dimension+1)]
    dataset.columns = columns
    label = np.random.choice([0, 1], size=size, p=[0.5, 0.5])
    dataset['label'] = label
    return dataset

def generate_diagonal_dataset(dimension=2, size=1000000):
    lower_bound = 0  # Adjust this value as needed
    upper_bound = 1  # Adjust this value as needed

    data = np.random.uniform(low=lower_bound, high=upper_bound, size=size)
    data = [[i]*dimension for i in data]
    dataset = pd.DataFrame(data)
    columns = ['attr'+str(i) for i in range(1,dimension+1)]
    dataset.columns = columns
    label = np.random.choice([0, 1], size=size, p=[0.5, 0.5])
    dataset['label'] = label
    return dataset

path = 'datasets/vectorial/scalability'
dimensions = [2, 4, 20, 50]

for dimension in tqdm(dimensions, desc='Generating scalability datasets'):
    dataset_path = '%s/%dd' % (path, dimension)
    os.makedirs(dataset_path, exist_ok=True)

    dataset = generate_uniform_dataset(dimension)
    dataset.to_csv('%s/%dduniform-100perc.csv' % (dataset_path, dimension), index=False)
    dataset.sample(n=500000).to_csv('%s/%dduniform-50perc.csv' % (dataset_path, dimension), index=False)
    dataset.sample(n=250000).to_csv('%s/%dduniform-25perc.csv' % (dataset_path, dimension), index=False)
    dataset.sample(n=125000).to_csv('%s/%dduniform-13perc.csv' % (dataset_path, dimension), index=False)

    dataset = generate_diagonal_dataset(dimension)
    dataset.to_csv('%s/%dddiagonal-100perc.csv' % (dataset_path, dimension), index=False)
    dataset.sample(n=500000).to_csv('%s/%dddiagonal-50perc.csv' % (dataset_path, dimension), index=False)
    dataset.sample(n=250000).to_csv('%s/%dddiagonal-25perc.csv' % (dataset_path, dimension), index=False)
    dataset.sample(n=125000).to_csv('%s/%dddiagonal-13perc.csv' % (dataset_path, dimension), index=False)            