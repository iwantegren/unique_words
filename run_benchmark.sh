#!/bin/bash

mkdir -p out/benchmark/
cd out/benchmark/
rm benchmark

cmake -S ../../benchmark -B .
make

echo "------------------------------------------------------------"
./benchmark --benchmark_time_unit=ms