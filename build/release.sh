#!/bin/sh

echo "merge farthest..."
cat ./farthest/* > ./release/farthest.csv

echo "merge solution..."
cat ./solution/* > ./release/solution.csv