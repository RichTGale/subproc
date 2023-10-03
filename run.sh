#!/bin/bash

DIRECTORY="output"

cd ./build/bin
if [ ! -d "${DIRECTORY}" ]; then
    mkdir ./output
fi

./subproc.run

cd ../../
