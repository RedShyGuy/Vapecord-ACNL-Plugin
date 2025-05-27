#! /bin/bash
clear
cd libctrpf/Library
make -j 8
cd ../..
make -j 8
echo finished
