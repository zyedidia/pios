#!/bin/bash

echo "uint8_t prog[] = {"
hexdump -e '"\t" 8/1 "0x%02x, " "\n"' hello.bin
echo "};"
