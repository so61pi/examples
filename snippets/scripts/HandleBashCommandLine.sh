#! /bin/bash

all=""
base=""
while :; do
    case "$1" in
        -a|--all)
            all="$2"
            shift 2;;
        -b|--base)
            base="base"
            shift 1;;
        -*)
            echo "Unknown flag : $1"
            exit 1
            ;;
        *)
            break
            ;;
    esac
done

echo "all = $all"
echo "base = $base"
