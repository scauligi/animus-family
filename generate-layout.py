#!/usr/bin/env python3

import yaml
import re
from pathlib import Path
import urllib.parse

#25b0db
#5c56db

class KeyFile:
    def __init__(self):
        self.keymap = {}
        self.macros = {}
        self.tapdance = {}

def tw(tup, n):
    return tuple(list(tup) + [n])

def swap(pair):
    return (pair[1], pair[0])

with open('arbites.yaml') as f:
    data = yaml.safe_load(f)
def parse_tuple(t):
    if m := re.match(r'\((\d+), (\d+)\)', t):
        return (int(m[1]), int(m[2]))
    return t
pair_to_name = {parse_tuple(k): v for k, v in data['pair_to_name'].items()}

newkeys = KeyFile()
leftright = Path('leftright.hex').read_bytes()
left = leftright[:1024]
right = leftright[1024:]
nlayers = left[-1]

for w,side in enumerate([left, right]):
    for z in range(nlayers):
        for y in range(5):
            for x in range (8):
                if x == 7 and y < 4:
                    continue
                addr = (x + 8 * y + 5 * 8 * z) * 2
                newkeys.keymap[(x,y,w,z)] = swap(tuple(side[addr:addr+2]))

# read macro data
for i in range(12):
    addr = 600 + (i * (8 * 2 + 2));
    downup_bitmap = left[addr]
    nkeys = left[addr+1]
    if 0 < nkeys <= 8:
        newkeys.macros[i] = []
        for n in range(nkeys):
            kaddr = addr + 2 + 2*n
            newkeys.macros[i].append(swap(tuple(left[kaddr:kaddr+2])))

lmap = {
    'Null': '',
    'Backspace': 'Bksp',
    'Delete': 'Del',
    'Up': '↑',
    'Down': '↓',
    'Left': '←',
    'Right': '→',
    'Page Up': 'PgUp',
    'Page Down': 'PgDn',
    'Win': "<i class='kb kb-logo-windows-8'></i>",
    'Vol Up': "<i class='kb kb-Multimedia-Volume-Up-2'></i>",
    'Vol Dn': "<i class='kb kb-Multimedia-Volume-Down-2'></i>",
    'Mute': "<i class='kb kb-Multimedia-Mute-2'></i>",
    'M Play': "<i class='kb kb-Multimedia-Play-Pause'></i>",
    'M Back': "<i class='kb kb-Multimedia-Rewind-Start'></i>",
    'M Next': "<i class='kb kb-Multimedia-FastForward-End'></i>",
}
def legendmap(legend, withshift=True):
    if len(legend) == 1 and legend.islower():
        legend = legend.upper()
    elif (not withshift) and len(legend) == 3 and legend[1] == ' ':
        legend = legend[0]
    elif legend in lmap:
        legend = lmap[legend]
    elif legend.endswith(' + Shift'):
        legend = legend[:-len(' + Shift')]
        legend = legendmap(legend)
        if len(legend) == 3 and legend[1] == ' ':
            legend = legend[2]
        elif legend == 'Tab':
            legend = '←Tab'
    elif legend.startswith('Num '):
        legend = legend[len('Num '):]
        legend = legendmap(legend)
    elif legend.startswith('Fn'):
        if legend == "Fn":
            num = 2
        else:
            num = int(legend[2:]) + 1
        rows = 1
        while num // rows > 4:
            rows += 1
        legend = ""
        remaining = num
        for n in range(rows, 0, -1):
            row = remaining // n
            legend += "<i class='fa fa-angle-double-up'/>" * row + '<br/>'
            remaining -= row
        legend = legend[:-len('<br/>')]
    elif legend.startswith('macro'):
        n = int(legend[len('macro'):])
        legend = 'Macro ' + str(n)
    elif legend.startswith('tapDance'):
        n = int(legend[len('tapDance'):])
        legend = 'Tap Dance ' + str(n)
    elif legend.startswith('toggle'):
        legend = legend[len('toggle'):]
        legend = legendmap(legend)
        legend += '<br>(Toggle)'
    elif ' || ' in legend:
        legend, backhalf = legend.split(' || ')
        legend = legendmap(legend)
        backhalf = legendmap(backhalf)
        legend = legend + '<br>' + backhalf
    elif legend.startswith('L ') or legend.startswith('R '):
        legend = legend[2:]
        legend = legendmap(legend)
    return legend

