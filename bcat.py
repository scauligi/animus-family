#!/usr/bin/env python3

from serial import Serial
from select import select
import sys

inputIsArgs = False

if len(sys.argv) > 1:
    inputIsArgs = True
    args = list(reversed(sys.argv[1:]))

sp = Serial('/dev/tty.usbmodemCHIDKH1', baudrate=19200, timeout=0.2, write_timeout=0)

while True:
    ser = sp.read()
    if ser:
        sys.stdout.buffer.write(ser)
        sys.stdout.buffer.flush()
    elif inputIsArgs:
        if args:
            sp.write((args.pop() + " ").encode())
        else:
            break
    elif select([sys.stdin], [], [], 0)[0]:
        sp.write((sys.stdin.readline().strip() + " ").encode())

