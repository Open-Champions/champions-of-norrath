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
HIST_FLAVOR = 'gcc-2.95.2-mips3-leaf'
HIST_MODE = 'historical-integer-leaf-v1'
HIST_TARGET = 'mips64el-unknown-elf'
HIST_VERSION = '2.95.2'
HIST_FLAGS = ['-mips3', '-mgp64', '-mlong32', '-mno-abicalls', '-G0',
              '-fomit-frame-pointer', '-fno-exceptions', '-fno-rtti', '-O2']
HIST_AS_FLAGS = ['-EL', '-mips3', '-mabi=o64']
HIST_OBJECT_FLAGS = 0x20002001
HIST_DESCRIPTOR = 'toolchains/gcc-2.95.2/profile.json'
HIST_LAYOUT = {'schema_version': 1, 'flavor': HIST_FLAVOR, 'version': HIST_VERSION,
               'target': HIST_TARGET, 'driver_directory': 'work/toolchains/build-gcc-2.95.2/gcc',
               'configured_prefix': 'work/toolchains/gcc-2.95.2-leaf-install',
               'assembler': 'work/toolchains/ee/bin/mips64r5900el-ps2-elf-as',
               'search_policy': 'gcc-2.95.2-cross-startfile-v1'}
HIST_POLICY = {'mode': HIST_MODE, 'object_flags': HIST_OBJECT_FLAGS,
               'abi_scope': 'isolated-integer-leaf-not-ee-abi',
               'control_flow': 'stackless-forward-integer-branches-v1',
               'runtime_relocations': False, 'dependencies': False}
HIST_MACROS = {'__GNUC__': '2', '__GNUC_MINOR__': '95', '__GNUG__': '2',
               '__cplusplus': '1', '__mips': '3', '__mips64': '1', '__MIPSEL__': '1',
               '__SIZE_TYPE__': 'unsigned int', '__PTRDIFF_TYPE__': 'int'}
HIST_MODEL_SOURCE = b'''typedef char c1[sizeof(char)==1?1:-1];
typedef char s2[sizeof(short)==2?1:-1];
typedef char i4[sizeof(int)==4?1:-1];
typedef char l4[sizeof(long)==4?1:-1];
typedef char p4[sizeof(void*)==4?1:-1];
typedef char ll8[sizeof(long long)==8?1:-1];
typedef char signed_char[((char)-1 < 0)?1:-1];
extern "C" int historical_model_probe() { return sizeof(void*); }
'''
SCALAR_MODE = 'ee-gprel-scalar-v1'
SCALAR_V2_MODE = 'ee-gprel-scalar-v2'
SCALAR_MODES = (SCALAR_MODE, SCALAR_V2_MODE)
FLOAT_MODE = 'ee-gprel-float-store-v1'
BYTE_MODE = 'ee-gprel-byte-store-v1'
DATA_MODES = (*SCALAR_MODES, FLOAT_MODE, BYTE_MODE)
SCALAR_FLAGS = sorted(EE_FLAGS - {'-G0'}) + ['-G4', '-fno-zero-initialized-in-bss']
GP_CONTEXT = 'targets/SLUS-20565/gp-context.json'
GP_SPEC = {'schema_version': 1, 'policy': 'norrath-fixed-gp-v1',
    'baseline_sha256': TARGET['baseline_sha256'],
    'gp_symbol': {'name': '_gp', 'address': '0x004879f0', 'size': 4, 'type': 1,
                  'binding': 1, 'other': 0, 'section': 65521},
    'reginfo': {'name': '.reginfo', 'type': 0x70000006, 'flags': 2, 'size': 24,
                'alignment': 4, 'entry_size': 1, 'gp': '0x004879f0'},
    'startup': {'symbol': '_start', 'address': '0x00100008', 'size': 576,
                'context_address': '0x00100198', 'context_size': 44,
                'context_sha256': 'a43b3bc3342538217f2306ff342353e372683db55f2c7a9ae383f9353015db29'},
    'runtime': {'gp_preservation': 'unverified', 'initialization': 'unverified'}}
SCALAR_POLICY = {'mode': SCALAR_MODE, 'storage': 'sdata-file-zero', 'type': 'uint32',
    'size': 4, 'relocation': 'R_MIPS_GPREL16', 'addend': 0,
    'closure': 'fresh-source-owner', 'operations': ['read', 'write', 'set-one'],
    'runtime': GP_SPEC['runtime']}
SCALAR_V2_POLICY = {'mode': SCALAR_V2_MODE, 'storage': 'sdata-file-zero', 'type': 'uint32',
    'size': 4, 'relocation': 'R_MIPS_GPREL16', 'addend': 0, 'max_owners': 2,
    'closure': 'fresh-source-owner', 'operations': ['read', 'write', 'set-one', 'add-ten'],
    'site_offsets': {'read': [4], 'write': [4], 'set-one': [8], 'add-ten': [0, 12]},
    'update': 'unsigned32-modulo-add-ten-exclusive-access', 'runtime': GP_SPEC['runtime'],
    'compiler_args': SCALAR_FLAGS.copy(), 'gp_context_policy': GP_SPEC['policy'],
    'owner_flags': BASELINE_FLAGS & ~1, 'consumer_flags': BASELINE_FLAGS}


FLOAT_STORE_FLAGS = SCALAR_FLAGS.copy()
FLOAT_MODEL = {'__SIZEOF_FLOAT__': '4', '__FLT_RADIX__': '2', '__FLT_MANT_DIG__': '24',
    '__FLT_MIN_EXP__': '(-125)', '__FLT_MAX_EXP__': '129', '__FLT_HAS_QUIET_NAN__': '0',
    '__FLT_HAS_INFINITY__': '0', '__FLT_HAS_DENORM__': '1'}
FLOAT_VALUE_CONTRACT = {'source': 'finite-common-representable-float-values',
    'target': 'store-low32-f12-object-bits', 'memory': 'exclusive-live-ordinary-float-object',
    'exceptional_values': 'target-bits-only-no-portable-cpp-claim'}
FLOAT_ABI_BYTES = bytes.fromhex('000003000201000206000000000000000100000000000000')
FLOAT_ATTRIBUTES_BYTES = bytes.fromhex('410f000000676e750001070000000402')
FLOAT_ABI = {'version': 0, 'isa_level': 3, 'isa_revision': 0, 'gpr_size': 64,
    'cpr1_size': 32, 'cpr2_size': 0, 'fp_abi': 2, 'isa_extension': 6,
    'ases': 0, 'flags1': 1, 'flags2': 0,
    'abiflags_sha256': hashlib.sha256(FLOAT_ABI_BYTES).hexdigest(),
    'attributes_sha256': hashlib.sha256(FLOAT_ATTRIBUTES_BYTES).hexdigest()}
FLOAT_POLICY = {'mode': FLOAT_MODE, 'storage': 'sdata-file-float-one', 'data_type': 'float32',
    'size': 4, 'initializer': '1.0f', 'initializer_hex': '0000803f', 'max_owners': 1,
    'operations': ['store-float-argument'], 'site_offsets': {'store-float-argument': [4]},
    'relocation': 'R_MIPS_GPREL16', 'addend': 0, 'closure': 'fresh-source-owner',
    'compiler_args': FLOAT_STORE_FLAGS.copy(), 'float_model': FLOAT_MODEL, 'abi': FLOAT_ABI,
    'value_contract': FLOAT_VALUE_CONTRACT, 'runtime': GP_SPEC['runtime'],
    'owner_flags': BASELINE_FLAGS & ~1, 'consumer_flags': BASELINE_FLAGS}


BYTE_STORE_FLAGS = sorted(EE_FLAGS - {'-G0'}) + ['-G8', '-fno-zero-initialized-in-bss']
BYTE_MODEL = {'__CHAR_BIT__': '8', '__SIZEOF_INT__': '4', '__INT_MAX__': '0x7fffffff',
              '__SCHAR_MAX__': '0x7f'}
BYTE_VALUE_CONTRACT = {'source': 'int32-to-unsigned8-modulo256',
    'target': 'store-low8-a0', 'memory': 'exclusive-live-ordinary-unsigned-char-object',
    'original_signedness': 'unclaimed'}
BYTE_POLICY = {'mode': BYTE_MODE, 'storage': 'sdata-file-zero', 'data_type': 'uint8',
    'size': 1, 'alignment': 1, 'initializer': 0, 'initializer_hex': '00', 'max_owners': 1,
    'operations': ['store-byte-argument'], 'site_offsets': {'store-byte-argument': [4]},
    'relocation': 'R_MIPS_GPREL16', 'addend': 0, 'closure': 'fresh-source-owner',
    'compiler_args': BYTE_STORE_FLAGS.copy(), 'byte_model': BYTE_MODEL, 'abi': FLOAT_ABI,
    'abi_scope': 'object-metadata-only-no-fpu-or-general-abi-proof',
    'gp_context_policy': GP_SPEC['policy'],
    'value_contract': BYTE_VALUE_CONTRACT, 'runtime': GP_SPEC['runtime'],
    'owner_flags': BASELINE_FLAGS & ~1, 'consumer_flags': BASELINE_FLAGS}


def byte_owner(owner):
    return isinstance(owner, dict) and owner.get('comparison_mode') == BYTE_MODE


def owner_width(owner):
    # Never infer a variable width from untrusted ledger size/type fields.
    return 1 if byte_owner(owner) else 4


def data_flags(mode):
    if mode == BYTE_MODE:
        return BYTE_STORE_FLAGS
    return FLOAT_STORE_FLAGS if mode == FLOAT_MODE else SCALAR_FLAGS


def float_owner(owner):
    return isinstance(owner, dict) and owner.get('comparison_mode') == FLOAT_MODE


def owner_initializer(owner):
    # Only explicit modes select float-one versus one-byte or four-byte zero.
    # No reinterpretation based on an untrusted initializer or size field.
    return bytes.fromhex('0000803f') if float_owner(owner) else bytes(owner_width(owner))


def relocation_key(mode):
    return ('byte_store_relocations' if mode == BYTE_MODE else
            'float_store_relocations' if mode == FLOAT_MODE else
            'scalar_relocations' if mode == SCALAR_V2_MODE else 'scalar_relocation')


