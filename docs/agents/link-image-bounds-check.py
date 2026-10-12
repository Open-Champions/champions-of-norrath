"""Compare authored data semantics with a private hash-pinned executable.

No original bytes or extracted files are written. Host contracts separately
check the compiled table slots. This grants no runtime or original-byte match.
"""
import argparse
from pathlib import Path
import re
import struct
import sys

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'tools'))
from baseline import inspect_elf
from symbol_inventory import inspect_symbols


def check(original):
    data = original.read_bytes()
    inventory = inspect_symbols(
        data, '9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0')
    sections = inspect_elf(data)['sections']
    symbols = {s['name']: s for s in inventory['symbols'] if s['name']}
    sources = ROOT / 'experimental/sources'
    for name, source, count in [
        ('__CTOR_LIST__', 'ctors/ctor_list.cpp', 101),
        ('__DTOR_LIST__', 'dtors/dtor_list.cpp', 11),
    ]:
        symbol = symbols[name]
        section = sections[symbol['section_index']]
        assert symbol['value'] == section['address']
        assert section['size'] == (count + 2) * 4
        words = struct.unpack_from('<' + 'I' * (count + 2), data, section['offset'])
        assert words[0] == 0xffffffff and words[-1] == 0
        source_text = (sources / ('link-image-bounds-' + source)).read_text()
        callbacks = re.findall(r'^    (_GLOBAL_[^,\s]+),$', source_text, re.M)
        assert len(callbacks) == count
        assert [symbols[n]['value'] for n in callbacks] == list(words[1:-1])
        assert all(symbols[n]['type'] == 2 for n in callbacks)
        print(name, count, 'callback names and order verified')
    for name in ['__sce_sema_id', '__sce_eh_sema_id']:
        symbol = symbols[name]
        section = sections[symbol['section_index']]
        offset = section['offset'] + symbol['value'] - section['address']
        assert symbol['size'] == 4 and section['type'] == 1
        assert data[offset:offset + 4] == bytes(4)
        print(name, 'four-byte zero initializer verified')
    assert symbols['__sce_sema_id']['value'] != symbols['__sce_eh_sema_id']['value']


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original', type=Path, required=True)
    check(parser.parse_args().original)
