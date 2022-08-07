#!/usr/bin/env python3

from serial import Serial

sp = Serial('/dev/ttyACM0', baudrate=19200, timeout=1, write_timeout=0)

def rpc(msg, *args):
    fargs = ''.join(f"({arg}" for arg in args)
    msg = f"uniquek{msg}{fargs} "
    sp.write(msg.encode())
    return sp.readline().decode().strip()

sp.write("dumpme ".encode())
dump = sp.read(1024)
print(repr(dump))
with open('dump.eeprom', 'wb') as f:
    f.write(dump)

# nlayers = int(rpc("getlay"))
# print(f'Layers: {nlayers}')

# print(repr(rpc("getkey", 3, 2, 0)))

# print(repr(rpc("getmods")))
