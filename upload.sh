#!/bin/bash
set -eu

avrdude -cavr109 -patmega32u4 -P/dev/ttyACM0 -b57600 -D -v "-Uflash:w:$1:i"
