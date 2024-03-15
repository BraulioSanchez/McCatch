#!/bin/bash

export javaCommand="java -Xmx30G -jar competitors/elki-0.8.0.jar KDDCLIApplication"
algorithm="outlier.distance.ODIN"
name="ODIN"

[ -d competitors/results ] || mkdir -p competitors/results
# rm -f -- competitors/results/$name.csv

file=$1
labelIndices=$2

for k in 1 5 10 20 30 50
do
    $javaCommand \
    -algorithm $algorithm \
    -time \
    -dbc.in $file \
    -parser.labelIndices $labelIndices \
    -odin.k $k > $name.log
    awk -v dataset="$3" '/runtime: |measures AUROC|Adjusted AUROC|measures AUPRC|Adjusted AUPRC|measures AUPRGC|Adjusted AUPRGC|measures Average Precision|Adjusted AveP|measures R-Precision|Adjusted R-Prec|measures Maximum F1|Adjusted Max F1|measures DCG|Adjusted DCG|measures NDCG /{
                    if (/hdbscan.minPts/) {
                        hdbscan_minPts = $NF;  # assuming the value is in the last field
                    } else if (/hdbscan.minclsize/) {
                        hdbscan_minclsize = $NF;
                    } else if (/measures AUROC/) {
                        auroc = $NF;
                    } else if (/Adjusted AUROC/) {
                        adj_auroc = $NF;
                    } else if (/measures AUPRC/) {
                        auprc = $NF;
                    } else if (/Adjusted AUPRC/) {
                        adj_auprc = $NF;
                    } else if (/measures AUPRGC/) {
                        auprgc = $NF;
                    } else if (/Adjusted AUPRGC/) {
                        adj_auprgc = $NF;
                    } else if (/measures Average Precision/) {
                        avg_prec = $NF;
                    } else if (/Adjusted AveP/) {
                        adj_avg_prec = $NF;
                    } else if (/measures R-Precision/) {
                        rprec = $NF;
                    } else if (/Adjusted R-Prec/) {
                        adj_rprec = $NF;
                    } else if (/measures Maximum F1/) {
                        max_f1 = $NF;
                    } else if (/Adjusted Max F1/) {
                        adj_max_f1 = $NF;
                    } else if (/measures DCG/) {
                        dcg = $NF;
                    } else if (/Adjusted DCG/) {
                        adj_dcg = $NF;
                    } else if (/measures NDCG/) {
                        ndcg = $NF;
                    } else if (/runtime: /) {
                        runtime = $(NF-1);
                    }
                } END {
                    print auroc","avg_prec","max_f1;
                }' $name.log >> competitors/results/$name.csv
    rm $name.log
done