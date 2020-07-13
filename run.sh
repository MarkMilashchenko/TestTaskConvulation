#!/bin/bash
rm start
rm direct_conv.bin
rm fast_conv.bin
cmake .
make
./start

