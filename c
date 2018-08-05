#!/bin/bash
mkdir -p build
pushd build >& /dev/null
conan install --build --profile clang ..
popd >& /dev/null
