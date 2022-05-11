#!/bin/bash

echo "char prog[] = {"
hexdump -e '"\t" 8/4 "0x%08x, " "\n"' hello.bin
echo "};"
