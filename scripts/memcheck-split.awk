#!/usr/bin/env -S awk -f
BEGIN {
    "dirname "ARGV[1] | getline path
}
/^==/ {
    print $0 > ARGV[1]".report"
}
!/^==/ {
    print $0 >> path"/all.supp"
}
