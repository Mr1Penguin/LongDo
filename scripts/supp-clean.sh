#!/usr/bin/env bash

if [ "$#" -ne 1 ]
then
    echo "$0 file" >&2
    exit 1
fi

if [ -e "$1" ]
then
    awk -e '/}/ { printf "}\0"; next } { print $0 }' "$0" | sort -z | uniq -z | tr '\0' '\n' > "$(dirname "$1")"/clean.supp
fi
