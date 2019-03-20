#!/bin/sh

update2github() {
    git remote update "origin"
    git push --mirror "github"
}

for d in */ ; do
    echo "--------"
    echo "Updating $d"
    (cd "$d" && update2github)
    echo
done
