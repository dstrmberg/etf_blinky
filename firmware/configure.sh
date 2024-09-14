#!/usr/bin/env bash

set -ex

# debug build
cmake \
    --toolchain tiny44a-toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -G Ninja \
    -B build/debug

# release build
cmake \
    --toolchain tiny44a-toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -G Ninja \
    -B build/release
