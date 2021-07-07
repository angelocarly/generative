#!/bin/bash

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
make

cd ..
ln -sf build/compile_commands.json .
