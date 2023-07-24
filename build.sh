#!/bin/bash

mkdir -p out/build/
cd out/build/
rm unique_words

cmake -S ../../ -B .
make

./unique_words -i ../../res/abc.txt -v