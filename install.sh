#!/bin/bash

cmake -H. -B ./ -DCMAKE_BUILD_TYPE=debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES

mkdir ./output
