#!/bin/bash
set -eu

./bcat.py "uniquekdumpall" | tee leftright.hex | xxd
