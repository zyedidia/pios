#!/bin/bash

echo "char prog[] = {"
hexdump -e '"\t" 8/1 "0x%02x, " "\n"' $@
echo "};"
echo -n "unsigned n_prog = "
echo "$(cat $@ | wc -c);"
