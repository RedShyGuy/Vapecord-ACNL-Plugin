@echo off
cls

set /p buildLib=Build libctrpf? (y/n):

if /I "%buildLib%"=="y" (
    cd libctrpf\Library
    make -j 8
    cd ..\..
) else (
    echo Skip libctrpf...
)

make -j 8
pause