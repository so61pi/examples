#!/bin/sh

set -e

mkdir /tmp/build && cd /tmp/build

cmake /code
cmake --build . --target install

# Wait until the PostgreSQL is up.
sleep 5
test-connect
