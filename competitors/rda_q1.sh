#!/bin/bash

name="RDA"

[ -d competitors/results ] || mkdir -p competitors/results
# rm -f -- competitors/results/$name.csv

file=$1
labelIndices=$2

python competitors/RDA/RDA_experiment.py --data $1 --file $2 >> competitors/results/$name.csv