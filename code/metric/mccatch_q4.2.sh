#!/bin/bash

export command="code/metric/mccatch/sample_ngedit/mccatch"
name="McCatch"

[ -d code/metric/results ] || mkdir -p code/metric/results

$command \
    $1 \
    15 \
    0.1 \
    0.1 \
    2 > code/metric/results/$name.csv