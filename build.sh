#!/bin/bash

mkdir -p out/build/
cd out/build/
rm unique_words

cmake -S ../../ -B .
make
