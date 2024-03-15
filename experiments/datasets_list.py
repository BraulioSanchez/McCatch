import numpy as np

datasets_vectorial_benchmark_axioms = [
    ["datasets/vectorial/benchmark/parkinson_preprocessed.csv", ",", 22, 50, "Parkinson"],
    ["datasets/vectorial/benchmark/hepatitis_preprocessed.csv", ",", 20, 70, "Hepatitis"],
    ["datasets/vectorial/benchmark/wine_preprocessed.csv", ",", 13, 129, "Wine"],
    ["datasets/vectorial/benchmark/glass_preprocessed.csv", ",", 9, 213, "Glass"],
    ["datasets/vectorial/benchmark/vertebral_preprocessed.csv", ",", 6, 240, "Vertebral"],
    ["datasets/vectorial/benchmark/ecoli_preprocessed.csv", ",", 7, 336, "Ecoli"],
    ["datasets/vectorial/benchmark/ionosphere_preprocessed.csv", ",", 33, 350, "Ionosphere"],
    ["datasets/vectorial/benchmark/pima_preprocessed.csv", ",", 8, 526, "Pima"],
    ["datasets/vectorial/benchmark/vowels_preprocessed.csv", ",", 12, 1452, "Vowels"],
    ["datasets/vectorial/benchmark/thyroid_preprocessed.csv", ",", 6, 3656, "Thyroid"],
    ["datasets/vectorial/benchmark/mammography_preprocessed.csv", ",", 6, 7848, "Mammography"],
    ["datasets/vectorial/benchmark/satimage2_preprocessed.csv", ",", 36, 5803, "Satimage2"],
    ["datasets/vectorial/benchmark/satellite_preprocessed.csv", ",", 36, 6435, "Satellite"],
    ["datasets/vectorial/benchmark/kddcupLeftMLOStm25_preprocessed.csv", ",", 25, 24995, "kddcup08"],
    ["datasets/vectorial/benchmark/shuttle_preprocessed.csv", ",", 9, 49097, "Shuttle"], 

    ["datasets/vectorial/benchmark/annthyroid_preprocessed.csv", ",", 21, 7129, "Annthyroid"],
    ["datasets/vectorial/benchmark/http_preprocessed.csv", ",", 3, 222027, "Http"],
    ["datasets/vectorial/benchmark/speech_preprocessed.csv", ",", 400, 3686, "Speech"],

    ["datasets/vectorial/axioms/cross_isolation_axiom.csv", ",", 2, 1000020, "Cross (I. Axiom)"],
    ["datasets/vectorial/axioms/cross_cardinality_axiom.csv", ",", 2, 1000110, "Cross (C. Axiom)"],

    ["datasets/vectorial/axioms/arc_isolation_axiom.csv", ",", 2, 999604, "Arc (I. Axiom)"],
    ["datasets/vectorial/axioms/arc_cardinality_axiom.csv", ",", 2, 1000310, "Arc (C. Axiom)"],

    ["datasets/vectorial/axioms/gaussian_isolation_axiom.csv", ",", 2, 1000020, "Gaussian (I. Axiom)"],
    ["datasets/vectorial/axioms/gaussian_cardinality_axiom.csv", ",", 2, 1000110, "Gaussian (C. Axiom)"],
]

datasets_vectorial_axioms_samples = [
    ["datasets/vectorial/axioms/gaussian_cardinality_samples/", ",", 2, "Gaussian (I. Axiom)", [np.arange(10, dtype="int"), np.arange(10, 20, dtype="int")]],
    ["datasets/vectorial/axioms/cross_cardinality_samples/", ",", 2, "Cross (I. Axiom)", [np.arange(10, dtype="int"), np.arange(10, 20, dtype="int")]],
    ["datasets/vectorial/axioms/arc_cardinality_samples/", ",", 2, "Arc (I. Axiom)", [np.arange(10, dtype="int"), np.arange(10, 20, dtype="int")]],

    ["datasets/vectorial/axioms/gaussian_cardinality_samples/", ",", 2, "Gaussian (C. Axiom)", [np.arange(100, dtype="int"), np.arange(100, 110, dtype="int")]],
    ["datasets/vectorial/axioms/cross_cardinality_samples/", ",", 2, "Cross (C. Axiom)", [np.arange(100, dtype="int"), np.arange(100, 110, dtype="int")]],
    ["datasets/vectorial/axioms/arc_cardinality_samples/", ",", 2, "Arc (C. Axiom)", [np.arange(100, dtype="int"), np.arange(100, 110, dtype="int")]],
]

datasets_vectorial_scalability = [
    ["datasets/vectorial/benchmark/satellite_preprocessed.csv", ",", 36, 6435, "Satellite"],
    ["datasets/vectorial/benchmark/http_preprocessed.csv", ",", 3, 222027, "Http"],
    ["datasets/vectorial/benchmark/speech_preprocessed.csv", ",", 400, 3686, "Speech"],

    ["datasets/vectorial/axioms/gaussian_isolation_axiom.csv", ",", 2, 1000020, "Gaussian (I. Axiom)"],
    ["datasets/vectorial/axioms/gaussian_cardinality_axiom.csv", ",", 2, 1000110, "Gaussian (C. Axiom)"],
    
    ["datasets/vectorial/axioms/cross_isolation_axiom.csv", ",", 2, 1000020, "Cross (I. Axiom)"],
    ["datasets/vectorial/axioms/cross_cardinality_axiom.csv", ",", 2, 1000110, "Cross (C. Axiom)"],

    ["datasets/vectorial/axioms/arc_isolation_axiom.csv", ",", 2, 999604, "Arc (I. Axiom)"],
    ["datasets/vectorial/axioms/arc_cardinality_axiom.csv", ",", 2, 1000310, "Arc (C. Axiom)"],

    ["datasets/vectorial/scalability/2d/2d", ",", 2, 1000000, "2 Dimensions"],
    ["datasets/vectorial/scalability/4d/4d", ",", 2, 1000000, "4 Dimensions"],
    ["datasets/vectorial/scalability/20d/20d", ",", 2, 1000000, "20 Dimensions"],
    ["datasets/vectorial/scalability/50d/50d", ",", 2, 1000000, "50 Dimensions"],
]

datasets_metric_vectorial = [
    ["datasets/vectorial/satellite_imagery/shanghai_preprocessed.csv", ",", 3, 1296, "Shanghai"],
    ["datasets/vectorial/satellite_imagery/volcanoes_preprocessed.csv", ",", 3, 3721, "Volcanoes"],

    ["datasets/metric/last_names.csv", "\t", None, 5050, "Last Names"],
    ["datasets/metric/fingerprints.csv", "\t", None, 408, "Fingerprints"],
    ["datasets/metric/skeletons.csv", "\t", None, 203, "Skeletons"],

    ["datasets/vectorial/benchmark/http_preprocessed.csv", ",", 3, 222027, "Http"],
]
