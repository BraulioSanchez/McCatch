#!/bin/bash

# For C++ implementation
cd code/metric/mccatch/
make clean && make
cd sample_nledit
make clean && make
cd ../sample/
make clean && make
cd ../sample_ngedit/
make clean && make

# For Java implementations
cd ../../../vectorial/mccatch
mvn clean package