def float_abi(elf):
    for name, typ, flags, alignment, entry, expected in (
            ('.MIPS.abiflags', 0x7000002a, 2, 8, 24, FLOAT_ABI_BYTES),
            ('.gnu.attributes', 0x6ffffff5, 0, 1, 0, FLOAT_ATTRIBUTES_BYTES)):
        index, section = elf.named_section(name)
        require((section[1], section[2], section[3], section[5], section[6], section[7], section[8], section[9])
                == (typ, flags, 0, len(expected), 0, 0, alignment, entry)
                and elf.section_data(index) == expected, 'unsupported float object ABI metadata')
    return json.loads(canonical(FLOAT_ABI))


def scalar_policy(ledger):
    return SCALAR_V2_POLICY if ledger['schema_version'] >= 3 else SCALAR_POLICY


FIXED_FLAGS = ['-nostdinc', '-nostdinc++', '-x', 'c++']
INJECTING_ENV = ('CCC_OVERRIDE_OPTIONS', 'CL', '_CL_', 'CPATH', 'CPLUS_INCLUDE_PATH',
                 'C_INCLUDE_PATH', 'OBJC_INCLUDE_PATH', 'COMPILER_PATH', 'GCC_EXEC_PREFIX',
                 'LIBRARY_PATH', 'SDKROOT', 'DYLD_INSERT_LIBRARIES', 'LD_PRELOAD',
                 'GCC_SPECS', 'GCC_COMPARE_DEBUG', 'GCC_COLORS', 'LD_LIBRARY_PATH',
                 'DYLD_LIBRARY_PATH', 'DYLD_FRAMEWORK_PATH', 'LPATH',
                 'DEPENDENCIES_OUTPUT', 'SUNPRO_DEPENDENCIES', 'OBJCPLUS_INCLUDE_PATH',
                 'COLLECT_GCC_OPTIONS')


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

    def rebuilt(self, name, call_bindings=None, scalar_binding=None, scalar_mode=SCALAR_MODE):
        require(self.kind == 1, 'compiler must emit relocatable object')
        symbol, code = self.function(name)
        require(symbol['binding'] == 1 and symbol['other'] == 0, 'candidate must be one global default-visibility function')
        direct = call_bindings is not None
        scalar = scalar_binding is not None
        require(not (direct and scalar), 'combined call/data relocation mode is unsupported')
        functions = [s for s in self.symbols if s['type'] == 2 and (not direct or s['section'] != 0)]
        require(len(functions) == 1, 'extra object functions are unsupported')
        if direct:
            undefined = self.call_symbols(symbol, call_bindings)
        elif scalar:
            undefined = self.call_symbols(symbol, [scalar_binding], data_import=True)
        else:
            require(not any(s['section'] == 0 and s['name'] for s in self.symbols), 'undefined object symbols are unsupported')
        records = []
        for i, section in enumerate(self.sections):
            if section[1] in (4, 9) and section[5]:
                if scalar and self.names[i] == '.rel.text':
                    require(section[1] == 9 and section[2] == 0x40 and section[3] == 0
                            and section[9] == 8 and section[5] == 8 * len(scalar_offsets(scalar_binding['operation'], scalar_mode))
                            and section[6] == symbol['table']
                            and section[7] == symbol['section'] and section[8] == 4,
                            'invalid scalar relocation table')
                    for offset, info in struct.iter_unpack('<II', self.section_data(i)):
                        require(info & 255 == 7 and info >> 8 in undefined,
                                'unbound or unsupported scalar relocation')
                        records.append({'offset': offset, 'type': 7, 'symbol': undefined[info >> 8]})
                    continue
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
        if scalar:
            require(records == [{'offset': offset, 'type': 7, 'symbol': scalar_binding['object_symbol']}
                                for offset in scalar_offsets(scalar_binding['operation'], scalar_mode)], 'missing or incorrect scalar relocation')
            require(self.names[symbol['section']] == '.text' and section[2] == 6,
                    'scalar consumer requires ordinary .text')
            generated_gp0(self)
            return code, records
        return code

    def call_symbols(self, candidate, bindings, historical=False, data_import=False):
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
        historical_markers = ['gcc2_compiled.', '__gnu_compiled_cplusplus']
        markers = [s for s in self.symbols if s['name'] in historical_markers]
        historical_split = False
        if historical and markers:
            require([s['name'] for s in markers] == historical_markers
                    and all(s['type'] == 0 and s['binding'] == s['value'] == s['size'] == s['other'] == 0
                            and s['section'] == candidate['section'] for s in markers),
                    'invalid historical compiler marker symbols')
            suffix = self.symbols[boundary:first_global]
            historical_split = (len(suffix) == 3 and suffix[0]['type'] == 4
                                and suffix[0]['section'] == 0xfff1
                                and suffix[0]['binding'] == suffix[0]['value'] == suffix[0]['size'] == suffix[0]['other'] == 0
                                and suffix[1:] == markers)
        require(0 < boundary and (boundary == first_global or file_split or historical_split)
                and all((s['binding'] == 0) == (s['index'] < first_global) for s in self.symbols),
                'invalid local/global symbol boundary')
        undefined = {}
        for s in self.symbols:
            if s['index'] == 0:
                require(all(s[k] == 0 for k in ('value', 'size', 'type', 'binding', 'other', 'section'))
                        and s['name'] == '', 'invalid null object symbol')
            elif s is candidate:
                continue
            elif historical and s in markers:
                continue
            elif s['section'] == 0:
                require(s['name'] and s['binding'] == 1 and s['other'] == 0 and s['type'] in ((0, 1) if data_import else (0, 2))
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
        require(mode in (None, DIRECT_MODE, HIST_MODE, *DATA_MODES), 'unsupported comparison mode')
        if mode in (None, HIST_MODE, *DATA_MODES):
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


def audit_historical_body(code):
    """Closed stackless integer leaf CFG; likely branches annul the false delay slot.

    This is a shape check, not proof of pointer lifetimes or the C++ view contract.
    Forward-only edges plus complete reachability exclude hidden trailing bodies.
    """
    require(len(code) >= 8 and len(code) % 4 == 0, 'invalid historical body extent')
    words = struct.unpack('<' + 'I' * (len(code) // 4), code)
    end = len(words) - 2
    require(words[end] == 0x03e00008, 'historical leaf needs terminal JR RA')
    branches, delays = {}, {end + 1}
    forbidden = set(range(16, 24)) | {26, 27, 28, 29, 30, 31}
    for i, word in enumerate(words):
        op, rs, rt, rd, sa, fn = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31, (word >> 6) & 31, word & 63
        if i == end:
            continue
        if op in (4, 5, 6, 7, 20, 21, 22, 23):
            require(op not in (6, 7, 22, 23) or rt == 0, 'reserved historical branch fields')
            require(rs not in forbidden and rt not in forbidden, 'historical branch uses protected register')
            displacement = word & 65535
            if displacement & 32768:
                displacement -= 65536
            target = i + 1 + displacement
            require(i + 2 <= target <= end and i + 2 <= end, 'historical branch target or continuation outside forward body')
            branches[i] = (target, op >= 20)
            delays.add(i + 1)
            continue
        # Reuse the already reviewed noncontrol integer encoding/reserved-field
        # decoder, then impose this profile's stricter stackless register rule.
        audit_integer_body(struct.pack('<3I', word, 0x03e00008, 0))
        if op == 0:
            used = {rt, rd} | ({rs} if fn not in (0, 2, 3, 0x38, 0x3a, 0x3b, 0x3c, 0x3e, 0x3f) else set())
        else:
            used = {rt} | ({rs} if op != 15 else set())
        require(not used.intersection(forbidden), 'historical leaf uses protected/stack/global-pointer register')
    controls = set(branches) | {end}
    require(not controls.intersection(delays), 'control transfer in historical delay slot')
    require(all(target not in delays and i + 2 not in delays for i, (target, _) in branches.items()),
            'historical edge enters delay slot')
    # Nodes are (instruction index, delayed continuation). Modeling delay context
    # preserves the distinction between ordinary and branch-likely false edges.
    pending, seen, reached = [(0, None)], set(), set()
    while pending:
        i, after = pending.pop()
        if (i, after) in seen:
            continue
        seen.add((i, after)); reached.add(i)
        require(0 <= i < len(words), 'historical control flow escapes body')
        if after is not None:
            if after != -1:
                pending.append((after, None))
        elif i == end:
            pending.append((i + 1, -1))
        elif i in branches:
            target, likely = branches[i]
            pending.append((i + 1, target))
            pending.append((i + 2, None) if likely else (i + 1, i + 2))
        else:
            require(i not in delays, 'historical fallthrough enters an unowned delay slot')
            pending.append((i + 1, None))
    require(reached == set(range(len(words))), 'unreachable historical body instructions')


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


def original_graph_body(baseline, item, strict_intersections=False):
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
    for typ, file_offset, virtual, _, filesz, memsz, flags, alignment in struct.iter_unpack('<8I', headers):
        if strict_intersections and typ == 1:
            require(filesz <= memsz and virtual + memsz <= 2**32
                    and (alignment in (0, 1) or (alignment & (alignment - 1) == 0 and file_offset % alignment == virtual % alignment)),
                    'malformed original code load mapping')
            baseline.span(file_offset, filesz)
            if memsz > 0 and virtual < symbol['value'] + len(code) and symbol['value'] < virtual + memsz:
                require(flags & 1 and virtual <= symbol['value'] and symbol['value'] + len(code) <= virtual + filesz,
                        'overlapping nonexecutable or partial original code mapping')
                mappings.append(file_offset + symbol['value'] - virtual)
            continue
        if typ == 1 and virtual <= symbol['value'] and symbol['value'] + len(code) <= virtual + filesz:
            baseline.span(file_offset, filesz)
            if flags & 1 and filesz <= memsz and virtual + memsz <= 2**32:
                mappings.append(file_offset + symbol['value'] - virtual)
    require(mappings == [offset], 'original function lacks unique executable load mapping')
    return code


def scalar_offset(operation):
    require(operation in SCALAR_POLICY['operations'], 'unsupported scalar operation')
    return 8 if operation == 'set-one' else 4


def scalar_offsets(operation, mode=SCALAR_MODE):
    if mode == BYTE_MODE:
        require(operation == 'store-byte-argument', 'unsupported byte store operation')
        return [4]
    if mode == FLOAT_MODE:
        require(operation == 'store-float-argument', 'unsupported float store operation')
        return [4]
    if mode == SCALAR_MODE:
        return [scalar_offset(operation)]
    require(mode == SCALAR_V2_MODE and operation in SCALAR_V2_POLICY['site_offsets'], 'unsupported scalar operation/version')
    return SCALAR_V2_POLICY['site_offsets'][operation].copy()


def gp_descriptor(root):
    path = local_path(root, GP_CONTEXT)
    require(not (Path(root) / GP_CONTEXT).is_symlink(), 'GP context must not be a symlink')
    data = path.read_bytes()
    require(json.loads(data) == GP_SPEC and GP_SPEC['baseline_sha256'] == TARGET['baseline_sha256'],
            'unsupported fixed GP context')
    return digest(data)


def gp_context_evidence(baseline, root):
    context_hash = gp_descriptor(root)
    expected = GP_SPEC['gp_symbol']
    symbols = [s for s in baseline.symbols if s['name'] == expected['name']]
    require(len(symbols) == 1 and all(symbols[0][key] == expected[key]
            for key in ('name', 'size', 'type', 'binding', 'other', 'section'))
            and symbols[0]['value'] == address(expected['address']), 'original GP symbol identity mismatch')
    _, section = baseline.named_section('.reginfo')
    require((section[1], section[2], section[5], section[8], section[9]) == (0x70000006, 2, 24, 4, 1),
            'original GP .reginfo shape mismatch')
    words = struct.unpack('<6I', baseline.span(section[4], 24))
    require(words[-1] == address(expected['address']), 'original .reginfo GP differs from pinned context')
    startup = GP_SPEC['startup']
    startup_item = {'original_symbol': startup['symbol'], 'address': startup['address'], 'size': startup['size']}
    code = baseline.original(startup_item)
    original_graph_body(baseline, startup_item, strict_intersections=True)
    start = address(startup['context_address']) - address(startup['address'])
    require(0 <= start and start + startup['context_size'] <= len(code), 'GP context escapes startup symbol')
    window = code[start:start + startup['context_size']]
    require(len(window) == 44 and digest(window) == startup['context_sha256'], 'startup GP context changed')
    # Decode the small reviewed arithmetic window. Unknown initial registers are
    # never read; no loads, branches, syscalls or side effects are interpreted.
    registers, writes = {0: 0}, {}
    for offset in range(0, 44, 4):
        word, = struct.unpack_from('<I', window, offset)
        op, rs, rt, rd = word >> 26, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31
        immediate = word & 65535
        if op == 15 and rs == 0:
            destination, value = rt, immediate << 16
        elif op == 9 and rs in registers:
            destination, value = rt, (registers[rs] + (immediate if immediate < 32768 else immediate - 65536)) & 0xffffffff
        elif op == 0 and word & 2047 == 0x2d and rs in registers and rt in registers:
            destination, value = rd, (registers[rs] + registers[rt]) & 0xffffffff
        else:
            raise ValueError('unsupported startup GP arithmetic')
        require(destination in (4, 5, 6, 7, 8, 28), 'unexpected startup GP register destination')
        registers[destination] = value
        writes.setdefault(destination, []).append(offset)
    require(writes.get(4) == [0, 20] and writes.get(28) == [40]
            and registers[28] == address(expected['address']), 'startup arithmetic does not establish pinned GP')
    return {'context_sha256': context_hash, 'startup_context_sha256': digest(window),
            'gp': expected['address'], 'runtime': GP_SPEC['runtime']}


def scalar_review(ledger, owner, root):
    consumers = [i for i in ledger['functions'] if i.get('comparison_mode') in DATA_MODES
                 and i['data_binding']['owner_id'] == owner['id']]
    path = local_path(root, owner['review'])
    require(path.suffix == '.json' and not (Path(root) / owner['review']).is_symlink(), 'scalar review must be regular JSON')
    data = path.read_bytes()
    review = json.loads(data)
    source_hashes = {item['source']: digest(source_bytes(local_path(root, item['source']))) for item in [owner] + consumers}
    require(isinstance(review, dict) and review.get('schema_version') == (4 if byte_owner(owner) else 3 if float_owner(owner) else 2 if ledger['schema_version'] >= 3 else 1)
            and review.get('baseline_sha256') == TARGET['baseline_sha256']
            and review.get('owner_id') == owner['id']
            and review.get('function_ids') == sorted(i['id'] for i in consumers)
            and review.get('source_sha256') == source_hashes
            and review.get('gp_context_sha256') == gp_descriptor(root)
            and review.get('runtime') == GP_SPEC['runtime'], 'stale or incomplete scalar source review')
    if ledger['schema_version'] >= 3:
        require(review.get('comparison_mode') == (BYTE_MODE if byte_owner(owner) else FLOAT_MODE if float_owner(owner) else SCALAR_V2_MODE)
                and review.get('operations') == {i['id']: i['data_binding']['operation'] for i in consumers},
                'stale or incomplete scalar-v2 operation review')
    if float_owner(owner):
        require(canonical(review.get('value_contract')) == canonical(FLOAT_VALUE_CONTRACT), 'missing float value/lifetime review contract')
    if byte_owner(owner):
        require(type(review.get('schema_version')) is int
                and set(review) - {'manual_contract'} == {'schema_version', 'baseline_sha256', 'owner_id', 'function_ids',
                    'source_sha256', 'gp_context_sha256', 'runtime', 'comparison_mode', 'operations', 'value_contract'}
                and canonical(review.get('value_contract')) == canonical(BYTE_VALUE_CONTRACT),
                'missing byte source/value review contract')
        require('manual_contract' not in review or isinstance(review['manual_contract'], dict),
                'manual byte review notes must be a dictionary')
    return digest(data)


def scalar_owners(ledger, root):
    consumers = [i for i in ledger['functions'] if i.get('comparison_mode') in DATA_MODES]
    if not consumers:
        require('data_objects' not in ledger and ledger['schema_version'] not in (4, 5), 'unreferenced data owner collection or missing float lane')
        return {}
    items = ledger.get('data_objects')
    v2 = ledger['schema_version'] >= 3
    mixed_float = ledger['schema_version'] in (4, 5)
    mixed_byte = ledger['schema_version'] == 5
    mode = SCALAR_V2_MODE if v2 else SCALAR_MODE
    require(ledger['schema_version'] in (2, 3, 4, 5) and isinstance(items, list)
            and all(isinstance(owner, dict) for owner in items)
            and (2 <= len(items) <= 4 if mixed_byte else 1 <= len(items) <= 3 if mixed_float else 1 <= len(items) <= 2 if v2 else len(items) == 1)
            and all(i['comparison_mode'] in (SCALAR_V2_MODE, FLOAT_MODE, BYTE_MODE) if mixed_byte
                    else i['comparison_mode'] in (SCALAR_V2_MODE, FLOAT_MODE) if mixed_float
                    else i['comparison_mode'] == mode for i in consumers),
            'scalar version/owner count mismatch or mixed scalar policies')
    if mixed_float:
        require(type(ledger['schema_version']) is int, 'float ledger schema must be an integer')
        require(sum(float_owner(o) for o in items) == 1
                and sum(byte_owner(o) for o in items) == (1 if mixed_byte else 0)
                and all(o.get('comparison_mode') in ((SCALAR_V2_MODE, FLOAT_MODE, BYTE_MODE) if mixed_byte
                                                   else (SCALAR_V2_MODE, FLOAT_MODE)) for o in items),
                'ledger4/5 requires exact float/byte lanes and explicit per-owner modes')
    else:
        require(not any(float_owner(o) or byte_owner(o) for o in items), 'typed owner needs ledger4/5')
    identities = {key: {i[key] for i in ledger['functions']} for key in ('id', 'symbol', 'original_symbol')}
    if v2:
        require(all(len(values) == len(ledger['functions']) for values in identities.values()),
                'scalar-v2 function identities collide')
    sources = {local_path(root, i['source']) for i in ledger['functions']}
    review_paths, extents, owners = set(), [], {}
    for owner in items:
        require(isinstance(owner, dict) and IDENT.fullmatch(owner['id']) and IDENT.fullmatch(owner['symbol'])
                and re.fullmatch(r'[A-Za-z_.$][A-Za-z0-9_.$]*', owner['original_symbol']), 'invalid scalar owner identity')
        require(all(owner[key] not in identities[key] for key in ('id', 'symbol'))
                and (not v2 or owner['original_symbol'] not in identities['original_symbol']), 'scalar owner identity collision')
        require(type(owner['size']) is int and owner['size'] == owner_width(owner) and owner['section'] == '.sdata'
                and ((float_owner(owner) and owner['data_type'] == 'float32'
                      and owner['storage'] == 'sdata-file-float-one' and owner['initializer'] == '1.0f')
                     or (byte_owner(owner) and owner['data_type'] == 'uint8'
                         and owner['storage'] == 'sdata-file-zero'
                         and type(owner['initializer']) is int and owner['initializer'] == 0)
                     or (not float_owner(owner) and not byte_owner(owner) and owner['data_type'] == 'uint32'
                         and owner['storage'] == 'sdata-file-zero'
                         and type(owner['initializer']) is int and owner['initializer'] == 0))
                and type(owner['original_binding']) is int and owner['original_binding'] in (0, 1), 'unsupported scalar owner type/storage')
        require(owner['linkage_adapter'] == ('original-local-to-unique-extern' if owner['original_binding'] == 0 else 'global-to-global'),
                'missing explicit original/source linkage adaptation')
        start = address(owner['address'])
        require(0 < start <= 0x80000000 - owner_width(owner) and start % owner_width(owner) == 0, 'scalar owner address out of supported range')
        source = local_path(root, owner['source'])
        require(source.suffix == '.cpp' and owner['language'] == 'c++' and owner['toolchain'] == 'ee-gcc'
                and owner['compiler_args'] == data_flags(owner.get('comparison_mode')) and source not in sources, 'invalid scalar owner source/profile')
        source_bytes(source)
        if v2:
            review_path = local_path(root, owner['review'])
            require(review_path not in review_paths and (not mixed_float or review_path not in sources
                    and review_path != source), 'data owners need distinct reviews and sources')
            require(not mixed_float or source not in review_paths, 'source collides with data review')
            require(all(start + owner_width(owner) <= address(i['address']) or address(i['address']) + i['size'] <= start
                        for i in ledger['functions']), 'scalar-v2 owner overlaps function')
            require(all(start + owner_width(owner) <= lo or hi <= start for lo, hi in extents), 'scalar-v2 owner extents overlap')
            review_paths.add(review_path)
        extents.append((start, start + owner_width(owner))); sources.add(source)
        for key in identities:
            identities[key].add(owner[key])
        owners[owner['id']] = owner
    used = set()
    for item in consumers:
        binding = item.get('data_binding')
        require(isinstance(binding, dict) and set(binding) == {'owner_id', 'object_symbol', 'operation'}
                and binding['owner_id'] in owners, 'unbound scalar source dependency')
        owner = owners[binding['owner_id']]
        require(binding['object_symbol'] == owner['symbol'], 'scalar import belongs to another owner')
        require(not mixed_float or owner['comparison_mode'] == item['comparison_mode'], 'cross-policy data owner binding')
        require(item['size'] == max(scalar_offsets(binding['operation'], item['comparison_mode'])) + 4
                and item.get('toolchain') == 'ee-gcc' and item['compiler_args'] == data_flags(item['comparison_mode']),
                'scalar consumer shape/profile mismatch')
        used.add(owner['id'])
    require(used == set(owners), 'unconsumed scalar source owner')
    for owner in owners.values():
        scalar_review(ledger, owner, root)
    return owners


def generated_gp0(elf):
    index, section = elf.named_section('.reginfo')
    require((section[1], section[2], section[5], section[8], section[9]) == (0x70000006, 0, 24, 4, 1)
            and struct.unpack('<6I', elf.section_data(index))[-1] == 0, 'unsupported generated input GP0 metadata')


def scalar_owner_original(elf, owner):
    matches = [s for s in elf.symbols if s['name'] == owner['original_symbol']]
    require(len(matches) == 1, 'ambiguous original scalar owner')
    symbol = matches[0]
    start = address(owner['address']); width = owner_width(owner); end = start + width
    require(symbol['value'] == start and symbol['size'] == width and symbol['type'] == 1
            and symbol['binding'] == owner['original_binding'] and symbol['other'] == 0
            and 0 < symbol['section'] < len(elf.sections), 'original scalar owner identity mismatch')
    index, section = elf.named_section('.sdata')
    require(symbol['section'] == index and section[1] == 1 and section[2] == 0x10000003
            and section[8] >= width and section[8] & (section[8] - 1) == 0
            and section[3] <= start and end <= section[3] + section[5] <= 2**32, 'original scalar storage bounds mismatch')
    for other in elf.symbols:
        require(other['section'] != 0xfff2, 'unresolved COMMON symbol in original scalar executable')
        gp = GP_SPEC['gp_symbol']
        exact_gp = other['name'] == gp['name'] and other['value'] == address(gp['address']) and all(other[k] == gp[k] for k in ('size', 'type', 'binding', 'other', 'section'))
        if other is symbol or other['type'] in (3, 4) or other['section'] == 0 or exact_gp:
            continue
        overlaps = (other['size'] > 0 and other['value'] < end and start < other['value'] + other['size'])
        require(not overlaps and not (other['size'] == 0 and start <= other['value'] < end),
                'original scalar owner overlaps or aliases another symbol')
    offset = section[4] + start - section[3]
    contents = elf.span(offset, width)
    require(contents == owner_initializer(owner), 'original data initializer differs from explicit policy')
    phoff, = struct.unpack_from('<I', elf.data, 28)
    phsize, phnum = struct.unpack_from('<HH', elf.data, 42)
    require(phsize == 32 and 0 < phnum < 0xffff, 'scalar owner needs ordinary load headers')
    intersections = []
    for i, row in enumerate(struct.iter_unpack('<8I', elf.span(phoff, phsize * phnum))):
        typ, fileoff, virtual, _, filesz, memsz, flags, alignment = row
        if typ != 1:
            continue
        require(filesz <= memsz and virtual + memsz <= 2**32
                and (alignment in (0, 1) or (alignment & (alignment - 1) == 0 and fileoff % alignment == virtual % alignment)),
                'malformed scalar load mapping')
        elf.span(fileoff, filesz)
        # Reject ANY additional overlap, not merely additional full mappings.
        if memsz > 0 and virtual < end and start < virtual + memsz:
            intersections.append((i, row))
    require(len(intersections) == 1, 'scalar owner has overlapping or missing load mapping')
    load_index, row = intersections[0]
    require(row[6] & 3 == 2 and row[2] <= start and end <= row[2] + row[4]
            and row[1] + start - row[2] == offset, 'scalar owner lacks unique writable file mapping')
    return {'symbol': owner['original_symbol'], 'address': owner['address'], 'size': owner_width(owner),
            'binding': symbol['binding'], 'section': '.sdata', 'section_index': index,
            'file_offset': offset, 'load_index': load_index, 'initializer_sha256': digest(contents)}


def scalar_owner_rebuilt(elf, owner):
    require(elf.kind == 1 and elf.flags == BASELINE_FLAGS & ~1, 'scalar owner requires reviewed EE relocatable object')
    width = owner_width(owner)
    matches = [s for s in elf.symbols if s['name'] == owner['symbol']]
    require(len(matches) == 1, 'ambiguous generated scalar owner')
    symbol = matches[0]
    require((symbol['type'], symbol['binding'], symbol['other'], symbol['value'], symbol['size']) == (1, 1, 0, 0, width),
            'generated scalar must be one complete global object')
    index, section = elf.named_section('.sdata')
    require(symbol['section'] == index and (section[1], section[2], section[3], section[5], section[8]) == (1, 0x10000003, 0, width, width),
            'generated scalar storage mismatch')
    require(elf.section_data(index) == owner_initializer(owner), 'generated scalar initializer mismatch')
    if float_owner(owner) or byte_owner(owner):
        float_abi(elf)
    elf.call_symbols(symbol, [])
    generated_gp0(elf)
    for i, current in enumerate(elf.sections):
        require(not (current[1] in (4, 9) and current[5]), 'scalar owner contains relocations')
        require(not (current[2] & 4 and current[5]), 'scalar owner contains code')
        if current[2] & 2 and current[5] and i != index:
            require((elf.names[i], current[1], current[2], current[5]) == ('.MIPS.abiflags', 0x7000002a, 2, 24),
                    'scalar owner contains extra allocated payload')
    return owner_initializer(owner)


def scalar_body(code, operation, raw=False, mode=SCALAR_MODE):
    offset = max(scalar_offsets(operation, mode))
    require(len(code) == offset + 4, 'scalar body is not complete closed accessor shape')
    words = struct.unpack('<' + 'I' * (len(code) // 4), code)
    require(words[-2] == 0x03e00008 and (operation != 'set-one' or words[0] == 0x24020001),
            'scalar accessor contains unsupported instruction')
    if operation == 'add-ten':
        require(words[1] == 0x2442000a and words[0] & 0xffff0000 == 0x8f820000
                and words[0] & 65535 == words[-1] & 65535, 'unsupported or cross-owner add-ten load/update')
    expected = {'read': 0x8f820000, 'write': 0xaf840000, 'set-one': 0xaf820000, 'add-ten': 0xaf820000, 'store-float-argument': 0xe78c0000, 'store-byte-argument': 0xa3840000}[operation]
    require(words[-1] & 0xffff0000 == expected and (not raw or words[-1] & 65535 == 0),
            'scalar access opcode/register or zero-addend mismatch')
    value = words[-1] & 65535
    return value if value < 32768 else value - 65536


def scalar_relocation_evidence(item, owner):
    gp = address(GP_SPEC['gp_symbol']['address'])
    delta = address(owner['address']) - gp
    require(-32768 <= delta <= 32767 and gp + ((delta & 65535) if delta & 65535 < 32768 else (delta & 65535) - 65536)
            == address(owner['address']), 'scalar GP-relative displacement overflow')
    evidence = {'type': 'R_MIPS_GPREL16',
            'object_symbol': owner['symbol'], 'owner_id': owner['id'], 'address': owner['address'],
            'gp': GP_SPEC['gp_symbol']['address'], 'addend': 0, 'displacement': delta}
    mode = item.get('comparison_mode', SCALAR_MODE)
    sites = [dict(evidence, offset=offset) for offset in scalar_offsets(item['data_binding']['operation'], mode)]
    return sites if mode in (SCALAR_V2_MODE, FLOAT_MODE, BYTE_MODE) else sites[0]


def relocate_scalar(code, original, item, owner):
    operation = item['data_binding']['operation']
    evidence = scalar_relocation_evidence(item, owner)
    mode = item.get('comparison_mode', SCALAR_MODE)
    sites = evidence if mode in (SCALAR_V2_MODE, FLOAT_MODE, BYTE_MODE) else [evidence]
    require(scalar_body(original, operation, mode=mode) == sites[0]['displacement'], 'original scalar access targets a different owner')
    scalar_body(code, operation, raw=True, mode=mode)
    output = bytearray(code)
    for site in sites:
        word, = struct.unpack_from('<I', output, site['offset'])
        struct.pack_into('<I', output, site['offset'], word | (site['displacement'] & 65535))
    return bytes(output), evidence


def validate_ledger(ledger, root=ROOT):
    require(isinstance(ledger, dict) and type(ledger.get('schema_version')) is int and ledger['schema_version'] in (1, 2, 3, 4, 5), 'unsupported ledger schema')
    require(ledger.get('target') == TARGET, 'ledger must pin the reviewed Norrath baseline identity and .text bounds')
    require(not any(k in ledger for k in ('byte_store_policy', 'byte_store_policy_sha256')),
            'byte policy evidence belongs to receipts, not ledgers')
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
        require(flavor in ('clang', 'ee-gcc', HIST_FLAVOR), 'unsupported toolchain flavor')
        scalar = item.get('comparison_mode') in DATA_MODES
        require(scalar or 'data_binding' not in item, 'ordinary entries cannot declare scalar dependencies')
        if scalar:
            require(flavor == 'ee-gcc' and flags == data_flags(item['comparison_mode']), 'scalar mode requires exact isolated EE profile')
        if flavor == HIST_FLAVOR:
            require(item.get('comparison_mode') == HIST_MODE and flags == HIST_FLAGS,
                    'historical leaf requires explicit mode and exact ordered profile')
        else:
            require(item.get('comparison_mode') != HIST_MODE, 'historical mode requires historical compiler')
        required, allowed = ((set(data_flags(item['comparison_mode'])), set(data_flags(item['comparison_mode']))) if scalar else (REQUIRED_FLAGS, ALLOWED_FLAGS) if flavor == 'clang' else
                             (set(HIST_FLAGS), set(HIST_FLAGS)) if flavor == HIST_FLAVOR else (EE_FLAGS, EE_FLAGS))
        require(len(flags) == len(set(flags)) and required <= set(flags) <= allowed, 'unsupported or missing compiler flags')
        require(sum(f.startswith('-O') for f in flags) == 1, 'exactly one optimization setting is required')
        require(not ({'-msoft-float', '-mhard-float'} <= set(flags)) and not ({'-fomit-frame-pointer', '-fno-omit-frame-pointer'} <= set(flags)), 'conflicting compiler arguments')
        ids.add(ident)
        sources.add(source)
        extents.append((lo, lo + size))
    extents.sort()
    require(all(a[1] <= b[0] for a, b in zip(extents, extents[1:])), 'ledger function extents overlap')
    scalar_owners(ledger, root)
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
    if item.get('toolchain') == HIST_FLAVOR:
        return ['-B<reviewed-driver-directory>/'] + HIST_FLAGS + FIXED_FLAGS + ['-S', '<source-snapshot.cpp>', '-o', '<fresh-assembly.s>']
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


def ee_data_model(executable, flags, env, expected=None):
    expected = EE_MODEL if expected is None else expected
    output = compiler_output([str(executable)] + flags + FIXED_FLAGS + ['-dM', '-E', '-'], env, input=b'')
    macros = {}
    for line in output.decode('ascii').splitlines():
        parts = line.split(maxsplit=2)
        if len(parts) == 3 and parts[0] == '#define' and parts[1] in expected:
            require(parts[1] not in macros, 'duplicate EE data-model declaration')
            macros[parts[1]] = parts[2]
    require(macros == expected, 'EE GCC data model does not match reviewed R5900/EABI leaf scope')
    return macros


def historical_layout(root):
    root = Path(root).resolve()
    descriptor = root / HIST_DESCRIPTOR
    require(descriptor.is_file() and not descriptor.is_symlink(), 'missing regular historical build descriptor')
    data = descriptor.read_bytes()
    require(json.loads(data) == HIST_LAYOUT, 'unsupported historical build layout descriptor')
    directory = root / HIST_LAYOUT['driver_directory']
    prefix = root / HIST_LAYOUT['configured_prefix']
    assembler = root / HIST_LAYOUT['assembler']
    machine = Path(HIST_TARGET) / HIST_VERSION
    standard = prefix / 'lib/gcc-lib'
    # gcc.c process_command + find_a_file: startfile prefixes are NOT exec
    # prefixes. require_machine_suffix=1 excludes raw and target-only forms.
    libraries = [directory / machine, directory, standard / machine,
                 Path('/usr/lib/gcc') / machine, prefix / HIST_TARGET / 'lib' / machine,
                 prefix / HIST_TARGET / 'lib']
    programs = [directory / machine, directory, standard / machine, standard / HIST_TARGET,
                Path('/usr/lib/gcc') / machine, Path('/usr/lib/gcc') / HIST_TARGET,
                prefix / HIST_TARGET / 'bin' / machine, prefix / HIST_TARGET / 'bin']
    specs = [p / 'specs' for p in libraries] + [standard / HIST_TARGET / 'specs']
    require(all(not os.path.lexists(p) for p in specs), 'external historical GCC specs are unsupported')
    return directory, assembler, digest(data), {
        'install': str(standard / machine) + '/',
        'programs': ':'.join(str(p) + '/' for p in programs),
        'libraries': ':'.join(str(p) + '/' for p in libraries)}


def historical_identity(compiler, assembler, env, root):
    directory, expected_as, descriptor_hash, searches = historical_layout(root)
    def probe(command):
        return compiler_output(command, env, cwd=env['TMPDIR'])
    require(compiler is not None and assembler is not None, 'missing historical compiler or assembler')
    expected = {'xgcc': directory / 'xgcc', 'cpp': directory / 'cpp',
                'cc1plus': directory / 'cc1plus', 'as': expected_as}
    require(Path(compiler).absolute() == expected['xgcc'] and Path(assembler).absolute() == expected_as,
            'historical compiler/assembler differ from reviewed fixed layout')
    for path in expected.values():
        require(path.is_file() and not path.is_symlink() and path.resolve() == path,
                'historical components must be regular files in the reviewed layout')
    hashes = {name: digest(path.read_bytes()) for name, path in expected.items()}
    executable = expected['xgcc']
    command = [str(executable), '-B' + str(directory) + '/'] + HIST_FLAGS + FIXED_FLAGS
    require(probe(command + ['-dumpversion']).decode('ascii').strip() == HIST_VERSION,
            'unsupported historical GCC version')
    require(probe(command + ['-dumpmachine']).decode('ascii').strip() == HIST_TARGET,
            'unsupported historical GCC target')
    observed = probe(command + ['-print-search-dirs']).decode('utf-8')
    require(observed == ''.join(key + ': ' + value + '\n' for key, value in searches.items()),
            'historical configured prefix/search paths differ from descriptor')
    require(probe(command + ['-print-file-name=specs']).decode('utf-8').strip() == 'specs',
            'external historical GCC specs are unsupported')
    for name in ('cpp', 'cc1plus'):
        selected = probe(command + ['-print-prog-name=' + name]).decode('utf-8').strip()
        require(selected == str(expected[name]), 'historical component resolution is shadowed or outside layout')
    specs = probe(command + ['-dumpspecs'])
    version = probe([str(expected_as), '--version']).decode('ascii').splitlines()
    require(version and version[0] == 'GNU assembler (GNU Binutils) 2.45.1'
            and any(EE_TARGET in line for line in version), 'unsupported historical assembler identity')
    require(historical_layout(root)[2:] == (descriptor_hash, searches)
            and hashes == {name: digest(path.read_bytes()) for name, path in expected.items()},
            'historical components or descriptor changed during identity probes')
    return executable, {'flavor': HIST_FLAVOR, 'name': 'xgcc', 'version': HIST_VERSION,
                        'target': HIST_TARGET, 'sha256': hashes['xgcc'],
                        'components': {name: {'name': name, 'sha256': hashes[name]}
                                       for name in ('cpp', 'cc1plus', 'as')},
                        'assembler_version': version[0], 'descriptor_sha256': descriptor_hash,
                        'specs_sha256': digest(specs), 'search_policy': HIST_LAYOUT['search_policy']}


def historical_model_record():
    return {'probe_sha256': digest(HIST_MODEL_SOURCE), 'macros': HIST_MACROS,
            'assertions': 'char1-short2-int4-long4-pointer4-longlong8-signedchar', 'compiled': True}


def historical_evidence(result):
    return digest(canonical({key: result[key] for key in (
        'source_sha256', 'compiler_args', 'toolchain_sha256', 'data_model',
        'assembly_sha256', 'assembler_args', 'object_flags', 'object_sha256',
        'comparison_mode', 'control_flow_policy', 'original_sha256', 'rebuilt_sha256', 'size')}))


def historical_compile(executable, assembler, snapshot, obj, directory, env, model=False):
    assembly = directory / ('model.s' if model else 'fresh.s')
    require(not os.path.lexists(assembly) and not os.path.lexists(obj), 'historical outputs must be fresh')
    command = [str(executable), '-B' + str(executable.parent) + '/'] + HIST_FLAGS + FIXED_FLAGS
    compiled = subprocess.run(command + ['-S', str(snapshot), '-o', str(assembly)],
                              cwd=directory, capture_output=True, timeout=120, check=False, env=env)
    require(compiled.returncode == 0, 'historical source/model compilation failed')
    require(assembly.is_file() and not assembly.is_symlink(), 'historical compiler produced no regular assembly')
    data = assembly.read_bytes()
    assembled = subprocess.run([str(assembler)] + HIST_AS_FLAGS + ['-o', str(obj)], input=data,
                               cwd=directory, capture_output=True, timeout=120, check=False, env=env)
    require(assembled.returncode == 0 and obj.is_file() and not obj.is_symlink(),
            'historical assembly failed or produced no regular object')
    require(assembly.read_bytes() == data, 'generated assembly changed during historical compilation')
    return digest(data), compiled.stdout + compiled.stderr + assembled.stdout + assembled.stderr


def historical_data_model(executable, assembler, directory, env):
    probe = directory / 'model.cpp'
    probe.write_bytes(HIST_MODEL_SOURCE)
    obj = directory / 'model.o'
    historical_compile(executable, assembler, probe, obj, directory, env, model=True)
    require(probe.read_bytes() == HIST_MODEL_SOURCE, 'historical data-model source changed')
    elf = Elf32(obj.read_bytes())
    require(elf.flags == HIST_OBJECT_FLAGS, 'historical data-model object flags mismatch')
    code = elf.rebuilt('historical_model_probe')
    audit_historical_body(code)
    command = [str(executable), '-B' + str(executable.parent) + '/'] + HIST_FLAGS + FIXED_FLAGS
    output = compiler_output(command + ['-dM', '-E', '-'], env, input=b'', cwd=directory)
    macros = {}
    for line in output.decode('ascii').splitlines():
        parts = line.split(maxsplit=2)
        if len(parts) == 3 and parts[0] == '#define':
            require(parts[1] not in ('__mips_eabi', '_MIPS_ARCH_R5900'), 'historical model unexpectedly claims EE ABI')
            if parts[1] in HIST_MACROS:
                require(parts[1] not in macros, 'duplicate historical model macro')
                macros[parts[1]] = parts[2].strip()
    require(macros == HIST_MACROS, 'historical data-model macro mismatch')
    return historical_model_record()


def compile_scalar_owner(owner, original, contents, executable, identity, review_hash, context, output_dir, root, env):
    result = {'id': owner['id'], 'source_sha256': digest(contents), 'compiler_args': effective_args(owner),
              'toolchain': 'ee-gcc', 'toolchain_sha256': digest(canonical(identity)),
              'review_sha256': review_hash, 'gp_context_sha256': context['context_sha256'],
              'status': 'failed'}
    with tempfile.TemporaryDirectory(prefix=owner['id'] + '-', dir=output_dir) as temporary:
        directory = Path(temporary).resolve()
        snapshot, obj = directory / 'source.cpp', directory / 'fresh.o'
        snapshot.write_bytes(contents)
        try:
            require(toolchain_identity('ee-gcc', executable, env)[1] == identity, 'scalar owner compiler changed before compilation')
            result['data_model'] = ee_data_model(executable, owner['compiler_args'], env)
            if float_owner(owner):
                result['float_model'] = ee_data_model(executable, owner['compiler_args'], env, FLOAT_MODEL)
            if byte_owner(owner):
                result['byte_model'] = ee_data_model(executable, owner['compiler_args'], env, BYTE_MODEL)
            command = [str(executable)] + owner['compiler_args'] + FIXED_FLAGS + ['-c', str(snapshot), '-o', str(obj)]
            compiled = subprocess.run(command, cwd=directory, capture_output=True, timeout=120, check=False, env=env)
            (output_dir / (owner['id'] + '.log')).write_bytes(compiled.stdout + compiled.stderr)
            require(compiled.returncode == 0, 'scalar owner compilation failed')
            require(obj.is_file() and not obj.is_symlink(), 'scalar owner needs fresh regular object')
            require(source_bytes(local_path(root, owner['source'])) == contents and snapshot.read_bytes() == contents,
                    'scalar owner source changed during compilation')
            require(toolchain_identity('ee-gcc', executable, env)[1] == identity, 'scalar owner components changed during compilation')
            data = obj.read_bytes()
            rebuilt = scalar_owner_rebuilt(Elf32(data), owner)
            if float_owner(owner):
                result.update(float_abi=float_abi(Elf32(data)), comparison_mode=FLOAT_MODE, data_type='float32')
            if byte_owner(owner):
                result.update(byte_abi=float_abi(Elf32(data)), comparison_mode=BYTE_MODE, data_type='uint8')
            result.update(status='matched', size=owner_width(owner), storage=owner['storage'], initializer=owner['initializer'],
                          object_sha256=digest(data), original=original, rebuilt_sha256=digest(rebuilt),
                          generated_binding=1, object_flags=BASELINE_FLAGS & ~1, linkage_adapter=owner['linkage_adapter'], runtime=GP_SPEC['runtime'])
        except (OSError, ValueError, subprocess.TimeoutExpired) as exc:
            result['reason'] = str(exc) if isinstance(exc, ValueError) else type(exc).__name__
    return result


def verify(ledger, game, compiler, output_dir, root=ROOT, ee_compiler=None,
           historical_compiler=None, historical_assembler=None):
    validate_ledger(ledger, root)
    owners = scalar_owners(ledger, root)
    ordered, members = call_graph(ledger)
    baseline = baseline_elf(game)
    gp_evidence = gp_context_evidence(baseline, root) if owners else None
    original_owners = {key: scalar_owner_original(baseline, owner) for key, owner in owners.items()}
    originals = {item['id']: baseline.original(item) for item in ledger['functions']}
    for item in ordered:
        if item.get('comparison_mode') in DATA_MODES:
            original_graph_body(baseline, item, strict_intersections=True)
            owner = owners[item['data_binding']['owner_id']]
            evidence = scalar_relocation_evidence(item, owner)
            displacement = evidence[0]['displacement'] if item['comparison_mode'] in (SCALAR_V2_MODE, FLOAT_MODE, BYTE_MODE) else evidence['displacement']
            require(scalar_body(originals[item['id']], item['data_binding']['operation'], mode=item['comparison_mode'])
                    == displacement, 'original scalar access targets a different owner')
        if item.get('toolchain') == HIST_FLAVOR:
            original_graph_body(baseline, item)
            audit_historical_body(originals[item['id']])
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
    toolchains = {flavor: toolchain_identity(flavor, selected[flavor], env)
                  for flavor in sorted(flavors) if flavor != HIST_FLAVOR}
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    if HIST_FLAVOR in flavors:
        with tempfile.TemporaryDirectory(prefix='historical-identity-', dir=output_dir) as temporary:
            hist_env = dict(env, TMPDIR=str(Path(temporary).resolve()))
            toolchains[HIST_FLAVOR] = historical_identity(historical_compiler, historical_assembler, hist_env, root)
            historical_compiler = toolchains[HIST_FLAVOR][0]
            historical_assembler = historical_layout(root)[1]
    if owners and ledger['schema_version'] >= 3:
        toolchains = {flavor: (executable, json.loads(canonical(identity))) for flavor, (executable, identity) in toolchains.items()}
    report = {'schema_version': 8 if owners and ledger['schema_version'] == 5 else 7 if owners and ledger['schema_version'] == 4 else 6 if owners and ledger['schema_version'] == 3 else 5 if owners else 4 if HIST_FLAVOR in flavors else 3 if members else 2, 'scope': SCOPE, 'target': TARGET['id'], 'baseline_sha256': TARGET['baseline_sha256'],
              'ledger_sha256': digest(canonical(ledger)), 'verifier_sha256': digest(Path(__file__).read_bytes()),
              'recorded_at': datetime.now(timezone.utc).isoformat(),
              'toolchains': {flavor: value[1] for flavor, value in toolchains.items()}, 'functions': []}
    if members:
        report['call_policy'] = CALL_POLICY
        report['call_policy_sha256'] = digest(canonical(CALL_POLICY))
    if HIST_FLAVOR in flavors:
        report['historical_policy'] = HIST_POLICY
        report['historical_policy_sha256'] = digest(canonical(HIST_POLICY))
    snapshots = {item['id']: source_bytes(local_path(root, item['source'])) for item in ordered + list(owners.values())}
    owner_reviews = {key: scalar_review(ledger, owner, root) for key, owner in owners.items()}
    owner_results = {}
    if owners:
        report['gp_context'] = gp_evidence
        if any(not float_owner(o) and not byte_owner(o) for o in owners.values()):
            policy = scalar_policy(ledger)
            report.update(scalar_policy=policy, scalar_policy_sha256=digest(canonical(policy)))
        if ledger['schema_version'] in (4, 5):
            report.update(float_store_policy=FLOAT_POLICY, float_store_policy_sha256=digest(canonical(FLOAT_POLICY)))
        if ledger['schema_version'] == 5:
            report.update(byte_store_policy=BYTE_POLICY, byte_store_policy_sha256=digest(canonical(BYTE_POLICY)))
        for key, owner in owners.items():
            owner_results[key] = compile_scalar_owner(owner, original_owners[key], snapshots[key],
                toolchains['ee-gcc'][0], toolchains['ee-gcc'][1], owner_reviews[key], gp_evidence, output_dir, root, env)
    completed = {}
    for item in ordered:
        flavor = item.get('toolchain', 'clang')
        executable, identity = toolchains[flavor]
        source = local_path(root, item['source'])
        contents = snapshots[item['id']]
        result = {'id': item['id'], 'source_sha256': digest(contents), 'compiler_args': effective_args(item),
                  'toolchain': flavor, 'toolchain_sha256': digest(canonical(identity)), 'status': 'failed'}
        with tempfile.TemporaryDirectory(prefix=item['id'] + '-', dir=output_dir) as directory:
            directory = Path(directory).resolve()
            snapshot, obj = directory / 'source.cpp', directory / 'fresh.o'
            snapshot.write_bytes(contents)
            command = [str(executable)] + item['compiler_args'] + FIXED_FLAGS + ['-c', str(snapshot.resolve()), '-o', str(obj.resolve())]
            try:
                if item.get('comparison_mode') in DATA_MODES:
                    require(owner_results[item['data_binding']['owner_id']]['status'] == 'matched',
                            'source-backed scalar owner did not freshly match')
                if item.get('comparison_mode') == DIRECT_MODE:
                    for binding in item['call_bindings']:
                        callee = completed[binding['callee_id']]
                        require(callee['status'] == 'matched' and callee['original_sha256'] == binding['body_sha256']
                                and callee['rebuilt_sha256'] == binding['body_sha256'],
                                'source-backed dependency did not freshly match')
                if flavor == 'ee-gcc':
                    require(toolchain_identity(flavor, executable, env)[1] == identity, 'EE GCC components changed before compilation')
                    result['data_model'] = ee_data_model(executable, item['compiler_args'], env)
                    if item.get('comparison_mode') == FLOAT_MODE:
                        result['float_model'] = ee_data_model(executable, item['compiler_args'], env, FLOAT_MODEL)
                if item.get('comparison_mode') == BYTE_MODE:
                    result['byte_model'] = ee_data_model(executable, item['compiler_args'], env, BYTE_MODEL)
                if flavor == HIST_FLAVOR:
                    hist_env = dict(env, TMPDIR=str(directory))
                    require(historical_identity(executable, historical_assembler, hist_env, root)[1] == identity,
                            'historical components changed before compilation')
                    result['data_model'] = historical_data_model(executable, historical_assembler, directory, hist_env)
                    assembly_hash, log = historical_compile(executable, historical_assembler, snapshot, obj, directory, hist_env)
                    result.update(comparison_mode=HIST_MODE, control_flow_policy=HIST_MODE,
                                  assembly_sha256=assembly_hash, assembler_args=HIST_AS_FLAGS + ['-o', '<fresh-object.o>'],
                                  object_flags=HIST_OBJECT_FLAGS)
                    require(historical_identity(executable, historical_assembler, hist_env, root)[1] == identity,
                            'historical components or specs changed during compilation')
                    compiled = subprocess.CompletedProcess(command, 0, log, b'')
                else:
                    compiled = subprocess.run(command, cwd=directory, capture_output=True, timeout=120, check=False, env=env)
                require(source.read_bytes() == contents and snapshot.read_bytes() == contents, 'source changed during compilation')
                require(digest(executable.read_bytes()) == identity['sha256'], 'compiler changed during compilation')
                if flavor == 'ee-gcc':
                    require(toolchain_identity(flavor, executable, env)[1] == identity, 'EE GCC components or specs changed during compilation')
                (output_dir / (item['id'] + '.log')).write_bytes(compiled.stdout + compiled.stderr)
                require(compiled.returncode == 0, 'compilation failed; inspect private build log')
                if item.get('comparison_mode') in DATA_MODES:
                    require(obj.is_file() and not obj.is_symlink(), 'scalar consumer needs fresh regular object')
                object_data = obj.read_bytes()
                elf = Elf32(object_data)
                if flavor == 'ee-gcc':
                    require(elf.flags == BASELINE_FLAGS, 'EE GCC object flags differ from reviewed R5900/EABI64 target')
                direct = item.get('comparison_mode') == DIRECT_MODE
                if item.get('comparison_mode') in DATA_MODES:
                    binding = item['data_binding']; owner = owners[binding['owner_id']]
                    raw, _ = elf.rebuilt(item['symbol'], scalar_binding=binding, scalar_mode=item['comparison_mode'])
                    code, evidence = relocate_scalar(raw, originals[item['id']], item, owner)
                    result[relocation_key(item['comparison_mode'])] = evidence
                    if item['comparison_mode'] == FLOAT_MODE:
                        result['float_abi'] = float_abi(elf)
                        result['object_flags'] = BASELINE_FLAGS
                    if item['comparison_mode'] == BYTE_MODE:
                        result['byte_abi'] = float_abi(elf)
                        result['object_flags'] = BASELINE_FLAGS
                    result.update(comparison_mode=item['comparison_mode'],
                                  raw_rebuilt_sha256=digest(raw), owner_id=owner['id'],
                                  owner_result_sha256=digest(canonical(owner_results[owner['id']])),
                                  gp_context_sha256=gp_evidence['context_sha256'])
                elif direct:
                    raw, _ = elf.rebuilt(item['symbol'], item['call_bindings'])
                    code, evidence = relocate_calls(raw, originals[item['id']], item)
                    result.update({'comparison_mode': DIRECT_MODE, 'link_address': item['address'],
                                   'raw_rebuilt_sha256': digest(raw), 'relocations': evidence,
                                   'dependencies': {b['callee_id']: digest(canonical(completed[b['callee_id']]))
                                                    for b in item['call_bindings']}})
                else:
                    code = elf.rebuilt(item['symbol'])
                    if flavor == HIST_FLAVOR:
                        require(elf.flags == HIST_OBJECT_FLAGS, 'historical object flags differ from reviewed generic MIPS3/o64 profile')
                        elf.call_symbols(elf.function(item['symbol'])[0], [], historical=True)
                        audit_historical_body(code)
                    if item['id'] in members:
                        audit_integer_body(code)
                if item['id'] in members:
                    result['control_flow_policy'] = DIRECT_MODE
                require(len(code) == item['size'], 'rebuilt extent does not match the complete original function')
                original = originals[item['id']]
                result.update({'object_sha256': digest(object_data), 'size': len(code),
                               'original_sha256': digest(original), 'rebuilt_sha256': digest(code),
                               'status': 'matched' if code == original else 'mismatched'})
                if flavor == HIST_FLAVOR:
                    result['historical_evidence_sha256'] = historical_evidence(result)
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
    if HIST_FLAVOR in flavors:
        with tempfile.TemporaryDirectory(prefix='historical-final-', dir=output_dir) as temporary:
            hist_env = dict(env, TMPDIR=str(Path(temporary).resolve()))
            stable = historical_identity(historical_compiler, historical_assembler, hist_env, root)[1] == toolchains[HIST_FLAVOR][1]
        stable = stable and all(source_bytes(local_path(root, item['source'])) == snapshots[item['id']]
                                for item in ordered if item.get('toolchain') == HIST_FLAVOR)
        if not stable:
            for item in ordered:
                if item.get('toolchain') == HIST_FLAVOR:
                    completed[item['id']].update(status='failed', reason='historical source or compiler changed during verification')
    if owners:
        scalar_items = list(owners.values()) + [i for i in ordered if i.get('comparison_mode') in DATA_MODES]
        stable = all(source_bytes(local_path(root, i['source'])) == snapshots[i['id']] for i in scalar_items)
        stable = stable and toolchain_identity('ee-gcc', toolchains['ee-gcc'][0], env)[1] == toolchains['ee-gcc'][1]
        stable = stable and gp_descriptor(root) == gp_evidence['context_sha256']
        stable = stable and all(scalar_review(ledger, owner, root) == owner_reviews[key] for key, owner in owners.items())
        if not stable:
            for result in list(owner_results.values()) + [completed[i['id']] for i in scalar_items if i['id'] in completed]:
                result.update(status='failed', reason='scalar source, review or compiler changed during group verification')
        report['data_objects'] = [owner_results[key] for key in owners]
    report['functions'] = [completed[item['id']] for item in ledger['functions']]
    return report


def validate_identity(identity, flavor, root=ROOT):
    require(isinstance(identity, dict) and isinstance(identity.get('name'), str)
            and re.fullmatch(r'[A-Za-z0-9_.+-]+', identity['name'])
            and isinstance(identity.get('sha256'), str) and HASH.fullmatch(identity['sha256'])
            and identity.get('flavor') == flavor, 'invalid recorded compiler identity')
    if flavor == 'clang':
        require(isinstance(identity.get('version'), str)
                and re.fullmatch(r'(?:(?:Apple|Homebrew) )?clang version [A-Za-z0-9. +-]+', identity['version']), 'invalid recorded Clang version')
    elif flavor == HIST_FLAVOR:
        require(identity.get('target') == HIST_TARGET and identity.get('name') == 'xgcc'
                and identity.get('version') == HIST_VERSION
                and identity.get('assembler_version') == 'GNU assembler (GNU Binutils) 2.45.1'
                and identity.get('search_policy') == HIST_LAYOUT['search_policy'],
                'invalid recorded historical toolchain profile')
        descriptor = (Path(root) / HIST_DESCRIPTOR).read_bytes()
        require(json.loads(descriptor) == HIST_LAYOUT and identity.get('descriptor_sha256') == digest(descriptor),
                'stale historical build descriptor')
        components = identity.get('components')
        require(isinstance(components, dict) and set(components) == {'cpp', 'cc1plus', 'as'},
                'missing historical compiler components')
        for name, component in components.items():
            require(isinstance(component, dict) and component.get('name') == name
                    and isinstance(component.get('sha256'), str) and HASH.fullmatch(component['sha256']),
                    'invalid recorded historical component identity')
        require(isinstance(identity.get('specs_sha256'), str) and HASH.fullmatch(identity['specs_sha256']),
                'missing historical specs identity')
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


def recorded_scalar_owners(report, ledger, identities, root):
    owners = scalar_owners(ledger, root)
    if not owners:
        require(not any(k in report for k in ('data_objects', 'scalar_policy', 'scalar_policy_sha256', 'float_store_policy', 'float_store_policy_sha256', 'byte_store_policy', 'byte_store_policy_sha256', 'gp_context')), 'unexpected scalar receipt evidence')
        return {}
    context = {'context_sha256': gp_descriptor(root), 'startup_context_sha256': GP_SPEC['startup']['context_sha256'],
               'gp': GP_SPEC['gp_symbol']['address'], 'runtime': GP_SPEC['runtime']}
    require(report['schema_version'] == (8 if ledger['schema_version'] == 5 else 7 if ledger['schema_version'] == 4 else 6 if ledger['schema_version'] == 3 else 5)
            and report.get('gp_context') == context, 'missing data receipt policy/context')
    if any(not float_owner(o) and not byte_owner(o) for o in owners.values()):
        policy = scalar_policy(ledger)
        require(report.get('scalar_policy') == policy and report.get('scalar_policy_sha256') == digest(canonical(policy)),
                'missing scalar receipt policy')
    else:
        require('scalar_policy' not in report and 'scalar_policy_sha256' not in report, 'float-only receipt cannot claim scalar policy')
    if ledger['schema_version'] in (4, 5):
        require(type(report['schema_version']) is int, 'float receipt schema must be an integer')
        require(canonical(report.get('float_store_policy')) == canonical(FLOAT_POLICY)
                and report.get('float_store_policy_sha256') == digest(canonical(FLOAT_POLICY)), 'missing float store policy')
    else:
        require('float_store_policy' not in report and 'float_store_policy_sha256' not in report, 'unexpected float store policy')
    if ledger['schema_version'] == 5:
        require(canonical(report.get('byte_store_policy')) == canonical(BYTE_POLICY)
                and report.get('byte_store_policy_sha256') == digest(canonical(BYTE_POLICY)), 'missing byte store policy')
    else:
        require('byte_store_policy' not in report and 'byte_store_policy_sha256' not in report, 'unexpected byte store policy')
    results = report.get('data_objects')
    require(isinstance(results, list) and len(results) == len(owners), 'missing source owner receipts')
    for result, owner in zip(results, owners.values()):
        require(result.get('id') == owner['id'] and result.get('toolchain') == 'ee-gcc'
                and result.get('toolchain_sha256') == digest(canonical(identities['ee-gcc']))
                and result.get('source_sha256') == digest(source_bytes(local_path(root, owner['source'])))
                and result.get('compiler_args') == effective_args(owner)
                and result.get('review_sha256') == scalar_review(ledger, owner, root)
                and result.get('gp_context_sha256') == context['context_sha256'], 'stale scalar owner source or provenance')
        require(result.get('status') in ('matched', 'mismatched', 'failed'), 'invalid owner status')
        if byte_owner(owner):
            require(not any(k in result for k in ('float_model', 'float_abi', 'float_store_relocations', 'scalar_relocation', 'scalar_relocations', 'byte_store_relocations')),
                    'byte owner cannot claim other-lane or consumer evidence')
        if not byte_owner(owner):
            require(not any(k in result for k in ('byte_model', 'byte_abi', 'byte_store_relocations')), 'nonbyte owner cannot claim byte evidence')
        if result['status'] != 'matched':
            continue
        if byte_owner(owner):
            require(canonical(result.get('byte_model')) == canonical(BYTE_MODEL)
                    and canonical(result.get('byte_abi')) == canonical(FLOAT_ABI)
                    and result.get('comparison_mode') == BYTE_MODE and result.get('data_type') == 'uint8'
                    and all(type(result.get(k)) is int for k in ('size', 'object_flags', 'generated_binding', 'initializer'))
                    and not any(k in result for k in ('float_model', 'float_abi', 'float_store_relocations', 'scalar_relocation', 'scalar_relocations')),
                    'missing byte owner model/ABI evidence')
        elif float_owner(owner):
            require(canonical(result.get('float_model')) == canonical(FLOAT_MODEL)
                    and canonical(result.get('float_abi')) == canonical(FLOAT_ABI)
                    and result.get('comparison_mode') == FLOAT_MODE and result.get('data_type') == 'float32'
                    and type(result.get('size')) is int and type(result.get('object_flags')) is int
                    and type(result.get('generated_binding')) is int,
                    'missing float owner model/ABI evidence')
        else:
            require(not any(k in result for k in ('float_model', 'float_abi', 'data_type', 'comparison_mode')),
                    'integer owner cannot claim float evidence')
        original = result.get('original', {})
        expected = {'symbol': owner['original_symbol'], 'address': owner['address'], 'size': owner_width(owner),
                    'binding': owner['original_binding'], 'section': '.sdata', 'initializer_sha256': digest(owner_initializer(owner))}
        require((canonical({k: original.get(k) for k in expected}) == canonical(expected) if float_owner(owner) or byte_owner(owner)
                 else all(original.get(k) == v for k, v in expected.items()))
                and all(type(original.get(k)) is int and original[k] >= 0 for k in ('section_index', 'file_offset', 'load_index'))
                and original['section_index'] > 0 and original['file_offset'] >= 52
                and result.get('data_model') == EE_MODEL and result.get('size') == owner_width(owner)
                and result.get('storage') == owner['storage'] and result.get('initializer') == owner['initializer']
                and result.get('rebuilt_sha256') == digest(owner_initializer(owner)) and result.get('generated_binding') == 1
                and result.get('object_flags') == BASELINE_FLAGS & ~1
                and result.get('linkage_adapter') == owner['linkage_adapter'] and result.get('runtime') == GP_SPEC['runtime']
                and isinstance(result.get('object_sha256'), str) and HASH.fullmatch(result['object_sha256']),
                'inconsistent scalar owner match evidence')
    return {r['id']: r for r in results}


def recorded_matches(report, ledger, root=ROOT):
    validate_ledger(ledger, root)
    ordered, members = call_graph(ledger)
    require(report.get('schema_version') in (1, 2, 3, 4, 5, 6, 7, 8) and report.get('scope') == SCOPE, 'unsupported comparison receipt')
    require(type(report.get('schema_version')) is int, 'receipt schema must be an integer')
    require((report['schema_version'] == 8) == (ledger['schema_version'] == 5),
            'byte receipt version requires ledger5 and vice versa')
    if members:
        require(report['schema_version'] in (3, 4, 5, 6, 7, 8) and report.get('call_policy') == CALL_POLICY
                and report.get('call_policy_sha256') == digest(canonical(CALL_POLICY)), 'missing direct-call receipt policy')
    require(report.get('target') == TARGET['id'] and report.get('baseline_sha256') == TARGET['baseline_sha256'], 'receipt targets different baseline')
    require(report.get('ledger_sha256') == digest(canonical(ledger)), 'stale receipt: ledger changed')
    require(report.get('verifier_sha256') == digest(Path(__file__).read_bytes()), 'stale receipt: verifier changed')
    flavors = {item.get('toolchain', 'clang') for item in ledger['functions']}
    if HIST_FLAVOR in flavors:
        require(report['schema_version'] in (4, 5, 6, 7, 8) and report.get('historical_policy') == HIST_POLICY
                and report.get('historical_policy_sha256') == digest(canonical(HIST_POLICY)),
                'missing historical comparison receipt policy')
    if report['schema_version'] == 1:
        require(flavors == {'clang'}, 'legacy receipts cannot claim EE GCC comparisons')
        identity = dict(report.get('compiler', {}))
        identity['flavor'] = 'clang'
        identities = {'clang': identity}
    else:
        identities = report.get('toolchains')
        require(isinstance(identities, dict) and set(identities) == flavors, 'receipt toolchain inventory mismatch')
    for flavor, identity in identities.items():
        validate_identity(identity, flavor, root)
    owner_results = recorded_scalar_owners(report, ledger, identities, root)
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
        if item.get('comparison_mode') == BYTE_MODE:
            require(not any(k in result for k in ('float_model', 'float_abi', 'float_store_relocations', 'scalar_relocation', 'scalar_relocations')),
                    'byte consumer cannot claim other-lane evidence')
        if item.get('comparison_mode') != BYTE_MODE:
            require(not any(k in result for k in ('byte_model', 'byte_abi', 'byte_store_relocations')), 'nonbyte function cannot claim byte evidence')
        if result['status'] == 'matched':
            if item.get('comparison_mode') == BYTE_MODE:
                require(canonical(result.get('byte_model')) == canonical(BYTE_MODEL)
                        and canonical(result.get('byte_abi')) == canonical(FLOAT_ABI)
                        and type(result.get('object_flags')) is int and result['object_flags'] == BASELINE_FLAGS
                        and type(result.get('size')) is int, 'missing byte consumer model/ABI evidence')
            if item.get('comparison_mode') == FLOAT_MODE:
                require(canonical(result.get('float_model')) == canonical(FLOAT_MODEL)
                    and canonical(result.get('float_abi')) == canonical(FLOAT_ABI)
                        and type(result.get('object_flags')) is int and result['object_flags'] == BASELINE_FLAGS
                        and type(result.get('size')) is int,
                        'missing float consumer model/ABI evidence')
            else:
                require('float_model' not in result and 'float_abi' not in result and 'float_store_relocations' not in result,
                        'ordinary receipt cannot claim float evidence')
            if flavor == 'ee-gcc':
                require(result.get('data_model') == EE_MODEL, 'missing recorded EE GCC data-model evidence')
            if flavor == HIST_FLAVOR:
                require(result.get('data_model') == historical_model_record()
                        and result.get('comparison_mode') == HIST_MODE and result.get('control_flow_policy') == HIST_MODE
                        and result.get('object_flags') == HIST_OBJECT_FLAGS
                        and result.get('assembler_args') == HIST_AS_FLAGS + ['-o', '<fresh-object.o>']
                        and isinstance(result.get('assembly_sha256'), str) and HASH.fullmatch(result['assembly_sha256']),
                        'missing historical compilation/model/audit evidence')
                require(result.get('historical_evidence_sha256') == historical_evidence(result),
                        'inconsistent historical stage evidence')
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
        elif item.get('comparison_mode') in DATA_MODES:
            owner = next(o for o in ledger['data_objects'] if o['id'] == item['data_binding']['owner_id'])
            dependency = owner_results[owner['id']]
            require(dependency['status'] == 'matched' and result.get('owner_id') == owner['id']
                    and result.get('owner_result_sha256') == digest(canonical(dependency))
                    and result.get('comparison_mode') == item['comparison_mode']
                    and (canonical(result.get(relocation_key(item['comparison_mode']))) == canonical(scalar_relocation_evidence(item, owner))
                         if item['comparison_mode'] in (FLOAT_MODE, BYTE_MODE) else
                         result.get(relocation_key(item['comparison_mode'])) == scalar_relocation_evidence(item, owner))
                    and not any(k in result for k in {'scalar_relocation', 'scalar_relocations', 'float_store_relocations', 'byte_store_relocations'}
                                - {relocation_key(item['comparison_mode'])})
                    and result.get('gp_context_sha256') == gp_descriptor(root)
                    and isinstance(result.get('raw_rebuilt_sha256'), str) and HASH.fullmatch(result['raw_rebuilt_sha256'])
                    and 'relocations' not in result and 'dependencies' not in result,
                    'missing or stale source-backed scalar dependency')
        elif item.get('comparison_mode') == HIST_MODE:
            require('relocations' not in result and 'dependencies' not in result, 'historical leaf cannot claim linked dependencies')
        else:
            require('comparison_mode' not in result and 'relocations' not in result and 'dependencies' not in result,
                    'leaf receipt cannot claim relocated comparison')
    return matched


def invalidate_report(destination, ledger_path, ledger, game, compiler, root=ROOT, ee_compiler=None,
                      historical_compiler=None, historical_assembler=None):
    destination = Path(destination)
    require(destination.suffix == '.json' and not destination.is_symlink(), 'receipt must be a regular JSON path')
    protected = {Path(ledger_path).resolve(), Path(game).resolve(), Path(__file__).resolve()}
    protected.add((Path(root) / HIST_DESCRIPTOR).resolve())
    for selected in (compiler, ee_compiler, historical_compiler, historical_assembler):
        resolved_compiler = shutil.which(str(selected)) if selected else None
        if resolved_compiler:
            protected.add(Path(resolved_compiler).resolve())
    protected.update(local_path(root, item['source']) for item in ledger['functions'] + ledger.get('data_objects', []))
    protected.add((Path(root) / GP_CONTEXT).resolve())
    protected.update(local_path(root, item['review']) for item in ledger.get('data_objects', []))
    require(destination.resolve() not in protected, 'receipt path collides with a protected input')
    if destination.exists():
        previous = json.loads(destination.read_text(encoding='utf-8'))
        require(isinstance(previous, dict) and type(previous.get('schema_version')) is int and previous.get('schema_version') in (1, 2, 3, 4, 5, 6, 7, 8)
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
            command.add_argument('--require-matches', action='store_true', help='Require a receipt with every ledger function and source-backed data owner recorded as matched')
        if name == 'verify':
            command.add_argument('--game', type=Path, required=True)
            command.add_argument('--compiler', help='Clang compiler for default/clang entries')
            command.add_argument('--ee-compiler', help='Isolated EE GCC g++ for ee-gcc entries')
            command.add_argument('--historical-compiler', type=Path, help='Reviewed fixed-layout GCC 2.95.2 xgcc for isolated integer leaves')
            command.add_argument('--historical-assembler', type=Path, help='Reviewed fixed-layout EE assembler for historical generated assembly')
            command.add_argument('--output-dir', type=Path, default=ROOT / 'work/verifier')
    args = parser.parse_args(argv)
    try:
        ledger = load_ledger(args.ledger)
        if args.command == 'verify':
            destination = args.report or args.output_dir / 'verification.json'
            invalidate_report(destination, args.ledger, ledger, args.game, args.compiler, ee_compiler=args.ee_compiler,
                              historical_compiler=args.historical_compiler, historical_assembler=args.historical_assembler)
        if args.command == 'check':
            require(not args.require_matches or args.report is not None, '--require-matches requires --report')
            report = json.loads(args.report.read_text()) if args.report else None
            count = len(recorded_matches(report, ledger)) if report is not None else 0
            owner_count = sum(r['status'] == 'matched' for r in report.get('data_objects', [])) if report is not None else 0
            require(not args.require_matches or (count == len(ledger['functions']) and owner_count == len(ledger.get('data_objects', []))),
                    'receipt does not record a match for every ledger function and source-backed data owner')
            print(f'Record checks passed: {len(ledger["functions"])} candidates, {count} recorded function matches, {owner_count} recorded data owner matches; no fresh private-byte verification.')
            return 0
        report = verify(ledger, args.game, args.compiler, args.output_dir, ee_compiler=args.ee_compiler,
                        historical_compiler=args.historical_compiler, historical_assembler=args.historical_assembler)
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
        count = len(recorded_matches(report, ledger))
        owner_count = sum(r['status'] == 'matched' for r in report.get('data_objects', []))
        print(f'Fresh private comparison: {count}/{len(ledger["functions"])} complete symbol-bounded functions matched. {owner_count} source-backed data owners matched separately. No linked-game or runtime proof.')
        return 0 if count == len(ledger['functions']) and owner_count == len(ledger.get('data_objects', [])) else 1
    except (OSError, ValueError, KeyError, TypeError, subprocess.TimeoutExpired) as exc:
        print('Verification failed: ' + str(exc), file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
