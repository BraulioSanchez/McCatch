#!/bin/bash

name="D.MCA"

[ -d competitors/results ] || mkdir -p competitors/results

pip install -e competitors/$name/

process_dataset() {
    local file=$1
    local dataset=$2
    
    python competitors/$name/experiment/exp_DMCA.py --method DMCA --dataset $file --num-exp 10 --file $dataset >> competitors/results/$name.csv
}

# file="datasets/vectorial/benchmark/parkinson_preprocessed.csv"
# labelIndices=22
# dataset="Parkinson"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/hepatitis_preprocessed.csv"
# labelIndices=20
# dataset="Hepatitis"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/wine_preprocessed.csv"
# labelIndices=13
# dataset="Wine"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/glass_preprocessed.csv"
# labelIndices=9
# dataset="Glass"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/vertebral_preprocessed.csv"
# labelIndices=6
# dataset="Vertebral"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/ecoli_preprocessed.csv"
# labelIndices=7
# dataset="Ecoli"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/ionosphere_preprocessed.csv"
# labelIndices=33
# dataset="Ionosphere"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/pima_preprocessed.csv"
# labelIndices=8
# dataset="Pima"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/vowels_preprocessed.csv"
# labelIndices=12
# dataset="Vowels"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/thyroid_preprocessed.csv"
# labelIndices=6
# dataset="Thyroid"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/mammography_preprocessed.csv"
# labelIndices=6
# dataset="Mammography"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/satimage2_preprocessed.csv"
# labelIndices=36
# dataset="Satimage2"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/satellite_preprocessed.csv"
# labelIndices=36
# dataset="Satellite"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/kddcupLeftMLOStm25_preprocessed.csv"
# labelIndices=25
# dataset="kddcup08"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/shuttle_preprocessed.csv"
# labelIndices=9
# dataset="Shuttle"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/annthyroid_preprocessed.csv"
# labelIndices=21
# dataset="Annthyroid"
# process_dataset "$file" "$dataset"

file="datasets/vectorial/benchmark/speech_preprocessed.csv"
labelIndices=400
dataset="Speech"
process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/http_preprocessed.csv"
# labelIndices=3
# dataset="Http"
# process_dataset "$file" "$dataset"