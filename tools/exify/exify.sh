#!/bin/bash

echo "#include <string.h>"
echo "unsigned char prog_$2[] = {"
hexdump -v -e '"\t" 4/1 "0x%02x, " "\n"' $1
echo "};"
echo "size_t prog_$2_sz = sizeof(prog_$2);"
