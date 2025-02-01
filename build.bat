@echo off

if not exist build (
    mkdir build
)

cd build
cmake ..
cmake --build . -j 6 --config Debug
