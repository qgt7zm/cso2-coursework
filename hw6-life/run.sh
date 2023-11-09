#!/bin/sh

# Test all life implementations
clear
make
./life 10 input/tiny time-and-result || (echo "Test failed"; exit 1)
./life 75 input/o0075 time-and-result || (echo "Test failed"; exit 1)
./life 120 input/make-a time-and-result || (echo "Test failed"; exit 1)

./life-tsan 3 input/tiny time-and-result || (echo "Test failed"; exit 1)

# ./life 200 input/puf-qb-c3 time-and-result || (echo "Test failed"; exit 1)
# ./life 201 input/puf-qb-c3 time-and-result || (echo "Test failed"; exit 1)
