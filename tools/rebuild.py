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
REQUIRED_FLAGS = {'--no-default-config', '--target=mipsel-none-elf', '-march=mips3', '-mabi=32',
                  '-ffreestanding', '-fno-pic', '-mno-abicalls', '-fno-stack-protector',
                  '-fno-exceptions', '-fno-rtti'}
ALLOWED_FLAGS = REQUIRED_FLAGS | {'-O0', '-O1', '-O2', '-O3', '-Os', '-Oz',
    '-fomit-frame-pointer', '-fno-omit-frame-pointer', '-fno-builtin',
    '-fno-unwind-tables', '-fno-asynchronous-unwind-tables', '-msoft-float',
    '-mhard-float', '-fno-strict-aliasing', '-fwrapv', '-std=c++17'}
FIXED_FLAGS = ['-nostdinc', '-nostdinc++', '-x', 'c++']
INJECTING_ENV = ('CCC_OVERRIDE_OPTIONS', 'CL', '_CL_', 'CPATH', 'CPLUS_INCLUDE_PATH',
                 'C_INCLUDE_PATH', 'OBJC_INCLUDE_PATH', 'COMPILER_PATH', 'GCC_EXEC_PREFIX',
                 'LIBRARY_PATH', 'SDKROOT', 'DYLD_INSERT_LIBRARIES', 'LD_PRELOAD')


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

    def rebuilt(self, name):
        require(self.kind == 1, 'compiler must emit relocatable object')
        symbol, code = self.function(name)
        require(symbol['binding'] == 1 and symbol['other'] == 0, 'candidate must be one global default-visibility function')
        functions = [s for s in self.symbols if s['type'] == 2]
        require(len(functions) == 1, 'extra object functions are unsupported')
        require(not any(s['section'] == 0 and s['name'] for s in self.symbols), 'undefined object symbols are unsupported')
        for i, section in enumerate(self.sections):
            if section[1] in (4, 9) and section[5]:
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
        return code


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
        require(len(flags) == len(set(flags)) and REQUIRED_FLAGS <= set(flags) <= ALLOWED_FLAGS, 'unsupported or missing compiler flags')
        require(sum(f.startswith('-O') for f in flags) == 1, 'exactly one optimization setting is required')
        require(not ({'-msoft-float', '-mhard-float'} <= set(flags)) and not ({'-fomit-frame-pointer', '-fno-omit-frame-pointer'} <= set(flags)), 'conflicting compiler arguments')
        ids.add(ident)
        sources.add(source)
        extents.append((lo, lo + size))
    extents.sort()
    require(all(a[1] <= b[0] for a, b in zip(extents, extents[1:])), 'ledger function extents overlap')
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


def verify(ledger, game, compiler, output_dir, root=ROOT):
    validate_ledger(ledger, root)
    baseline = baseline_elf(game)
    originals = {item['id']: baseline.original(item) for item in ledger['functions']}
    for name in INJECTING_ENV:
        require(not os.environ.get(name), 'unset ' + name + '; unrecorded compiler environment injection is unsupported')
    executable = shutil.which(str(compiler))
    require(executable is not None, 'compiler not found')
    executable = Path(executable).resolve()
    compiler_hash = digest(executable.read_bytes())
    # Do not inherit ambient compiler paths, config, include paths or dylib hooks.
    env = {'PATH': os.defpath, 'LANG': 'C', 'LC_ALL': 'C'}
    probe = subprocess.run([str(executable), '--no-default-config', '--version'], capture_output=True, timeout=15, check=False, env=env)
    require(probe.returncode == 0, 'compiler version probe failed')
    version = probe.stdout.decode('utf-8', errors='replace').splitlines()
    require(version and re.fullmatch(r'(?:(?:Apple|Homebrew) )?clang version [A-Za-z0-9. +-]+', version[0]), 'compiler identity must be a sanitized Clang version')
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    report = {'schema_version': 1, 'scope': SCOPE, 'target': TARGET['id'], 'baseline_sha256': TARGET['baseline_sha256'],
              'ledger_sha256': digest(canonical(ledger)), 'verifier_sha256': digest(Path(__file__).read_bytes()),
              'recorded_at': datetime.now(timezone.utc).isoformat(),
              'compiler': {'name': executable.name, 'sha256': compiler_hash, 'version': version[0]}, 'functions': []}
    for item in ledger['functions']:
        source = local_path(root, item['source'])
        contents = source_bytes(source)
        result = {'id': item['id'], 'source_sha256': digest(contents), 'compiler_args': effective_args(item), 'status': 'failed'}
        # A new isolated directory prevents successful exit from reusing a stale object.
        with tempfile.TemporaryDirectory(prefix=item['id'] + '-', dir=output_dir) as directory:
            directory = Path(directory)
            snapshot, obj = directory / 'source.cpp', directory / 'fresh.o'
            snapshot.write_bytes(contents)
            command = [str(executable)] + item['compiler_args'] + FIXED_FLAGS + ['-c', str(snapshot.resolve()), '-o', str(obj.resolve())]
            try:
                compiled = subprocess.run(command, cwd=directory, capture_output=True, timeout=120, check=False, env=env)
                require(source.read_bytes() == contents and snapshot.read_bytes() == contents, 'source changed during compilation')
                require(digest(executable.read_bytes()) == compiler_hash, 'compiler changed during compilation')
                (output_dir / (item['id'] + '.log')).write_bytes(compiled.stdout + compiled.stderr)
                require(compiled.returncode == 0, 'compilation failed; inspect private build log')
                object_data = obj.read_bytes()
                code = Elf32(object_data).rebuilt(item['symbol'])
                require(len(code) == item['size'], 'rebuilt extent does not match the complete original function')
                original = originals[item['id']]
                result.update({'object_sha256': digest(object_data), 'size': len(code),
                               'original_sha256': digest(original), 'rebuilt_sha256': digest(code),
                               'status': 'matched' if code == original else 'mismatched'})
            except (OSError, ValueError, subprocess.TimeoutExpired) as exc:
                result['reason'] = str(exc) if isinstance(exc, ValueError) else type(exc).__name__
        report['functions'].append(result)
    return report


