#!/usr/bin/env python3

from serial import Serial
from select import select
import sys

sp = Serial('/dev/tty.usbmodemCHIDKH1', baudrate=19200, timeout=0.2, write_timeout=0)

while True:
    ser = sp.readline()
    if ser:
        print(ser.decode().strip())
    if select([sys.stdin], [], [], 0)[0]:
        line = sys.stdin.readline()
        if not line:
            # EOF is returned as an empty string
            break
        sp.write((sys.stdin.readline().strip() + " ").encode())

