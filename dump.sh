#!/bin/bash
set -eu

./bcat.py "uniquekdumpall" | tee _dumped.hex | xxd
