#!/bin/bash

name="Gen2Out"

[ -d competitors/results ] || mkdir -p competitors/results

process_dataset() {
    local file=$1
    local dataset=$2

    python competitors/gen2Out/gen2out_.py $file $dataset >> competitors/results/$name.csv
}

# file="datasets/vectorial/benchmark/parkinson_preprocessed.csv"
# dataset="Parkinson"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/wine_preprocessed.csv"
# dataset="Wine"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/glass_preprocessed.csv"
# dataset="Glass"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/vertebral_preprocessed.csv"
# dataset="Vertebral"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/ecoli_preprocessed.csv"
# dataset="Ecoli"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/ionosphere_preprocessed.csv"
# dataset="Ionosphere"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/pima_preprocessed.csv"
# dataset="Pima"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/vowels_preprocessed.csv"
# dataset="Vowels"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/thyroid_preprocessed.csv"
# dataset="Thyroid"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/mammography_preprocessed.csv"
# dataset="Mammography"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/satimage2_preprocessed.csv"
# dataset="Satimage2"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/satellite_preprocessed.csv"
# dataset="Satellite"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/kddcupLeftMLOStm25_preprocessed.csv"
# dataset="kddcup08"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/shuttle_preprocessed.csv"
# dataset="Shuttle"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/annthyroid_preprocessed.csv"
# dataset="Annthyroid"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/musk_preprocessed.csv"
# dataset="Musk-166d"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/musk_reduced.csv"
# dataset="Musk-50d"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/benchmark/http_preprocessed.csv"
# dataset="Http"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/axioms/cross_isolation_axiom.csv"
# dataset="Cross_I_Axiom"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/axioms/cross_cardinality_axiom.csv"
# dataset="Cross_C_Axiom"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/axioms/arc_isolation_axiom.csv"
# dataset="Arc_I_Axiom"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/axioms/arc_cardinality_axiom.csv"
# dataset="Arc_C_Axiom"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/axioms/gaussian_isolation_axiom.csv"
# dataset="Gaussian_I_Axiom"
# process_dataset "$file" "$dataset"

# file="datasets/vectorial/axioms/gaussian_cardinality_axiom.csv"
# dataset="Gaussian_C_Axiom"
# process_dataset "$file" "$dataset"