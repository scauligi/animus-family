#!/usr/bin/env python3

import re
from pathlib import Path

import yaml

# from lxml import etree

# parser = etree.XMLParser(encoding='utf-8', load_dtd=True, remove_blank_text=True)

# _e = etree.parse(
#     '/home/scauligi/tmp/bb/home/scauligi/workspace/arbites/keygroup/Core.arb2kg', parser
# )
# kdata = {}
# krdata = {}
# for k in _e.find('Keys'):
#     kid = int(k.find('DisplayID').text)
#     ktype = int(k.find('KeyType').text)
#     kval = int(k.find('ValScan').text)
#     if (ktype, kval) in krdata:
#         print(ktype, kval, '->', krdata[(ktype, kval)], kid)
#         if kval == 116:
#             kval = 117
#     kdata[kid] = (ktype, kval)
#     krdata[(ktype, kval)] = kid

# _e = etree.parse(
#     '/home/scauligi/tmp/bb/home/scauligi/workspace/arbites/input-method/US-ANSI.arb2im',
#     parser,
# )
# klookup = {}
# krlookup = {}
# names = []
# for n in _e.find('Display'):
#     names.append(n.text)
# for n, ix in zip(names, _e.find('Index')):
#     i = int(ix.text)
#     klookup[n] = i
#     krlookup[i] = n

# with open('arbites.yaml', 'w') as f:
#     yaml.safe_dump(
#         {
#             'pair_to_name': {
#                 str(k): krlookup[v]
#                 for k, v in sorted((k, v) for k, v in krdata.items())
#             },
#             'name_to_pair': {k: str(kdata[v]) for k, v in klookup.items()},
#         },
#         f,
#         sort_keys=False,
#     )

with open('arbites.yaml') as f:
    data = yaml.safe_load(f)


def parse_tuple(t):
    if m := re.match(r'\((\d+), (\d+)\)', t):
        return (int(m[1]), int(m[2]))
    return t


pair_to_name = {parse_tuple(k): v for k, v in data['pair_to_name'].items()}

leftright = Path('leftright.hex').read_bytes()
left = leftright[:1024]
right = leftright[1024:]
nlayers = left[-1]
print('Layers: %d\n' % (nlayers,))
for w in range(nlayers):
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
                z = left
                if x >= 8:
                    x = 15 - x
                    z = right
                addr = (x + 8 * yy + 5 * 8 * w) * 2
                kval, ktype = z[addr : addr + 2]
                try:
                    n = pair_to_name[(ktype, kval)]
                except KeyError:
                    n = str((ktype, kval))
            print(n.center(14), end='', flush=True)
        print()
    print()
    print()

# print(len(set(kdata.values())))
# print(len(set(krdata.values())))
# print(len(set(klookup.values())))
# print(len(set(krlookup.values())))
