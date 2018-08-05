#!/bin/bash
mkdir -p mbuild
pushd mbuild >& /dev/null
conan install --build missing --profile default ..
popd >& /dev/null