def lget(i, n):
    coord = tw(i, n)
    kid = newkeys.keymap[coord]
    name = pair_to_name[kid]
    return legendmap(name)

def kle_escape(s):
    specials = '/@_&;:='
    for c in specials:
        s = s.replace(c, '/' + c)
    return s

def kle_encode(obj):
    if type(obj) == int:
        return ':' + str(obj)
    elif type(obj) == float:
        return ':' + str(obj)
    elif type(obj) == bool:
        return ':' + str(obj).lower()
    elif type(obj) == str:
        return '=' + kle_escape(obj)
    elif type(obj) == list:
        return '@' + '&'.join(kle_encode(x) for x in obj) + ';'
    elif type(obj) == dict:
        return '_' + '&'.join(k + kle_encode(v) for k,v in obj.items()) + ';'


biglegends = []
for yy in range(5):
    legends = []
    i = (0,0,0)
    for n in range(72):
        legend = ['' for _ in range(10)]

        if n >= 66:
            if n == 66:
                i = (5,4,0)
            elif n == 67:
                i = (6,4,0)
            elif n == 68:
                i = (7,4,0)
            elif n == 69:
                i = (7,4,1)
            elif n == 70:
                i = (6,4,1)
            elif n == 71:
                i = (5,4,1)

        legend = lget(i, yy)
        legends.append(legend)

        if i[2] == 0:
            i = (i[0], i[1], 1)
        elif i[0] < 6 and i[1] < 4:
            i = (i[0] + 1, i[1], 0)
        elif i[0] < 4 and i[1] == 4:
            i = (i[0] + 1, i[1], 0)
        else:
            i = (0, i[1] + 1, 0)
    biglegends.append(legends)

jkeys = []

x = 0

for i,n in enumerate(newkeys.macros):
    attrib = {'w': 2, 'c': '#cc9999', 'a': 7, 'fa': [4]}
    jkeys.append([attrib, 'Macro ' + str(n)])
    attrib = {'c': '#cccccc', 'a': 7, 'x': 0.5, 'fa': [0]}
    jkeys[-1].append(attrib)
    for kid in newkeys.macros[n]:
        jkeys[-1].append(legendmap(pair_to_name[kid], withshift=False))

for i,n in enumerate(newkeys.tapdance):
    attrib = {'w': 2, 'c': '#cc99cc', 'a': 7, 'fa': [4]}
    if i == 0:
        attrib['y'] = 0.5
    if i % 3 == 0:
        jkeys.append([attrib, 'Tap Dance ' + str(n)])
    else:
        attrib['x'] = x + 0.5
        jkeys[-1].append(attrib)
        jkeys[-1].append('Tap Dance ' + str(n))
    attrib = {'c': '#cccccc', 'a': 7, 'x': 0.5, 'fa': [0]}
    jkeys[-1].append(attrib)
    for kid in newkeys.tapdance[n]:
        jkeys[-1].append(legendmap(kid))
    x = 3 - len(newkeys.tapdance[n])

offset = -7 + (len(newkeys.tapdance) + 1) // 3 + 1 + len(newkeys.macros) + 0.5

