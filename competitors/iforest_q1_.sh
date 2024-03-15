#!/bin/bash

export javaCommand="java -Xmx30G -jar competitors/elki-0.8.0.jar KDDCLIApplication"
algorithm="outlier.density.IsolationForest"
name="iForest"

[ -d competitors/results ] || mkdir -p competitors/results
# rm -f -- competitors/results/$name.csv

process_dataset() {
    local file=$1
    local labelIndices=$2

    percentage=0.3
    maxpsi=$(echo "$3 * $percentage" | bc)
    maxpsi=$(echo $maxpsi | awk -F'.' '{print $1}')
    flag=0
    for psi in 2 4 8 16 32 64 128 256 512 1024
    do
        if [ $psi -gt $maxpsi ]; then
            flag=1
            psi=$maxpsi
        fi

        for _ in {1..10}
        do
            $javaCommand \
            -algorithm $algorithm \
            -time \
            -dbc.in $file \
            -parser.labelIndices $labelIndices \
            -iforest.seed $((1 + RANDOM % 2147483647)) \
            -iforest.subsample $psi > $name.log
            awk -v dataset="$4" '/runtime: |measures AUROC|Adjusted AUROC|measures AUPRC|Adjusted AUPRC|measures AUPRGC|Adjusted AUPRGC|measures Average Precision|Adjusted AveP|measures R-Precision|Adjusted R-Prec|measures Maximum F1|Adjusted Max F1|measures DCG|Adjusted DCG|measures NDCG /{
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
        done
        rm $name.log

        if [ $flag -eq 1 ]; then
            break
        fi
    done
}

file="datasets/vectorial/benchmark/parkinson_preprocessed.csv"
labelIndices=22
cardinality=50
dataset="Parkinson"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/hepatitis_preprocessed.csv"
labelIndices=20
cardinality=70
dataset="Hepatitis"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/wine_preprocessed.csv"
labelIndices=13
cardinality=129
dataset="Wine"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/glass_preprocessed.csv"
labelIndices=9
cardinality=213
dataset="Glass"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/vertebral_preprocessed.csv"
labelIndices=6
cardinality=240
dataset="Vertebral"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/ecoli_preprocessed.csv"
labelIndices=7
cardinality=336
dataset="Ecoli"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/ionosphere_preprocessed.csv"
labelIndices=33
cardinality=350
dataset="Ionosphere"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/pima_preprocessed.csv"
labelIndices=8
cardinality=526
dataset="Pima"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/vowels_preprocessed.csv"
labelIndices=12
cardinality=1452
dataset="Vowels"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/thyroid_preprocessed.csv"
labelIndices=6
cardinality=3656
dataset="Thyroid"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/mammography_preprocessed.csv"
labelIndices=6
cardinality=7848
dataset="Mammography"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/satimage2_preprocessed.csv"
labelIndices=36
cardinality=5803
dataset="Satimage2"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/satellite_preprocessed.csv"
labelIndices=36
cardinality=6435
dataset="Satellite"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/kddcupLeftMLOStm25_preprocessed.csv"
labelIndices=25
cardinality=24995
dataset="kddcup08"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/shuttle_preprocessed.csv"
labelIndices=9
cardinality=49097
dataset="Shuttle"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/annthyroid_preprocessed.csv"
labelIndices=21
cardinality=7129
dataset="Annthyroid"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/benchmark/http_preprocessed.csv"
labelIndices=3
cardinality=222027
dataset="Http"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/axioms/cross_isolation_axiom.csv"
labelIndices=2
cardinality=1000020
dataset="Cross_I_Axiom"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/axioms/cross_cardinality_axiom.csv"
labelIndices=2
cardinality=1000110
dataset="Cross_C_Axiom"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/axioms/arc_isolation_axiom.csv"
labelIndices=2
cardinality=999604
dataset="Arc_I_Axiom"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/axioms/arc_cardinality_axiom.csv"
labelIndices=2
cardinality=1000310
dataset="Arc_C_Axiom"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/axioms/gaussian_isolation_axiom.csv"
labelIndices=2
cardinality=1000020
dataset="Gaussian_I_Axiom"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"

file="datasets/vectorial/axioms/gaussian_cardinality_axiom.csv"
labelIndices=2
cardinality=1000110
dataset="Gaussian_C_Axiom"
process_dataset "$file" "$labelIndices" "$cardinality" "$dataset"