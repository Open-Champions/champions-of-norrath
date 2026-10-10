#!/usr/bin/env python3
"""Norrath leaf reconstruction verifier: public record checks or fresh private comparison.

Adapted by review of the Return to Arms verification design, not its target data.
Only the pinned executable and complete, unambiguous ELF function symbols count.
Receipts record prior local comparisons; they are not authentication or runtime proof.
"""
import argparse
from datetime import datetime, timezone
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parent.parent
DEFAULT_LEDGER = ROOT / 'targets/SLUS-20565/rebuild.json'
TARGET = {'id': 'SLUS-20565-sha256-9a60f13c5c31',
          'baseline_sha256': '9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0',
          'elf_machine': 8, 'text': {'address': '0x00100000', 'size': 2209032}}
BASELINE_SIZE = 9249659
BASELINE_FLAGS = 0x20924001
BASELINE_ENTRY = 0x00100008
HASH = re.compile(r'[0-9a-f]{64}')
IDENT = re.compile(r'[A-Za-z_][A-Za-z0-9_]*')
SCOPE = 'recorded-complete-symbol-byte-comparison'
DIRECT_MODE = 'ee-direct-call-v1'
CALL_POLICY = {'mode': DIRECT_MODE, 'relocation': 'R_MIPS_26', 'addend': 0,
               'max_dependencies': 16, 'max_sites': 64, 'closure': 'fresh-source-dag',
               'control_flow': 'straight-line-integer-jal-terminal-jr-ra-v1'}
REQUIRED_FLAGS = {'--no-default-config', '--target=mipsel-none-elf', '-march=mips3', '-mabi=32',
                  '-ffreestanding', '-fno-pic', '-mno-abicalls', '-fno-stack-protector',
                  '-fno-exceptions', '-fno-rtti'}
ALLOWED_FLAGS = REQUIRED_FLAGS | {'-O0', '-O1', '-O2', '-O3', '-Os', '-Oz',
    '-fomit-frame-pointer', '-fno-omit-frame-pointer', '-fno-builtin',
    '-fno-unwind-tables', '-fno-asynchronous-unwind-tables', '-msoft-float',
    '-mhard-float', '-fno-strict-aliasing', '-fwrapv', '-std=c++17'}
EE_TARGET = 'mips64r5900el-ps2-elf'
EE_VERSION = '15.2.0'
EE_FLAGS = {'-std=c++17', '-O2', '-march=r5900', '-mabi=eabi', '-mgp64', '-mlong32',
            '-mfp32', '-msingle-float', '-ffreestanding', '-fno-pic', '-mno-abicalls',
            '-fno-stack-protector', '-fno-exceptions', '-fno-rtti', '-fwrapv',
            '-fno-unwind-tables', '-fno-asynchronous-unwind-tables', '-fomit-frame-pointer', '-G0'}
EE_MODEL = {'__SIZEOF_INT__': '4', '__SIZEOF_LONG__': '4', '__SIZEOF_POINTER__': '4',
            '__mips_eabi': '1', '__mips64': '1', '_MIPS_ARCH_R5900': '1',
            '__mips_fpr': '32', '__mips_single_float': '1'}
FIXED_FLAGS = ['-nostdinc', '-nostdinc++', '-x', 'c++']
INJECTING_ENV = ('CCC_OVERRIDE_OPTIONS', 'CL', '_CL_', 'CPATH', 'CPLUS_INCLUDE_PATH',
                 'C_INCLUDE_PATH', 'OBJC_INCLUDE_PATH', 'COMPILER_PATH', 'GCC_EXEC_PREFIX',
                 'LIBRARY_PATH', 'SDKROOT', 'DYLD_INSERT_LIBRARIES', 'LD_PRELOAD',
                 'GCC_SPECS', 'GCC_COMPARE_DEBUG', 'GCC_COLORS', 'LD_LIBRARY_PATH',
                 'DYLD_LIBRARY_PATH', 'DYLD_FRAMEWORK_PATH')


def require(condition, message):
    if not condition:
        raise ValueError(message)


def digest(data):
    return hashlib.sha256(data).hexdigest()


def canonical(value):
    return json.dumps(value, sort_keys=True, separators=(',', ':')).encode('utf-8')


def address(value):
    require(isinstance(value, str) and re.fullmatch(r'0x[0-9a-f]{8}', value), 'address must use 0x plus eight lowercase hexadecimal digits')
    return int(value, 16)


def local_path(root, relative):
    require(isinstance(relative, str) and relative and not Path(relative).is_absolute(), 'expected repository-relative path')
    path = (Path(root) / relative).resolve()
    require(path.is_relative_to(Path(root).resolve()), 'path escapes repository')
    return path


