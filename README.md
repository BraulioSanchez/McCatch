# McCatch: *Scalable Microcluster Detection in Dimensional and Nondimensional Datasets*

by
Braulio V. Sánchez Vinces, Robson L. F. Cordeiro, and Christos Faloutsos

# To Cite
    @INPROCEEDINGS{MCCATCH2024,
      author={Braulio V. Sánchez Vinces, Robson L. F. Cordeiro, and Christos Faloutsos},
      title={McCatch: Scalable Microcluster Detection in Dimensional and Nondimensional Datasets},
      booktitle={2024 IEEE 40th International Conference on Data Engineering (ICDE)},
      year={2024},
      organization={IEEE}
    }

# Abstract

> How could we have an outlier detector that works even with *nondimensional* data, and ranks *together* both singleton microclusters (‘one-off’ outliers) and nonsingleton microclusters by their anomaly scores? How to obtain scores that are *principled* in one *scalable* and *‘hands-off’* manner? Microclusters of outliers indicate coalition or repetition in fraud activities, etc.; their identification is thus highly desirable. This paper presents McCATCH: a new algorithm that detects microclusters by leveraging our proposed ‘Oracle’ plot (1NN Distance versus Group 1NN Distance). We study 31 real and synthetic datasets with up to 1M data elements to show that McCATCH is the only method that answers both of the questions above; and, it outperforms 11 other methods, especially when the data has non-singleton microclusters or is nondimensional. We also showcase McCATCH’s ability to detect meaningful microclusters in graphs, fingerprints, logs of network connections, text data, and satellite imagery. For example, it found a 30-elements microcluster of confirmed ‘Denial of Service’ attacks in the network logs, taking only *∼ 3 minutes* for 222K data elements on a stock desktop.

# Main Sections
1. [Directory Tree]
2. [McCatch Usage]
3. [McCatch Competitors]
4. [Experiments]

## Directory Tree

A summary of the file structure can be found in the following directory tree.

```bash
McCatch
├───code
│   ├───metric \\ Slim-tree-based implementation (C++)
│   │   ├───mccatch
│   │   │   ├───sample \\ McCatch instantiation of Slim-tree and overall code
│   │   │   ├───src \\ Slim-tree main code
│   ├───vectorial \\ R*-tree-based implementation (Java)
│   │   ├───mccatch \\ McCatch instantiation of R*-tree and overall code
├───competitors \\ Source code of all competitors
├───datasets
│   ├───metric \\ Last Names, Fingerprints and Skeletons datasets
│   ├───vectorial \\  Benchmark datasets, satellite imagery of Volcanoes and Shanghai, Scalability datasets, and Axioms datasets
├───experiments
│   ├───q1_accurate
│   ├───q2_principled
│   ├───q3_scalable
│   ├───q4_practical
│   ├───q5_hands_off
```

## McCatch Usage

McCatch and its competitors use a number of open source tools to work properly. The following packages should be installed before starting

#### Related to Java

- jre8+ - Java Runtime Environment version 8 or more.
- maven - Software project management and comprehension tool.

#### Related to C++

- make - Unix tool for running Makefiles.
- gcc/g++ - Compiler for C++ code.

#### Related to Python

- python - Python interpreter in version 3.7 or above.

### Building McCatch

Run with no options:

```sh
sh code/build.sh
```

### Running McCatch

For Java implementation, run:

```sh
java -jar code/vectorial/mccatch/target/mccatch-1.0.jar -algorithm elki.outlier.distance.McCatch -time -dbc.in <dataset> -parser.labelIndices <num_features> -mccatch.a <a> -mccatch.b <b> -mccatch.c <c>
```

For C++ implementation, run:

```sh
code/metric/mccatch/sample/mccatch <dataset> <a> <b> <c>
```

## McCatch Competitors

All competitors are publicly available, below are the source code download links:

- ABOD, ALOCI, DB-Out, FastABOD, iForest, LOCI, LOF, ODIN - <https://repo1.maven.org/maven2/io/github/elki-project/elki/0.8.0/elki-0.8.0.jar>
- D.MCA - <https://github.com/11hifish/D.MCA>
- Gen2Out - <https://github.com/mengchillee/gen2Out>
- RDA - <https://github.com/zc8340311/RobustAutoencoder>

## Experiments

Before running the experiments, we recommend using [Anaconda](https://docs.anaconda.com/anaconda/install/) to create the environment with all the necessary packages with the following command:

```sh
conda env create -f experiments/environment.yml
conda activate mccatch_env
```

To recreate the axiom samples and datasets for the scalability experiments,

```sh
python datasets/vectorial/run.py
```

To evaluate how accurate is McCatch (**Q1**),

```sh
python experiments/q1_accurate/run.py
```

To evaluate if McCatch obey the axioms (**Q2**),

```sh
python experiments/q2_principled/run.py
```

To evaluate how fast is McCatch (**Q3**),

```sh
python experiments/q3_scalable/run.py
```

To evaluate how well McCatch works on real data (**Q4**),

```sh
python experiments/q4_practical/run.py
```

To evaluate the sensitivity to McCatch hyperparameters in all datasets (**Q5**),
```sh
python experiments/q5_hands_off/run.py
```

_This software was designed in Unix-like systems, it is not yet fully tested in other OS._