#!/bin/bash

export javaCommand="java -Xmx30G -jar competitors/elki-0.8.0.jar KDDCLIApplication"
algorithm="outlier.lof.LOCI"
name="LOCI"

[ -d competitors/results ] || mkdir -p competitors/results
# rm -f -- competitors/results/$name.csv

process_dataset() {
    local file=$1
    local labelIndices=$2

    for perc in 0.05 0.1 0.25 0.5 1
    do
        rmax=`echo "scale=12; sqrt($2) * $perc" | bc -l`
        $javaCommand \
        -algorithm $algorithm \
        -time \
        -dbc.in $file \
        -parser.labelIndices $labelIndices \
        -loci.rmax $rmax > $name.log
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
                        print dataset","auroc","auprc","auprgc","avg_prec","rprec","max_f1","dcg","ndcg","adj_auroc","adj_auprc","adj_auprgc","adj_avg_prec","adj_rprec","adj_max_f1","adj_dcg","runtime;
                    }' $name.log >> competitors/results/$name.csv
        rm $name.log    
    done
}

file="datasets/vectorial/benchmark/parkinson_preprocessed.csv"
labelIndices=22
dataset="Parkinson"
process_dataset "$file" "$labelIndices" "$dataset"

file="datasets/vectorial/benchmark/hepatitis_preprocessed.csv"
labelIndices=20
dataset="Hepatitis"
process_dataset "$file" "$labelIndices" "$dataset"

file="datasets/vectorial/benchmark/wine_preprocessed.csv"
labelIndices=13
dataset="Wine"
process_dataset "$file" "$labelIndices" "$dataset"

file="datasets/vectorial/benchmark/glass_preprocessed.csv"
labelIndices=9
dataset="Glass"
process_dataset "$file" "$labelIndices" "$dataset"

file="datasets/vectorial/benchmark/vertebral_preprocessed.csv"
labelIndices=6
dataset="Vertebral"
process_dataset "$file" "$labelIndices" "$dataset"

file="datasets/vectorial/benchmark/ecoli_preprocessed.csv"
labelIndices=7
dataset="Ecoli"
process_dataset "$file" "$labelIndices" "$dataset"

file="datasets/vectorial/benchmark/ionosphere_preprocessed.csv"
labelIndices=33
dataset="Ionosphere"
process_dataset "$file" "$labelIndices" "$dataset"

file="datasets/vectorial/benchmark/pima_preprocessed.csv"
labelIndices=8
dataset="Pima"
process_dataset "$file" "$labelIndices" "$dataset"

file="datasets/vectorial/benchmark/vowels_preprocessed.csv"
labelIndices=12
dataset="Vowels"
process_dataset "$file" "$labelIndices" "$dataset"

file="datasets/vectorial/benchmark/thyroid_preprocessed.csv"
labelIndices=6
dataset="Thyroid"
process_dataset "$file" "$labelIndices" "$dataset"