@echo off
cls
cd libctrpf/Library
make -j 8
cd ../..
make -j 8
pause
