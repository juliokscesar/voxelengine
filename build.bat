@echo off

if not exist build (
    mkdir build
)

cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . -j 8 --config Debug
