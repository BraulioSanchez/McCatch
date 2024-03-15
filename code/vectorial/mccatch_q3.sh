#!/bin/bash

export javaCommand="java -Xmx30G -jar code/vectorial/mccatch/target/mccatch-1.0.jar"
algorithm="outlier.distance.McCatch"
name="McCatch"

[ -d code/vectorial/results ] || mkdir -p code/vectorial/results
# rm -f -- code/vectorial/results/$name.csv

for _ in {1..10}
do
    $javaCommand \
    -algorithm $algorithm \
    -time \
    -dbc.in $1 \
    -parser.labelIndices $2 \
    -mccatch.a 15 \
    -mccatch.b 0.1 \
    -mccatch.c 0.1 > $name.log
    awk '/runtime:/{print $0}' $name.log | awk '{split($0,array," "); print array[2]}' >> code/vectorial/results/$name.csv
done
rm $name.log