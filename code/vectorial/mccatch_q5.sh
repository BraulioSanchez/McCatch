#!/bin/bash

export javaCommand="java -Xmx30G -jar code/vectorial/mccatch/target/mccatch-1.0.jar"
algorithm="outlier.distance.McCatch"
name="McCatch"

[ -d code/vectorial/results ] || mkdir -p code/vectorial/results

$javaCommand \
-algorithm $algorithm \
-time \
-dbc.in $1 \
-parser.labelIndices $2 \
-mccatch.a $3 \
-mccatch.b $4 \
-mccatch.c $5 \
-mccatch.output 2 > $name.log
filename=$(basename $1)
dataset="${filename%_preprocessed.csv}"
dataset="${dataset%.csv}"
auroc=$(awk '/measures AUROC /{print $0}' $name.log | awk '{split($0,array," "); print array[4]}')
echo $dataset,$3,$4,$5,$auroc >> code/vectorial/results/$name.csv
rm $name.log