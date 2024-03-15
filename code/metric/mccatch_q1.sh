#!/bin/bash

export command="code/metric/mccatch/sample_nledit/mccatch"
name="McCatch"

[ -d code/metric/results ] || mkdir -p code/metric/results

$command \
    $1 \
    15 \
    0.1 \
1 > code/metric/results/$name.csv