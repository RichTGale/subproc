#!/bin/bash

cmake -H. -B ./debug -DCMAKE_BUILD_TYPE=debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
ln -s ./debug/compile_commands.json .

mkdir ./media ./output
