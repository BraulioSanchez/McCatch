#!/bin/bash

name="Gen2Out"

[ -d competitors/results ] || mkdir -p competitors/results

file=$1
dataset=$2

python competitors/gen2Out/gen2out.py $file $dataset >> competitors/results/$name.csv