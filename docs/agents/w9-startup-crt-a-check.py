"""Compare authored data with a private hash-pinned executable.

No original bytes or extracted files are written. The check reads the table of
name pairs at _4item$globalItem and compares it, in order, with the authored
source text. It also confirms the object extents and initial values used by the
other data units of this package. This grants no runtime or original-byte match.
"""
import argparse
from pathlib import Path
import re
import struct
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools'))
from baseline import inspect_elf
from symbol_inventory import inspect_symbols

HASH = '9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0'


def check(original):
    data = original.read_bytes()
    inventory = inspect_symbols(data, HASH)
    sections = inspect_elf(data)['sections']
    symbols = {s['name']: s for s in inventory['symbols'] if s['name']}

    def read(address, size):
        for section in sections:
            if section['type'] == 1 and section['address'] <= address < section['address'] + section['size']:
                offset = section['offset'] + address - section['address']
                return data[offset:offset + size]
        raise KeyError(hex(address))

    def text(address):
        raw = read(address, 96)
        return raw[:raw.index(0)].decode()

    sources = ROOT / 'experimental/sources'
    symbol = symbols['_4item$globalItem']
    assert symbol['size'] == 1696
    words = struct.unpack('<424I', read(symbol['value'], 1696))
    original_rows = [(text(words[i]), text(words[i + 1]))
                     for i in range(0, 424, 4) if words[i]]
    assert not any(words[420:424])
    source = (sources / 'w9-startup-crt-a-item-global-item/global_item.cpp').read_text()
    authored_rows = re.findall(r'^    \{"([^"]+)", "([^"]+)", 0u, 0u\},$', source, re.M)
    assert len(original_rows) == 105
    assert [tuple(r) for r in authored_rows] == original_rows
    for i in range(0, 424, 4):
        assert words[i + 2] == 0 and words[i + 3] == 0
    print('_4item$globalItem: 105 name pairs and the zero tail verified in order')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original', type=Path, required=True)
    check(parser.parse_args().original)
