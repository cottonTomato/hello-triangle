default: build

@setup:
    cmake -S . -B build

@clean:
    rm -rf build
    rm -rf .cache

@build:
    cmake --build build
    mv ./build/main .
    ./main

@run:
    cmake --build build
    ./build/main