class Elf32:
    """A deliberately narrow ELF32/MIPS parser shared by original and object checks."""
    def __init__(self, data):
        self.data = data
        require(len(data) >= 52 and data[:7] == b'\x7fELF\x01\x01\x01', 'expected little-endian ELF32 version 1')
        fields = struct.unpack_from('<HHIIIIIHHHHHH', data, 16)
        self.kind, self.machine, version, self.entry, _, shoff, self.flags, ehsize, _, _, shsize, shnum, shstr = fields
        require(self.machine == 8 and version == 1 and ehsize == 52, 'expected standard MIPS ELF32 header')
        require(shoff >= 52 and shsize == 40 and 0 < shnum < 0xff00 and 0 < shstr < shnum, 'unsupported section table')
        self.span(shoff, shnum * shsize)
        self.sections = [struct.unpack_from('<10I', data, shoff + i * shsize) for i in range(shnum)]
        require(self.sections[shstr][1] == 3, 'section names require string table')
        names = self.section_data(shstr)
        self.names = [self.string(names, section[0]) for section in self.sections]
        for section in self.sections:
            if section[1] not in (0, 8):
                self.span(section[4], section[5])
        self.symbols = []
        for table, section in enumerate(self.sections):
            if section[1] != 2:
                continue
            require(section[9] == 16 and section[5] % 16 == 0, 'malformed symbol table')
            require(0 < section[6] < shnum and self.sections[section[6]][1] == 3, 'invalid symbol string table')
            strings = self.section_data(section[6])
            records = self.section_data(table)
            for offset in range(0, len(records), 16):
                name, value, size, info, other, index = struct.unpack_from('<IIIBBH', records, offset)
                self.symbols.append({'name': self.string(strings, name), 'value': value, 'size': size,
                                     'type': info & 15, 'binding': info >> 4, 'other': other, 'section': index,
                                     'table': table, 'index': offset // 16})

    def span(self, offset, size):
        require(0 <= offset <= len(self.data) and 0 <= size <= len(self.data) - offset, 'ELF range exceeds file')
        return self.data[offset:offset + size]

    def section_data(self, index):
        require(0 <= index < len(self.sections) and self.sections[index][1] != 8, 'unsupported section bytes')
        section = self.sections[index]
        return self.span(section[4], section[5])

    @staticmethod
    def string(table, offset):
        require(0 <= offset < len(table), 'string offset exceeds table')
        end = table.find(b'\0', offset)
        require(end >= 0, 'unterminated ELF string')
        return table[offset:end].decode('ascii')

    def named_section(self, name):
        matches = [i for i, current in enumerate(self.names) if current == name]
        require(len(matches) == 1, 'expected exactly one ' + name + ' section')
        return matches[0], self.sections[matches[0]]

    def function(self, name):
        matches = [s for s in self.symbols if s['name'] == name]
        require(len(matches) == 1, 'function name is missing or ambiguous')
        symbol = matches[0]
        index = symbol['section']
        require(symbol['type'] == 2 and 0 < index < len(self.sections), 'symbol must be a defined STT_FUNC')
        section = self.sections[index]
        require(section[1] == 1 and section[2] & 6 == 6, 'function must be in allocated executable PROGBITS')
        start = symbol['value'] - (section[3] if self.kind == 2 else 0)
        size = symbol['size']
        require(size > 0 and start >= 0 and start % 4 == 0 and size % 4 == 0 and start + size <= section[5], 'invalid function extent')
        return symbol, self.section_data(index)[start:start + size]

    def original(self, item):
        require(self.kind == 2, 'baseline must be executable')
        symbol, code = self.function(item['original_symbol'])
        index, section = self.named_section('.text')
        require(symbol['section'] == index and symbol['value'] == address(item['address'])
                and symbol['size'] == item['size'], 'candidate must cover the complete original symbol extent')
        lo, hi = symbol['value'], symbol['value'] + symbol['size']
        for other in self.symbols:
            if other is symbol or other['type'] != 2 or other['section'] != index:
                continue
            # Even an interior zero-sized label makes ownership ambiguous in this scope.
            require(not (lo <= other['value'] < hi or (other['size'] > 0 and other['value'] < hi
                        and lo < other['value'] + other['size'])), 'original function overlaps or aliases another symbol')
        return code

    def rebuilt(self, name, call_bindings=None):
        require(self.kind == 1, 'compiler must emit relocatable object')
        symbol, code = self.function(name)
        require(symbol['binding'] == 1 and symbol['other'] == 0, 'candidate must be one global default-visibility function')
        direct = call_bindings is not None
        functions = [s for s in self.symbols if s['type'] == 2 and (not direct or s['section'] != 0)]
        require(len(functions) == 1, 'extra object functions are unsupported')
        if direct:
            undefined = self.call_symbols(symbol, call_bindings)
        else:
            require(not any(s['section'] == 0 and s['name'] for s in self.symbols), 'undefined object symbols are unsupported')
        records = []
        for i, section in enumerate(self.sections):
            if section[1] in (4, 9) and section[5]:
                if direct and self.names[i] == '.rel.text':
                    require(section[1] == 9 and section[2] == 0x40 and section[3] == 0
                            and section[9] == 8 and section[5] % 8 == 0
                            and section[5] <= CALL_POLICY['max_sites'] * 8
                            and section[6] == symbol['table'] and section[7] == symbol['section']
                            and section[8] == 4, 'invalid direct-call relocation table')
                    for offset, info in struct.iter_unpack('<II', self.section_data(i)):
                        require(info & 255 == 4 and info >> 8 in undefined, 'unbound or unsupported call relocation')
                        records.append({'offset': offset, 'type': 4, 'symbol': undefined[info >> 8]})
                    continue
                # Clang's procedure-descriptor relocation is non-runtime metadata.
                # Permit only its exact shape, never arbitrary nonallocated relocations.
                target_index = section[7]
                require(section[1] == 9 and self.names[i] == '.rel.pdr'
                        and section[5] == 8 and section[9] == 8 and section[6] == symbol['table']
                        and 0 < target_index < len(self.sections), 'object contains unsupported relocations')
                target = self.sections[target_index]
                require(self.names[target_index] == '.pdr' and target[1] == 1
                        and target[2] == 0 and target[5] == 32, 'relocations must target nonallocated procedure metadata')
                offset, info = struct.unpack('<II', self.section_data(i))
                require(offset == 0 and info == (symbol['index'] << 8 | 2), 'unsupported procedure metadata relocation')
            if section[2] & 2 and section[5] and i != symbol['section']:
                # These two MIPS metadata records contain ABI declarations, not runtime data.
                require((self.names[i], section[1], section[2], section[5]) in
                        (('.reginfo', 0x70000006, 2, 24), ('.MIPS.abiflags', 0x7000002a, 2, 24)),
                        'extra allocated object data is unsupported')
            if section[2] & 4 and section[5]:
                require(i == symbol['section'], 'extra executable object section is unsupported')
        section = self.sections[symbol['section']]
        require(symbol['value'] == 0, 'candidate must begin its object code section')
        # Clang rounds .text up to its alignment; accept only zero padding after the symbol.
        padding = self.section_data(symbol['section'])[len(code):]
        require(len(padding) < max(section[8], 1) and not any(padding), 'extra object code outside candidate')
        if direct:
            expected = sorted((offset, b['object_symbol']) for b in call_bindings for offset in b['call_offsets'])
            require(sorted((r['offset'], r['symbol']) for r in records) == expected,
                    'call relocation sites differ from declared bindings')
            require(self.names[symbol['section']] == '.text' and section[2] == 6,
                    'direct-call body requires ordinary .text')
            return code, records
        return code

    def call_symbols(self, candidate, bindings):
        require(len(set(self.names)) == len(self.names), 'duplicate object section names')
        require(sum(s[1] == 2 for s in self.sections) == 1
                and not any(s[1] == 11 for s in self.sections), 'direct calls require one symbol table')
        table = self.sections[candidate['table']]
        require(table[2] == 0 and table[3] == 0 and table[8] == 4,
                'invalid direct-call symbol table')
        first_global = min(s['index'] for s in self.symbols if s['binding'] != 0)
        # Pinned EE binutils uses the MIPS SGI split (elf32-mips.c,
        # mips_elf_sym_is_global): a local FILE can follow the section-symbol
        # boundary. Accept only that one precisely validated metadata position.
        boundary = table[7]
        preceding_file = self.symbols[first_global - 1]
        file_split = (boundary == first_global - 1 and preceding_file['type'] == 4
                      and preceding_file['section'] == 0xfff1 and preceding_file['binding'] == 0
                      and preceding_file['other'] == preceding_file['value'] == preceding_file['size'] == 0)
        require(0 < boundary and (boundary == first_global or file_split)
                and all((s['binding'] == 0) == (s['index'] < first_global) for s in self.symbols),
                'invalid local/global symbol boundary')
        undefined = {}
        for s in self.symbols:
            if s['index'] == 0:
                require(all(s[k] == 0 for k in ('value', 'size', 'type', 'binding', 'other', 'section'))
                        and s['name'] == '', 'invalid null object symbol')
            elif s is candidate:
                continue
            elif s['section'] == 0:
                require(s['name'] and s['binding'] == 1 and s['other'] == 0 and s['type'] in (0, 2)
                        and s['value'] == s['size'] == 0, 'unsupported undefined call symbol')
                undefined[s['index']] = s['name']
            else:
                require(s['binding'] == 0 and s['other'] == 0 and s['value'] == s['size'] == 0
                        and ((s['type'] == 3 and 0 < s['section'] < len(self.sections)
                              and s['name'] in ('', self.names[s['section']]))
                             or (s['type'] == 4 and s['section'] == 0xfff1)), 'extra object symbol is unsupported')
        require(len(set(undefined.values())) == len(undefined)
                and set(undefined.values()) == {b['object_symbol'] for b in bindings},
                'undefined symbols differ from source-backed bindings')
        return undefined


def source_bytes(path):
    data = Path(path).read_bytes()
    text = data.decode('utf-8-sig')
    require('??' not in text and '\\' not in text, 'escape and trigraph syntax is outside initial source scope')
    # Conservative raw-text policy: comment-looking bytes inside strings must never
    # hide a real directive. This may reject harmless prose in comments by design.
    require('#' not in text and '%:' not in text, 'source must have no preprocessor directive tokens')
    require(not re.search(r'\b(?:asm|__asm|__asm__|__attribute__|__declspec|__builtin\w*|incbin)\b', text), 'assembly, compiler extensions and embedded data are unsupported')
    require(not re.search(r'\b(?:import|module)\b', text), 'external source dependencies are unsupported')
    return data


def call_graph(ledger):
    """Validate source-backed edges and return deterministic dependency-first order."""
    items = {item['id']: item for item in ledger['functions']}
    edges, members = {}, set()
    for item in items.values():
        mode = item.get('comparison_mode')
        require(mode in (None, DIRECT_MODE), 'unsupported comparison mode')
        if mode is None:
            require('call_bindings' not in item, 'leaf entries cannot declare call bindings')
            edges[item['id']] = set()
            continue
        require(item.get('toolchain') == 'ee-gcc', 'direct calls require reviewed EE GCC')
        bindings = item.get('call_bindings')
        require(isinstance(bindings, list) and 0 < len(bindings) <= CALL_POLICY['max_dependencies'],
                'direct calls need bounded source-backed dependencies')
        callees, symbols, addresses, offsets = set(), set(), set(), set()
        for binding in bindings:
            require(isinstance(binding, dict) and set(binding) == {'callee_id', 'object_symbol', 'original_symbol',
                    'address', 'size', 'body_sha256', 'call_offsets'}, 'invalid call binding fields')
            callee_id = binding['callee_id']
            require(isinstance(callee_id, str) and callee_id in items and callee_id not in callees
                    and callee_id != item['id'], 'missing, duplicate or recursive source dependency')
            callee = items[callee_id]
            require(callee.get('toolchain') == 'ee-gcc' and callee['compiler_args'] == item['compiler_args'],
                    'caller and dependency must use identical reviewed EE options')
            require(binding['object_symbol'] == callee['symbol'] and binding['original_symbol'] == callee['original_symbol']
                    and binding['address'] == callee['address'] and type(binding['size']) is int
                    and binding['size'] == callee['size'], 'dependency binding differs from source ledger entry')
            require(binding['object_symbol'] not in symbols and binding['address'] not in addresses,
                    'aliased dependency bindings are unsupported')
            require(isinstance(binding['body_sha256'], str) and HASH.fullmatch(binding['body_sha256']),
                    'dependency needs complete original body hash')
            sites = binding['call_offsets']
            require(isinstance(sites, list) and sites and all(type(o) is int and o >= 0 and o % 4 == 0
                    and o + 8 <= item['size'] for o in sites), 'invalid complete callsite bounds')
            require(sites == sorted(set(sites)) and not offsets.intersection(sites), 'duplicate or unsorted callsites')
            callees.add(callee_id); symbols.add(binding['object_symbol']); addresses.add(binding['address']); offsets.update(sites)
        require(len(offsets) <= CALL_POLICY['max_sites'] and not any(o + 4 in offsets for o in offsets),
                'too many or adjacent callsites')
        edges[item['id']] = callees
        members.update(callees | {item['id']})
    # Iterative traversal avoids Python recursion limits on malformed deep graphs.
    pending = {key: set(value) for key, value in edges.items()}
    order = []
    while pending:
        ready = [key for key, dependencies in pending.items() if not dependencies]
        require(ready, 'call dependency graph contains a cycle')
        order.extend(ready)
        for key in ready:
            del pending[key]
        for dependencies in pending.values():
            dependencies.difference_update(ready)
    return [items[key] for key in order], members


def audit_integer_body(code, call_offsets=()):
    """Closed, fixed-width R5900 subset: integer instructions, declared JAL, final JR RA.

    No branches, indirect calls, COP/MMI encodings, traps or unknown instructions.
    Reserved fields are checked instead of accepting whole SPECIAL opcode groups.
    This checks control-flow shape, not memory safety or the source ABI contract.
    """
    require(len(code) >= 8 and len(code) % 4 == 0, 'invalid audited body extent')
    calls = set(call_offsets)
    require(len(calls) == len(call_offsets), 'duplicate audited callsite')
    words = struct.unpack('<' + 'I' * (len(code) // 4), code)
    require(words[-2] == 0x03e00008, 'audited body needs terminal JR RA and delay slot')
    for index, word in enumerate(words):
        offset = index * 4
        op, rs, rt, rd, sa, fn = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31, (word >> 6) & 31, word & 63
        if offset in calls:
            require(op == 3 and offset + 8 <= len(code) and offset != len(code) - 8
                    and offset - 4 not in calls, 'invalid audited JAL or delay slot')
            continue
        if offset == len(code) - 8:
            require(offset - 4 not in calls, 'return cannot occupy a call delay slot')
            continue
        allowed = False
        destination = None
        if op == 0:
            if fn in (0, 2, 3, 0x38, 0x3a, 0x3b, 0x3c, 0x3e, 0x3f) and rs == 0:
                allowed, destination = True, rd  # fixed shifts, including zero-word NOP
            elif fn in (4, 6, 7, 0x14, 0x16, 0x17, 0x21, 0x23, 0x24, 0x25, 0x26, 0x27, 0x2a, 0x2b, 0x2d, 0x2f) and sa == 0:
                allowed, destination = True, rd  # variable shifts and nontrapping integer ALU
        elif op in (9, 10, 11, 12, 13, 14, 25):
            allowed, destination = True, rt
        elif op == 15 and rs == 0:
            allowed, destination = True, rt  # LUI
        elif op in (32, 33, 35, 36, 37, 39, 55):
            allowed, destination = True, rt  # LB/LH/LW/LBU/LHU/LWU/LD
        elif op in (40, 41, 43, 63):
            allowed = True  # SB/SH/SW/SD
        require(allowed, 'instruction outside closed integer/control-flow policy')
        require(destination != 31 or (op == 55 and rs == 29), 'unsupported write to return-address register')
    require(all(type(o) is int and o >= 0 and o % 4 == 0 and o + 8 <= len(code) for o in calls),
            'audited callsite outside body')


def callsite_evidence(item):
    evidence = []
    origin = address(item['address'])
    require(origin + item['size'] < 2**32, 'caller address wraps')
    for binding in item['call_bindings']:
        target = address(binding['address'])
        require(target % 4 == 0 and target + binding['size'] < 2**32
                and not origin <= target < origin + item['size'], 'invalid dependency address')
        for offset in binding['call_offsets']:
            pc = origin + offset
            require(target >> 28 == (pc + 4) >> 28, 'call target outside PC+4 jump region')
            word = 0x0c000000 | ((target >> 2) & 0x03ffffff)
            require(((pc + 4) & 0xf0000000) | ((word & 0x03ffffff) << 2) == target, 'call target round-trip failed')
            evidence.append({'offset': offset, 'address': f'0x{pc:08x}', 'type': 4, 'addend': 0,
                             'raw_word': '0x0c000000', 'relocated_word': f'0x{word:08x}',
                             'object_symbol': binding['object_symbol'], 'callee_id': binding['callee_id'],
                             'original_symbol': binding['original_symbol'], 'target_address': binding['address'],
                             'target_size': binding['size'], 'target_sha256': binding['body_sha256']})
    return sorted(evidence, key=lambda row: row['offset'])


def validate_original_calls(original, item):
    evidence = callsite_evidence(item)
    audit_integer_body(original, [row['offset'] for row in evidence])
    for row in evidence:
        word, = struct.unpack_from('<I', original, row['offset'])
        require(word == int(row['relocated_word'], 16), 'original JAL target differs from source-backed binding')
    return evidence


def relocate_calls(code, original, item):
    require(len(code) == len(original) == item['size'], 'call comparison requires complete equal extents')
    evidence = validate_original_calls(original, item)
    audit_integer_body(code, [row['offset'] for row in evidence])
    relocated = bytearray(code)
    for row in evidence:
        word, = struct.unpack_from('<I', code, row['offset'])
        require(word == 0x0c000000, 'call relocation requires zero-addend ordinary JAL')
        struct.pack_into('<I', relocated, row['offset'], int(row['relocated_word'], 16))
    return bytes(relocated), evidence


def original_graph_body(baseline, item):
    symbol, code = baseline.function(item['original_symbol'])
    require(symbol['binding'] == 1 and symbol['other'] == 0, 'call graph requires unambiguous global original functions')
    # Verify the exact file-backed executable load mapping, independently of sections.
    phoff, = struct.unpack_from('<I', baseline.data, 28)
    phsize, phnum = struct.unpack_from('<HH', baseline.data, 42)
    require(phsize == 32 and 0 < phnum < 0xffff, 'call graph requires ordinary load headers')
    headers = baseline.span(phoff, phnum * phsize)
    section = baseline.sections[symbol['section']]
    offset = section[4] + symbol['value'] - section[3]
    mappings = []
    for typ, file_offset, virtual, _, filesz, memsz, flags, _ in struct.iter_unpack('<8I', headers):
        if typ == 1 and virtual <= symbol['value'] and symbol['value'] + len(code) <= virtual + filesz:
            baseline.span(file_offset, filesz)
            if flags & 1 and filesz <= memsz and virtual + memsz <= 2**32:
                mappings.append(file_offset + symbol['value'] - virtual)
    require(mappings == [offset], 'original function lacks unique executable load mapping')
    return code


def validate_ledger(ledger, root=ROOT):
    require(isinstance(ledger, dict) and ledger.get('schema_version') == 1, 'unsupported ledger schema')
    require(ledger.get('target') == TARGET, 'ledger must pin the reviewed Norrath baseline identity and .text bounds')
    items = ledger.get('functions')
    require(isinstance(items, list) and len(items) > 0, 'ledger needs complete authored function entries')
    ids, sources, extents = set(), set(), []
    for item in items:
        require(isinstance(item, dict), 'invalid function entry')
        ident, symbol = item['id'], item['symbol']
        require(isinstance(ident, str) and IDENT.fullmatch(ident) and ident not in ids, 'invalid or duplicate function id')
        require(isinstance(symbol, str) and IDENT.fullmatch(symbol), 'rebuilt symbol must be an extern C identifier')
        require(isinstance(item['original_symbol'], str) and re.fullmatch(r'[A-Za-z_.$][A-Za-z0-9_.$]*', item['original_symbol']), 'unsupported original symbol name')
        lo, size = address(item['address']), item['size']
        require(type(size) is int and size > 0 and lo % 4 == 0 and size % 4 == 0, 'invalid complete function extent')
        base = address(TARGET['text']['address'])
        require(base <= lo < lo + size <= base + TARGET['text']['size'], 'candidate is outside pinned .text')
        require(item['language'] == 'c++', 'only authored C++ is credited')
        source = local_path(root, item['source'])
        require(source.suffix == '.cpp' and source not in sources, 'each candidate requires a distinct C++ source')
        source_bytes(source)
        flags = item['compiler_args']
        require(isinstance(flags, list) and all(isinstance(f, str) for f in flags), 'compiler arguments must be strings')
        flavor = item.get('toolchain', 'clang')
        require(flavor in ('clang', 'ee-gcc'), 'unsupported toolchain flavor')
        required, allowed = (REQUIRED_FLAGS, ALLOWED_FLAGS) if flavor == 'clang' else (EE_FLAGS, EE_FLAGS)
        require(len(flags) == len(set(flags)) and required <= set(flags) <= allowed, 'unsupported or missing compiler flags')
        require(sum(f.startswith('-O') for f in flags) == 1, 'exactly one optimization setting is required')
        require(not ({'-msoft-float', '-mhard-float'} <= set(flags)) and not ({'-fomit-frame-pointer', '-fno-omit-frame-pointer'} <= set(flags)), 'conflicting compiler arguments')
        ids.add(ident)
        sources.add(source)
        extents.append((lo, lo + size))
    extents.sort()
    require(all(a[1] <= b[0] for a, b in zip(extents, extents[1:])), 'ledger function extents overlap')
    call_graph(ledger)
    return ledger


def load_ledger(path=DEFAULT_LEDGER, root=ROOT):
    return validate_ledger(json.loads(Path(path).read_text(encoding='utf-8')), root)


def baseline_elf(path):
    data = Path(path).read_bytes()
    require(digest(data) == TARGET['baseline_sha256'] and len(data) == BASELINE_SIZE, 'baseline SHA-256 or size mismatch; no compilation performed')
    elf = Elf32(data)
    require(elf.kind == 2 and elf.flags == BASELINE_FLAGS and elf.entry == BASELINE_ENTRY, 'baseline ELF identity mismatch')
    _, section = elf.named_section('.text')
    require(section[3] == address(TARGET['text']['address']) and section[5] == TARGET['text']['size'], 'baseline .text bounds mismatch')
    return elf


def effective_args(item):
    return item['compiler_args'] + FIXED_FLAGS + ['-c', '<source-snapshot.cpp>', '-o', '<fresh-object.o>']


def compiler_output(command, env, **kwargs):
    result = subprocess.run(command, capture_output=True, timeout=15, check=False, env=env, **kwargs)
    require(result.returncode == 0, 'compiler identity probe failed')
    return result.stdout


def toolchain_identity(flavor, compiler, env):
    require(compiler is not None, 'missing compiler for toolchain ' + flavor)
    executable = shutil.which(str(compiler))
    require(executable is not None, 'compiler not found for toolchain ' + flavor)
    executable = Path(executable).resolve()
    driver_hash = digest(executable.read_bytes())
    if flavor == 'clang':
        version = compiler_output([str(executable), '--no-default-config', '--version'], env).decode('utf-8').splitlines()
        require(version and re.fullmatch(r'(?:(?:Apple|Homebrew) )?clang version [A-Za-z0-9. +-]+', version[0]), 'compiler identity must be a sanitized Clang version')
        return executable, {'flavor': flavor, 'name': executable.name, 'sha256': driver_hash, 'version': version[0]}
    require(flavor == 'ee-gcc' and executable.name == EE_TARGET + '-g++'
            and executable.parent.name == 'bin', 'EE GCC requires the reviewed target g++ driver layout')
    prefix = executable.parent.parent
    version = compiler_output([str(executable), '--version'], env).decode('ascii').splitlines()
    require(version and version[0] == EE_TARGET + '-g++ (GCC) ' + EE_VERSION, 'unsupported EE GCC version')
    require(compiler_output([str(executable), '-dumpmachine'], env).decode('ascii').strip() == EE_TARGET, 'EE GCC target mismatch')
    # GCC can auto-load a specs file; built-in -dumpspecs alone does not bind it.
    require(compiler_output([str(executable), '-print-file-name=specs'], env).decode('utf-8').strip() == 'specs',
            'external GCC specs files are unsupported')
    components = {}
    expected = {'cc1plus': prefix / 'libexec/gcc' / EE_TARGET / EE_VERSION / 'cc1plus',
                'as': prefix / EE_TARGET / 'bin/as'}
    for name, expected_path in expected.items():
        selected = compiler_output([str(executable), '-print-prog-name=' + name], env).decode('utf-8').strip()
        selected = Path(selected)
        require(selected.is_absolute() and selected.resolve().is_relative_to(prefix)
                and selected.resolve() == expected_path.resolve(), 'EE GCC component escapes its reviewed install layout')
        components[name] = {'name': name, 'sha256': digest(selected.read_bytes())}
    specs_hash = digest(compiler_output([str(executable), '-dumpspecs'], env))
    require(digest(executable.read_bytes()) == driver_hash, 'compiler changed during identity probes')
    return executable, {'flavor': flavor, 'name': executable.name, 'sha256': driver_hash, 'version': version[0],
                        'target': EE_TARGET, 'components': components, 'specs_sha256': specs_hash}


def ee_data_model(executable, flags, env):
    output = compiler_output([str(executable)] + flags + FIXED_FLAGS + ['-dM', '-E', '-'], env, input=b'')
    macros = {}
    for line in output.decode('ascii').splitlines():
        parts = line.split(maxsplit=2)
        if len(parts) == 3 and parts[0] == '#define' and parts[1] in EE_MODEL:
            require(parts[1] not in macros, 'duplicate EE data-model declaration')
            macros[parts[1]] = parts[2]
    require(macros == EE_MODEL, 'EE GCC data model does not match reviewed R5900/EABI leaf scope')
    return macros


def verify(ledger, game, compiler, output_dir, root=ROOT, ee_compiler=None):
    validate_ledger(ledger, root)
    ordered, members = call_graph(ledger)
    baseline = baseline_elf(game)
    originals = {item['id']: baseline.original(item) for item in ledger['functions']}
    for item in ordered:
        if item['id'] in members:
            original_graph_body(baseline, item)
            if item.get('comparison_mode') == DIRECT_MODE:
                validate_original_calls(originals[item['id']], item)
                for binding in item['call_bindings']:
                    require(digest(originals[binding['callee_id']]) == binding['body_sha256'],
                            'dependency original body hash mismatch')
            else:
                audit_integer_body(originals[item['id']])
    for name in INJECTING_ENV:
        require(not os.environ.get(name), 'unset ' + name + '; unrecorded compiler environment injection is unsupported')
    env = {'PATH': os.defpath, 'LANG': 'C', 'LC_ALL': 'C'}
    flavors = {item.get('toolchain', 'clang') for item in ledger['functions']}
    selected = {'clang': compiler, 'ee-gcc': ee_compiler}
    toolchains = {flavor: toolchain_identity(flavor, selected[flavor], env) for flavor in sorted(flavors)}
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    report = {'schema_version': 3 if members else 2, 'scope': SCOPE, 'target': TARGET['id'], 'baseline_sha256': TARGET['baseline_sha256'],
              'ledger_sha256': digest(canonical(ledger)), 'verifier_sha256': digest(Path(__file__).read_bytes()),
              'recorded_at': datetime.now(timezone.utc).isoformat(),
              'toolchains': {flavor: value[1] for flavor, value in toolchains.items()}, 'functions': []}
    if members:
        report['call_policy'] = CALL_POLICY
        report['call_policy_sha256'] = digest(canonical(CALL_POLICY))
    snapshots = {item['id']: source_bytes(local_path(root, item['source'])) for item in ordered}
    completed = {}
    for item in ordered:
        flavor = item.get('toolchain', 'clang')
        executable, identity = toolchains[flavor]
        source = local_path(root, item['source'])
        contents = snapshots[item['id']]
        result = {'id': item['id'], 'source_sha256': digest(contents), 'compiler_args': effective_args(item),
                  'toolchain': flavor, 'toolchain_sha256': digest(canonical(identity)), 'status': 'failed'}
        with tempfile.TemporaryDirectory(prefix=item['id'] + '-', dir=output_dir) as directory:
            directory = Path(directory)
            snapshot, obj = directory / 'source.cpp', directory / 'fresh.o'
            snapshot.write_bytes(contents)
            command = [str(executable)] + item['compiler_args'] + FIXED_FLAGS + ['-c', str(snapshot.resolve()), '-o', str(obj.resolve())]
            try:
                if item.get('comparison_mode') == DIRECT_MODE:
                    for binding in item['call_bindings']:
                        callee = completed[binding['callee_id']]
                        require(callee['status'] == 'matched' and callee['original_sha256'] == binding['body_sha256']
                                and callee['rebuilt_sha256'] == binding['body_sha256'],
                                'source-backed dependency did not freshly match')
                if flavor == 'ee-gcc':
                    require(toolchain_identity(flavor, executable, env)[1] == identity, 'EE GCC components changed before compilation')
                    result['data_model'] = ee_data_model(executable, item['compiler_args'], env)
                compiled = subprocess.run(command, cwd=directory, capture_output=True, timeout=120, check=False, env=env)
                require(source.read_bytes() == contents and snapshot.read_bytes() == contents, 'source changed during compilation')
                require(digest(executable.read_bytes()) == identity['sha256'], 'compiler changed during compilation')
                if flavor == 'ee-gcc':
                    require(toolchain_identity(flavor, executable, env)[1] == identity, 'EE GCC components or specs changed during compilation')
                (output_dir / (item['id'] + '.log')).write_bytes(compiled.stdout + compiled.stderr)
                require(compiled.returncode == 0, 'compilation failed; inspect private build log')
                object_data = obj.read_bytes()
                elf = Elf32(object_data)
                if flavor == 'ee-gcc':
                    require(elf.flags == BASELINE_FLAGS, 'EE GCC object flags differ from reviewed R5900/EABI64 target')
                direct = item.get('comparison_mode') == DIRECT_MODE
                if direct:
                    raw, _ = elf.rebuilt(item['symbol'], item['call_bindings'])
                    code, evidence = relocate_calls(raw, originals[item['id']], item)
                    result.update({'comparison_mode': DIRECT_MODE, 'link_address': item['address'],
                                   'raw_rebuilt_sha256': digest(raw), 'relocations': evidence,
                                   'dependencies': {b['callee_id']: digest(canonical(completed[b['callee_id']]))
                                                    for b in item['call_bindings']}})
                else:
                    code = elf.rebuilt(item['symbol'])
                    if item['id'] in members:
                        audit_integer_body(code)
                if item['id'] in members:
                    result['control_flow_policy'] = DIRECT_MODE
                require(len(code) == item['size'], 'rebuilt extent does not match the complete original function')
                original = originals[item['id']]
                result.update({'object_sha256': digest(object_data), 'size': len(code),
                               'original_sha256': digest(original), 'rebuilt_sha256': digest(code),
                               'status': 'matched' if code == original else 'mismatched'})
            except (OSError, ValueError, subprocess.TimeoutExpired) as exc:
                result['reason'] = str(exc) if isinstance(exc, ValueError) else type(exc).__name__
        completed[item['id']] = result
    if members:
        # Dependency snapshots and compiler components must remain stable for the
        # entire graph, including after the dependency's own compilation ended.
        stable = all(source_bytes(local_path(root, item['source'])) == snapshots[item['id']]
                     for item in ordered if item['id'] in members)
        stable = stable and toolchain_identity('ee-gcc', toolchains['ee-gcc'][0], env)[1] == toolchains['ee-gcc'][1]
        if not stable:
            for key in members:
                completed[key].update(status='failed', reason='source or compiler changed during graph verification')
    report['functions'] = [completed[item['id']] for item in ledger['functions']]
    return report


def validate_identity(identity, flavor):
    require(isinstance(identity, dict) and isinstance(identity.get('name'), str)
            and re.fullmatch(r'[A-Za-z0-9_.+-]+', identity['name'])
            and isinstance(identity.get('sha256'), str) and HASH.fullmatch(identity['sha256'])
            and identity.get('flavor') == flavor, 'invalid recorded compiler identity')
    if flavor == 'clang':
        require(isinstance(identity.get('version'), str)
                and re.fullmatch(r'(?:(?:Apple|Homebrew) )?clang version [A-Za-z0-9. +-]+', identity['version']), 'invalid recorded Clang version')
    else:
        require(flavor == 'ee-gcc' and identity.get('target') == EE_TARGET
                and identity.get('name') == EE_TARGET + '-g++'
                and identity.get('version') == EE_TARGET + '-g++ (GCC) ' + EE_VERSION,
                'invalid recorded EE GCC target/version')
        components = identity.get('components')
        require(isinstance(components, dict) and set(components) == {'cc1plus', 'as'}, 'missing EE GCC components')
        for name, component in components.items():
            require(isinstance(component, dict) and component.get('name') == name
                    and isinstance(component.get('sha256'), str) and HASH.fullmatch(component['sha256']), 'invalid recorded EE GCC component identity')
        require(isinstance(identity.get('specs_sha256'), str) and HASH.fullmatch(identity['specs_sha256']), 'missing EE GCC specs identity')


def recorded_matches(report, ledger, root=ROOT):
    validate_ledger(ledger, root)
    ordered, members = call_graph(ledger)
    require(report.get('schema_version') in (1, 2, 3) and report.get('scope') == SCOPE, 'unsupported comparison receipt')
    if members:
        require(report['schema_version'] == 3 and report.get('call_policy') == CALL_POLICY
                and report.get('call_policy_sha256') == digest(canonical(CALL_POLICY)), 'missing direct-call receipt policy')
    require(report.get('target') == TARGET['id'] and report.get('baseline_sha256') == TARGET['baseline_sha256'], 'receipt targets different baseline')
    require(report.get('ledger_sha256') == digest(canonical(ledger)), 'stale receipt: ledger changed')
    require(report.get('verifier_sha256') == digest(Path(__file__).read_bytes()), 'stale receipt: verifier changed')
    flavors = {item.get('toolchain', 'clang') for item in ledger['functions']}
    if report['schema_version'] == 1:
        require(flavors == {'clang'}, 'legacy receipts cannot claim EE GCC comparisons')
        identity = dict(report.get('compiler', {}))
        identity['flavor'] = 'clang'
        identities = {'clang': identity}
    else:
        identities = report.get('toolchains')
        require(isinstance(identities, dict) and set(identities) == flavors, 'receipt toolchain inventory mismatch')
    for flavor, identity in identities.items():
        validate_identity(identity, flavor)
    require(isinstance(report.get('recorded_at'), str), 'missing comparison time')
    datetime.fromisoformat(report['recorded_at'])
    results = report.get('functions')
    require(isinstance(results, list) and len(results) == len(ledger['functions']), 'missing or extra function receipt')
    matched = []
    for result, item in zip(results, ledger['functions']):
        require(result.get('id') == item['id'], 'receipt function identity mismatch')
        flavor = item.get('toolchain', 'clang')
        if report['schema_version'] >= 2:
            require(result.get('toolchain') == flavor
                    and result.get('toolchain_sha256') == digest(canonical(identities[flavor])), 'receipt toolchain identity mismatch')
        require(result.get('source_sha256') == digest(source_bytes(local_path(root, item['source'])))
                and result.get('compiler_args') == effective_args(item), 'stale receipt: source or compiler args changed')
        require(result.get('status') in ('matched', 'mismatched', 'failed'), 'invalid comparison status')
        if result['status'] == 'matched':
            if flavor == 'ee-gcc':
                require(result.get('data_model') == EE_MODEL, 'missing recorded EE GCC data-model evidence')
            require(result.get('size') == item['size'] and all(isinstance(result.get(key), str) and HASH.fullmatch(result[key])
                    for key in ('object_sha256', 'original_sha256', 'rebuilt_sha256'))
                    and result['original_sha256'] == result['rebuilt_sha256'], 'inconsistent match receipt')
            matched.append(item)
    by_id = {result['id']: result for result in results}
    for item in ordered:
        result = by_id[item['id']]
        if result['status'] != 'matched':
            continue
        if item['id'] in members:
            require(result.get('control_flow_policy') == DIRECT_MODE, 'missing graph control-flow audit')
        if item.get('comparison_mode') == DIRECT_MODE:
            require(result.get('comparison_mode') == DIRECT_MODE and result.get('link_address') == item['address']
                    and isinstance(result.get('raw_rebuilt_sha256'), str) and HASH.fullmatch(result['raw_rebuilt_sha256'])
                    and result.get('relocations') == callsite_evidence(item), 'inconsistent direct-call comparison evidence')
            dependencies = {}
            for binding in item['call_bindings']:
                callee = by_id[binding['callee_id']]
                require(callee['status'] == 'matched' and callee.get('original_sha256') == binding['body_sha256']
                        and callee.get('rebuilt_sha256') == binding['body_sha256'], 'receipt dependency is not source-matched')
                dependencies[binding['callee_id']] = digest(canonical(callee))
            require(result.get('dependencies') == dependencies, 'stale or missing dependency result binding')
        else:
            require('comparison_mode' not in result and 'relocations' not in result and 'dependencies' not in result,
                    'leaf receipt cannot claim relocated comparison')
    return matched


def invalidate_report(destination, ledger_path, ledger, game, compiler, root=ROOT, ee_compiler=None):
    destination = Path(destination)
    require(destination.suffix == '.json' and not destination.is_symlink(), 'receipt must be a regular JSON path')
    protected = {Path(ledger_path).resolve(), Path(game).resolve(), Path(__file__).resolve()}
    for selected in (compiler, ee_compiler):
        resolved_compiler = shutil.which(str(selected)) if selected else None
        if resolved_compiler:
            protected.add(Path(resolved_compiler).resolve())
    protected.update(local_path(root, item['source']) for item in ledger['functions'])
    require(destination.resolve() not in protected, 'receipt path collides with a protected input')
    if destination.exists():
        previous = json.loads(destination.read_text(encoding='utf-8'))
        require(isinstance(previous, dict) and previous.get('schema_version') in (1, 2, 3)
                and previous.get('scope') == SCOPE, 'refusing to replace a file that is not a comparison receipt')
        destination.unlink()


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest='command', required=True)
    for name in ('check', 'verify'):
        command = commands.add_parser(name)
        command.add_argument('--ledger', type=Path, default=DEFAULT_LEDGER)
        command.add_argument('--report', type=Path)
        if name == 'check':
            command.add_argument('--require-matches', action='store_true', help='Require a receipt with every ledger function recorded as matched')
        if name == 'verify':
            command.add_argument('--game', type=Path, required=True)
            command.add_argument('--compiler', help='Clang compiler for default/clang entries')
            command.add_argument('--ee-compiler', help='Isolated EE GCC g++ for ee-gcc entries')
            command.add_argument('--output-dir', type=Path, default=ROOT / 'work/verifier')
    args = parser.parse_args(argv)
    try:
        ledger = load_ledger(args.ledger)
        if args.command == 'verify':
            destination = args.report or args.output_dir / 'verification.json'
            invalidate_report(destination, args.ledger, ledger, args.game, args.compiler, ee_compiler=args.ee_compiler)
        if args.command == 'check':
            require(not args.require_matches or args.report is not None, '--require-matches requires --report')
            count = len(recorded_matches(json.loads(args.report.read_text()), ledger)) if args.report else 0
            require(not args.require_matches or count == len(ledger['functions']), 'receipt does not record a match for every ledger function')
            print(f'Record checks passed: {len(ledger["functions"])} candidates, {count} recorded matches; no fresh private-byte verification.')
            return 0
        report = verify(ledger, args.game, args.compiler, args.output_dir, ee_compiler=args.ee_compiler)
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
        count = len(recorded_matches(report, ledger))
        print(f'Fresh private comparison: {count}/{len(ledger["functions"])} complete symbol-bounded functions matched. No linked-game or runtime proof.')
        return 0 if count == len(ledger['functions']) else 1
    except (OSError, ValueError, KeyError, TypeError, subprocess.TimeoutExpired) as exc:
        print('Verification failed: ' + str(exc), file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
