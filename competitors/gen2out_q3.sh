#!/bin/bash

name="Gen2Out"

[ -d competitors/results ] || mkdir -p competitors/results

file=$1
dataset=$2

for _ in {1..10}
do
    python competitors/gen2Out/gen2out_q3.py $file $dataset >> competitors/results/$name.csv
done