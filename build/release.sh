#!/bin/sh

echo "merge farthest..."
cat ./farthest/* > ./farthest_raw.csv

echo "merge solution..."
cat ./solution/* > ./solution_raw.csv

g++ -O3 ../tools/streamline_csv.cpp -o ./streamline_csv
./streamline_csv

mv ./farthest.csv ./release/farthest.csv
mv ./solution.csv ./release/solution.csv

rm ./farthest_raw.csv
rm ./solution_raw.csv
rm ./streamline_csv