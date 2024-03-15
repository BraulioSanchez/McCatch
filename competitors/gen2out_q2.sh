#!/bin/bash

name="Gen2Out"

[ -d competitors/results ] || mkdir -p competitors/results

python competitors/gen2Out/gen2out_q2.py $1 > $name.log
awk '/mj/' $name.log >> competitors/results/$name.csv
rm $name.log