#!/usr/bin/env python3

import math
import sys
import re
import textwrap
from collections import defaultdict as ddict
from pathlib import Path

import yaml

BOX = " XX─X┌┐┬X└┘┴│├┤┼"

INWIDTH = 6
INHEIGHT = 2

WIDTH = INWIDTH + 1
HEIGHT = INHEIGHT + 1

YOFF = [0, HEIGHT, math.ceil(HEIGHT / 3), 0, 0, 0, math.ceil(HEIGHT / 3)]

LAYER_NAMES = ["Base", "Sym", "Fn", "NPad", "Mouse"]


def OFF(x, y):
    match x, y:
        case 5, 4:
            return (2, HEIGHT)
        case 6, 4:
            return (2, YOFF[x] + HEIGHT)
        case 7, 4:
            return (2, 2 * HEIGHT)
        case 7, _:
            return None
        case _:
            return (0, YOFF[x])


def newbuf():
    return ddict(int)


def box(buf, start, width, height):
    X, Y = start
    for i in range(X, X + width):
        buf[(i, Y)] |= 0b0001
        buf[(i, Y + height)] |= 0b0001
    for i in range(X + 1, X + width + 1):
        buf[(i, Y)] |= 0b0010
        buf[(i, Y + height)] |= 0b0010
    for j in range(Y, Y + height):
        buf[(X, j)] |= 0b0100
        buf[(X + width, j)] |= 0b0100
    for j in range(Y + 1, Y + height + 1):
        buf[(X, j)] |= 0b1000
        buf[(X + width, j)] |= 0b1000


def col(buf, start, sz):
    X, Y = start
    for y in range(Y, Y + HEIGHT * sz + 1):
        vert = 0
        if y > Y:
            vert |= 0b1000
        if y < Y + HEIGHT * sz:
            vert |= 0b0100
        for x in (X, X + WIDTH):
            buf[(x, y)] |= vert
        if (y - Y) % HEIGHT == 0:
            for x in range(X + 1, X + WIDTH):
                buf[(x, y)] |= 0b0011
            buf[(X, y)] |= 0b0001
            buf[(X + WIDTH, y)] |= 0b0010


def left(buf, start, *, flip=False):
    X, Y = start
    if flip:
        _orig_col = col

        def _col(buf, start, sz):
            _orig_col(buf, (2 * X - start[0] - WIDTH, start[1]), sz)

    else:
        _col = col
    for i in range(7):
        x = X + WIDTH * i
        y = Y + YOFF[i]
        _col(buf, (x, y), 5 if i < 5 else 4)
    x = X + 5 * WIDTH + 2
    y = Y + HEIGHT * 5
    _col(buf, (x, y), 1)
    x += WIDTH
    y += math.ceil(HEIGHT / 3)
    _col(buf, (x, y), 1)
    x += WIDTH
    y += math.floor(2 * HEIGHT / 3)
    _col(buf, (x, y), 1)


def render(buf):
    rendered = []
    k = buf.keys()
    maxX = max(p[0] for p in k) + 1
    maxY = max(p[1] for p in k) + 1
    for y in range(maxY):
        line = []
        for x in range(maxX):
            line.append(BOX[buf[(x, y)]])
        rendered.append(line)
    return rendered


def trunc(s, n):
    return s[: n - 1] + (s[n - 1 : n] if len(s) <= n else '⋯')


def write(rendered, start, s, *, overwrite=False):
    X, Y = start
    for i, c in enumerate(s):
        x = X + i
        if overwrite or rendered[Y][x] == ' ':
            rendered[Y][x] = c


def display(rendered):
    print('\n'.join(''.join(line) for line in rendered))


def parse_tuple(t):
    if m := re.match(r'\((\d+), (\d+)\)', t):
        return (int(m[1]), int(m[2]))
    return t


def ptadd(*args):
    return map(sum, zip(*args))


with open('arbites.yaml') as f:
    data = yaml.safe_load(f)


def _tr(name):
    if re.fullmatch(r'[a-z]', name):
        return name.upper()
    if m := re.fullmatch(r'(\S) \S', name):
        return m[1]
    if m := re.fullmatch(r'\S (\S) \+ Shift', name):
        return m[1]
    if name == 'Null':
        return ''
    name = re.sub(r'layer (\d+)', lambda m: LAYER_NAMES[w].lower() if (w := int(m[1]) - 1) < len(LAYER_NAMES) else m[0], name)
    return name


pair_to_name = {parse_tuple(k): _tr(v) for k, v in data['pair_to_name'].items()}

leftright = Path('leftright.hex').read_bytes()
lrom = bytearray(leftright[:1024])
rrom = bytearray(leftright[1024:])


def boxwrap(s, width=INWIDTH, height=INHEIGHT):
    return trunc(
        ''.join((format(line, f'<{width}') for line in textwrap.wrap(s, width))),
        width * height,
    )