for yy,legends in enumerate(biglegends):
    offset += 7
    jkeys.append([])
    i = (0,0,0)
    for n,legend in enumerate(legends):
        attrib = {}
        attrib['c'] = '#cccccc'
        if i == (0, 0, 0):
            attrib['rx'] = 0
            attrib['ry'] = 0
            attrib['r'] = 0
            attrib['x'] = 0
            attrib['y'] = 0
        if i[2] == 0:
            if i[0] == 1:
                attrib['y'] = 0.75
            elif i[0] == 2:
                attrib['y'] = -0.50
            elif i[0] == 3:
                attrib['y'] = -0.25
            elif i[0] == 5:
                attrib['y'] = 0.25
            elif i[0] == 6:
                attrib['y'] = 0.25
        elif i[2] == 1:
            if i[0] == 5:
                attrib['y'] = -0.25
            elif i[0] == 4:
                attrib['y'] = -0.25
            elif i[0] == 2:
                attrib['y'] = 0.25
            elif i[0] == 1:
                attrib['y'] = 0.50
            elif i[0] == 0:
                attrib['y'] = -0.75

        if i[0] == 6 and i[1] < 4 and i[2] == 1:
            attrib['x'] = 4
        elif i[0] == 4 and i[1] == 4 and i[2] == 1:
            attrib['x'] = 8
            attrib['y'] = 0

        if n >= 66:
            jkeys.append([])
            if n == 66:
                i = (5,4,0)
                attrib['rx'] = 5.75
                attrib['ry'] = 5.00
                attrib['r'] = 20
                attrib['x'] = -0.25
                attrib['y'] = -0.25
                attrib['n'] = True
            elif n == 67:
                i = (6,4,0)
                attrib['r'] = 25
                attrib['x'] = 0.95
                attrib['y'] = -1.00
            elif n == 68:
                i = (7,4,0)
                attrib['r'] = 27
                attrib['x'] = 2.15
                attrib['y'] = -1.00
            elif n == 69:
                i = (7,4,1)
                attrib['rx'] = 12.50
                attrib['ry'] = 4.90
                attrib['r'] = -27
                attrib['x'] = -3.40
                attrib['y'] = -0.25
            elif n == 70:
                i = (6,4,1)
                attrib['r'] = -25
                attrib['x'] = -2.20
                attrib['y'] = -2.00
            elif n == 71:
                i = (5,4,1)
                attrib['r'] = -20
                attrib['x'] = -1.00
                attrib['y'] = -2.00
        if 'ry' in attrib:
            attrib['ry'] += offset

        if i[0] == 4 and i[1] == 2:
            attrib['n'] = True
        if i[0] == 5 and i[1] == 4:
            attrib['n'] = True

        # if files[['base', 'symbol', 'function', 'numpad', 'mouse'][yy]].keymap[tw(i, 0)].k == 300:
        if not legend:
            attrib['c'] = '#99cccc'
        elif yy > 0 and newkeys.keymap[tw(i, yy)] == newkeys.keymap[tw(i, 0)]:
            attrib['c'] = '#99cccc'
        elif legend.startswith('Tap Dance '):
            attrib['c'] = '#cc99cc'
        elif legend.startswith('Macro '):
            attrib['c'] = '#cc9999'

        attrib['a'] = 7
        if '<br>' in legend:
            attrib['a'] = 5
            mid,bot = legend.split('<br>', 1)
            legend = '\n' + bot + '\n\n\n\n\n' + mid

        if 'fa-angle-double-up' in legend:
            attrib['fa'] = [4,2]
        else:
            attrib['fa'] = [0]

        m = ((len(jkeys[-1]) + 2) // 4) * 2
        jkeys[-1].insert(m, legend)
        jkeys[-1].insert(m, attrib)

        if i[2] == 0:
            i = (i[0], i[1], 1)
        elif i[0] < 6 and i[1] < 4:
            i = (i[0] + 1, i[1], 0)
        elif i[0] < 4 and i[1] == 4:
            i = (i[0] + 1, i[1], 0)
        else:
            i = (0, i[1] + 1, 0)
            jkeys.append([])
    jkeys[-1].append({'rx': 8, 'ry': 2+offset, 'r': 0, 'w': 2, 'c': '#99cc99'})
    # jkeys[-1].append(['Base', 'Symbol', 'Function', 'Numpad', 'Mouse'][yy])
    jkeys[-1].append(f"Layer {yy+1}")

with open('generated-full.url', 'w') as f:
    f.write('www.keyboard-layout-editor.com/##' + urllib.parse.quote(kle_encode(jkeys)))
