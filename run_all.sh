#!/bin/bash

bash build.sh
echo "------------------------------------------------------------"

bash run_test.sh
echo "------------------------------------------------------------"

bash run_benchmark.sh