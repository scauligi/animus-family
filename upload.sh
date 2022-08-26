#!/bin/bash
set -eu

avrdude -cavr109 -patmega32u4 -P/dev/tty.usbmodemCHIDKH1 -b57600 -D -v "-Uflash:w:$1:i"
