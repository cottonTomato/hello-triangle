default: build

@setup:
    cmake -S . -B build

@clean:
    rm -rf build
    rm -rf .cache
    rm ./main

@build:
    cmake --build build

@run: build
    mv ./build/main .
    ./main
