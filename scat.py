#!/usr/bin/env python3

from serial import Serial
from select import select
import sys

sp = Serial('/dev/ttyACM0', baudrate=19200, timeout=0.2, write_timeout=0)

while True:
    ser = sp.readline()
    if ser:
        print(ser.decode().strip())
    if select([sys.stdin], [], [], 0)[0]:
        sp.write((sys.stdin.readline().strip() + " ").encode())

