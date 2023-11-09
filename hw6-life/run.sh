#!/bin/sh

# Test all life implementations
clear
make

./life-tsan 3 input/tiny time || (echo "Test failed"; exit 1)
./life-asan 10 input/make-a time || (echo "Test failed"; exit 1)

./life-tsan 3 input/puf-qb-c3 time || (echo "Test failed"; exit 1)
./life-tsan 3 input/23334m time || (echo "Test failed"; exit 1)
