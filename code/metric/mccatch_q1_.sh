#!/bin/bash

export command="code/metric/mccatch/sample_nledit/mccatch"
name="McCatch"

[ -d code/metric/results ] || mkdir -p code/metric/results

for _ in {1..10}
do
    $command \
        $1 \
        15 \
        0.1 \
    0.1 | awk -v param=$2 '{print param","$0}' >> code/metric/results/$name.csv
done