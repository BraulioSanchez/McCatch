#!/bin/bash

export javaCommand="java -Xmx30G -jar code/vectorial/mccatch/target/mccatch-1.0.jar"
algorithm="outlier.distance.McCatch"
name="McCatch"

[ -d code/vectorial/results ] || mkdir -p code/vectorial/results

$javaCommand \
    -algorithm $algorithm \
    -dbc.in $1 \
    -parser.labelIndices $2 \
    -mccatch.a 15 \
    -mccatch.b 0.1 \
    -mccatch.c 0.1 > $name.log
awk '/^mj/' $name.log >> code/vectorial/results/$name.csv
rm $name.log