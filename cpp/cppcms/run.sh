#!/bin/sh

set -e

CODEDIR=/code
CONFIGFILE=$CODEDIR/config/config.json

WD=$(mktemp -d)
cd "$WD"
cmake "$CODEDIR"
cmake --build .
make install

echo ========================================
echo cppcms-test -c "$CONFIGFILE"
cppcms-test -c "$CONFIGFILE"
