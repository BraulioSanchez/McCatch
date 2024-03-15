#!/bin/bash

case "$5" in
    "Last Names") export command="code/metric/mccatch/sample_nledit/mccatch" ;;
    "Fingerprints") export command="code/metric/mccatch/sample_nledit/mccatch" ;;
    "Skeletons") export command="code/metric/mccatch/sample_ngedit/mccatch" ;;
    *)  echo "Unknown option: $5" ;;
esac

name="McCatch"

[ -d code/metric/results ] || mkdir -p code/metric/results

$command \
    $1 \
    $2 \
    $3 \
    $4 \
    2 > code/metric/results/$name.csv