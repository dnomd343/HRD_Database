#!/bin/sh
echo "Initialize project..."

# Compile the engine
cd ../src/engine/
make
cd ../../
mv src/engine/engine build/engine

# Create folder
cd build
mkdir seed
mkdir solution
mkdir farthest
mkdir release

# Create seed
./engine --all
mv *-*-*.txt seed/
mv main.csv release/
mv All_Case.txt release/