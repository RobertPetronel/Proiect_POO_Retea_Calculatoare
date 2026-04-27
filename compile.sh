#!/bin/bash

mkdir -p build 2>"/dev/null"
cd build

cmake ..
make

mv Proiect ../main