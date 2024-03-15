#!/bin/bash

name="D.MCA"

[ -d competitors/results ] || mkdir -p competitors/results

pip install -e competitors/$name/

file=$1
dataset=$2

python competitors/$name/experiment/exp_DMCA.py --method DMCA --dataset $file --num-exp 10 --file $dataset >> competitors/results/$name.csv