def boxlines(s, width=INWIDTH, height=INHEIGHT):
    s = boxwrap(s)
    for i in range(min(math.ceil(len(s) / width), height)):
        yield s[i * width : (i + 1) * width]


def macros():
    buf = newbuf()
    overlay = []
    for i in range(12):
        addr = 600 + (i * (8 * 2 + 2))
        downup_bitmap = lrom[addr]
        nkeys = lrom[addr + 1]
        if 0 < nkeys <= 8:
            col(buf, (0, i * HEIGHT), 1)
            for j, line in enumerate(boxlines(f'macro {i}')):
                overlay.append((1, i * HEIGHT + j + 1, line))
            assert not downup_bitmap
            for n in range(nkeys):
                pt = ((n + 1) * WIDTH + 2, i * HEIGHT)
                col(buf, pt, 1)
                kaddr = addr + 2 + 2 * n
                kval, ktype = lrom[kaddr : kaddr + 2]
                name = pair_to_name[(ktype, kval)]
                for j, line in enumerate(boxlines(name)):
                    overlay.append((*ptadd(pt, (1, j + 1)), line))
    rendered = render(buf)
    for x, y, s in overlay:
        write(rendered, (x, y), s)
    display(rendered)


def layer(w):
    buf = newbuf()
    FANCY = 0
    left(buf, (0, FANCY), flip=False)
    maxX = max(p[0] for p in buf.keys())
    REV_OFF = 2 * maxX + WIDTH
    left(buf, (REV_OFF, FANCY), flip=True)

    labelpt = (maxX - WIDTH // 2, HEIGHT * 2)
    labelwidth = WIDTH + 2 * (WIDTH // 2)
    box(buf, labelpt, labelwidth, HEIGHT)

    addlater = {}
    for y in range(6):
        for x in range(8 * 2):
            if x in (7, 8) and y < 5:
                n = ''
            elif y == 0 and x not in (0, 15):
                n = ''
            elif x in (0, 15) and y == 5:
                n = ''
            else:
                yy = y if x in (0, 15) else y - 1
                z = lrom
                if x >= 8:
                    x = 15 - x
                    z = rrom
                addr = (x + 8 * yy + 5 * 8 * w) * 2
                kval, ktype = z[addr : addr + 2]
                n = pair_to_name[(ktype, kval)]
            if n:
                zz = int(z == rrom)
                index = (x, yy, zz)
                # pt = ((x if zz == 0 else 17 - x) * WIDTH, yy * HEIGHT)
                # col(buf, pt, 1)
                addlater[index] = (ktype, kval, n)

    rendered = render(buf)

    for (X, Y, Z), (ktype, kval, n) in addlater.items():
        x, y = ptadd((X * WIDTH, Y * HEIGHT), OFF(X, Y), (1, FANCY + 1))
        if Z:
            x = REV_OFF - x - INWIDTH + 1
        for i, s in enumerate(boxlines(n)):
            write(rendered, (x, y + i), s)

    layername = LAYER_NAMES[w] if w < len(LAYER_NAMES) else f'Layer {w+1}'
    lines = list(boxlines(layername, width=labelwidth))
    index = (INHEIGHT - len(lines)) // 2
    for line in lines:
        write(rendered, ptadd(labelpt, (1, index + 1)), line.strip().center(labelwidth))

    display(rendered)

def emulate(command):
    cmd, *args = command.split('(')
    args = map(int, args)
    match cmd:
        case "uniquekseteeprom":
            addr, val = args
            lrom[addr] = val
        case "uniqueksetkey":
            x, y, z, val, ty = args
            addr = ((z * 5 + y) * 8 + x) * 2
            lrom[addr:addr+2] = (val, ty)
        case "uniqueksetsubkey":
            x, y, z, val, ty = args
            addr = ((z * 5 + y) * 8 + x) * 2
            rrom[addr:addr+2] = (val, ty)
        case "uniqueksetmacro":
            index, length, *keys, overflow = args
            addr = 600 + index * (8 * 2 + 2)
            assert len(keys) % 3 == 0
            assert len(keys) // 3 == length
            bits = 0
            vals = []
            for i in range(length):
                val, ty, updown = keys[i*3:(i+1)*3]
                bits |= int(bool(updown)) << i
                vals.extend([val, ty])
            lrom[addr:addr+2+length*2] = bits, length, *vals


def main():
    nlayers = lrom[-1]

    if len(sys.argv) > 1:
        for fname in sys.argv:
            for line in Path(fname).read_text().splitlines():
                emulate(line)


    names = ', '.join(f'"{n}"' for n in LAYER_NAMES)
    print(f'Layers: {nlayers} (using: {names})')
    print()

    macros()
    print()

    for w in range(nlayers):
        layer(w)

if __name__ == '__main__':
    main()
