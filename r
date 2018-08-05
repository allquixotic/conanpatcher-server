#!/bin/bash
pushd build/bin >& /dev/null
./conanpatcher -c ../../wt_config.xml --docroot=../../docroot --http-listen=127.0.0.1:8181
popd >& /dev/null