def recorded_matches(report, ledger, root=ROOT):
    validate_ledger(ledger, root)
    require(report.get('schema_version') == 1 and report.get('scope') == SCOPE, 'unsupported comparison receipt')
    require(report.get('target') == TARGET['id'] and report.get('baseline_sha256') == TARGET['baseline_sha256'], 'receipt targets different baseline')
    require(report.get('ledger_sha256') == digest(canonical(ledger)), 'stale receipt: ledger changed')
    require(report.get('verifier_sha256') == digest(Path(__file__).read_bytes()), 'stale receipt: verifier changed')
    compiler = report.get('compiler', {})
    require(isinstance(compiler, dict) and isinstance(compiler.get('name'), str) and re.fullmatch(r'[A-Za-z0-9_.+-]+', compiler['name'])
            and isinstance(compiler.get('sha256'), str) and HASH.fullmatch(compiler['sha256'])
            and isinstance(compiler.get('version'), str) and re.fullmatch(r'(?:(?:Apple|Homebrew) )?clang version [A-Za-z0-9. +-]+', compiler['version']), 'invalid recorded compiler identity')
    require(isinstance(report.get('recorded_at'), str), 'missing comparison time')
    datetime.fromisoformat(report['recorded_at'])
    results = report.get('functions')
    require(isinstance(results, list) and len(results) == len(ledger['functions']), 'missing or extra function receipt')
    matched = []
    for result, item in zip(results, ledger['functions']):
        require(result.get('id') == item['id'], 'receipt function identity mismatch')
        require(result.get('source_sha256') == digest(source_bytes(local_path(root, item['source'])))
                and result.get('compiler_args') == effective_args(item), 'stale receipt: source or compiler args changed')
        require(result.get('status') in ('matched', 'mismatched', 'failed'), 'invalid comparison status')
        if result['status'] == 'matched':
            require(result.get('size') == item['size'] and all(isinstance(result.get(key), str) and HASH.fullmatch(result[key])
                    for key in ('object_sha256', 'original_sha256', 'rebuilt_sha256'))
                    and result['original_sha256'] == result['rebuilt_sha256'], 'inconsistent match receipt')
            matched.append(item)
    return matched


def invalidate_report(destination, ledger_path, ledger, game, compiler, root=ROOT):
    destination = Path(destination)
    require(destination.suffix == '.json' and not destination.is_symlink(), 'receipt must be a regular JSON path')
    protected = {Path(ledger_path).resolve(), Path(game).resolve(), Path(__file__).resolve()}
    resolved_compiler = shutil.which(str(compiler))
    if resolved_compiler:
        protected.add(Path(resolved_compiler).resolve())
    protected.update(local_path(root, item['source']) for item in ledger['functions'])
    require(destination.resolve() not in protected, 'receipt path collides with a protected input')
    if destination.exists():
        previous = json.loads(destination.read_text(encoding='utf-8'))
        require(isinstance(previous, dict) and previous.get('schema_version') == 1
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
            command.add_argument('--compiler', required=True)
            command.add_argument('--output-dir', type=Path, default=ROOT / 'work/verifier')
    args = parser.parse_args(argv)
    try:
        ledger = load_ledger(args.ledger)
        if args.command == 'verify':
            destination = args.report or args.output_dir / 'verification.json'
            invalidate_report(destination, args.ledger, ledger, args.game, args.compiler)
        if args.command == 'check':
            require(not args.require_matches or args.report is not None, '--require-matches requires --report')
            count = len(recorded_matches(json.loads(args.report.read_text()), ledger)) if args.report else 0
            require(not args.require_matches or count == len(ledger['functions']), 'receipt does not record a match for every ledger function')
            print(f'Record checks passed: {len(ledger["functions"])} candidates, {count} recorded matches; no fresh private-byte verification.')
            return 0
        report = verify(ledger, args.game, args.compiler, args.output_dir)
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
