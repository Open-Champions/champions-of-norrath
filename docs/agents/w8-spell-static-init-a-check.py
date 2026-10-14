"""Compare the authored static initializers with a private hash-pinned executable.

For each w8-spell-static-init-a-* unit this builds the host contract, runs it, and
independently replays the original wrapper and its unit-local initializer with a
small register tracker (constants, stack stores, gp-relative float loads). The
particle constructor arguments, call order and record symbols must agree. Nothing
from the original is written out; only pass/fail lines are printed. This grants
no runtime or original-byte match.

  python3 docs/agents/w8-spell-static-init-a-check.py \
      --original /path/to/private/SLUS_205.65 --toolchain /path/to/ee [--only slug]
"""
import argparse
import bisect
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SOURCES = ROOT / 'experimental/sources'
EXPECTED_SHA = '9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0'


class Original:
    def __init__(self, path, toolchain):
        import hashlib
        self.data = Path(path).read_bytes()
        assert hashlib.sha256(self.data).hexdigest() == EXPECTED_SHA, 'wrong executable'
        self.path = str(path)
        self.objdump = str(Path(toolchain) / 'bin/mips64r5900el-ps2-elf-objdump')
        nm = subprocess.run([str(Path(toolchain) / 'bin/mips64r5900el-ps2-elf-nm'), '-n', self.path],
                            capture_output=True, text=True, check=True).stdout
        self.by_name, self.by_addr = {}, {}
        for line in nm.splitlines():
            parts = line.split()
            if len(parts) == 3:
                address = int(parts[0], 16)
                self.by_name[parts[2]] = address
                self.by_addr.setdefault(address, (parts[1], parts[2]))
        self.order = sorted(self.by_addr)
        self.gp = self.by_name['_gp']
        phoff = struct.unpack_from('<I', self.data, 28)[0]
        entsize, count = struct.unpack_from('<HH', self.data, 42)
        self.segments = []
        for i in range(count):
            kind, off, va, _, filesz, memsz, _, _ = struct.unpack_from('<8I', self.data, phoff + i * entsize)
            if kind == 1:
                self.segments.append((va, off, filesz))

    def word(self, va):
        for base, off, size in self.segments:
            if base <= va < base + size:
                return struct.unpack_from('<I', self.data, off + va - base)[0]
        return 0

    def disassemble(self, start, stop):
        return subprocess.run([self.objdump, '-d', '--start-address=%#x' % start, '--stop-address=%#x' % stop,
                               self.path], capture_output=True, text=True, check=True).stdout

    def replay(self, wrapper_name):
        wrapper = self.by_name[wrapper_name]
        text = self.disassemble(wrapper, wrapper + 32)
        ops = [re.sub(r'^\s*[0-9a-f]+:\s+[0-9a-f]+\s+', '', l) for l in text.splitlines()
               if re.match(r'\s+[0-9a-f]+:', l)]
        assert len(ops) == 8 and ops[1] == 'li\ta0,1' and ops[3].startswith('jal') and ops[4] == 'li\ta1,0xffff', ops
        helper = int(re.search(r'jal\s+([0-9a-f]+)', ops[3]).group(1), 16)
        end = self.order[bisect.bisect_right(self.order, helper)]
        reg = {'zero': 0, 'sp': 0, 'a0': 1, 'a1': 0xffff}
        fp, stack, rows, pending = {}, {}, [], None
        for line in self.disassemble(helper, end).splitlines():
            m = re.match(r'\s+[0-9a-f]+:\s+[0-9a-f]{8}\s+(\S+)\s*(.*)', line)
            if not m:
                continue
            op, rest = m.group(1), m.group(2)
            args = [a.strip() for a in rest.split('<')[0].split(',')] if rest.strip() else []
            flush, pending = pending, None
            if op in ('addiu', 'daddiu'):
                reg[args[0]] = (reg[args[1]] + int(args[2], 0)) & 0xffffffff
            elif op == 'li':
                reg[args[0]] = int(args[1], 0) & 0xffffffff
            elif op == 'lui':
                reg[args[0]] = (int(args[1], 0) << 16) & 0xffffffff
            elif op == 'move':
                reg[args[0]] = reg[args[1]]
            elif op == 'mtc1':
                fp[args[1]] = reg[args[0]]
            elif op == 'mov.s':
                fp[args[0]] = fp[args[1]]
            elif op == 'lwc1':
                off, base = re.match(r'(-?\d+)\((\w+)\)', args[1]).groups()
                assert base == 'gp'
                fp[args[0]] = self.word(self.gp + int(off))
            elif op == 'sw':
                off, base = re.match(r'(-?\d+)\((\w+)\)', args[1]).groups()
                if base == 'sp':
                    stack[int(off)] = reg[args[0]]
            elif op == 'jal':
                target = re.search(r'<([^>]+)>', rest).group(1)
                assert target.startswith('__11ParticleDef'), target
                pending = 'call'
            if flush:
                signed = lambda x: x - (1 << 32) if x & 0x80000000 else x
                ints = [signed(reg[r]) for r in ('a1', 'a2', 'a3', 't0', 't1', 't2', 't3')] + [signed(stack[0])]
                rows.append((reg['a0'], ints, [fp['$f12'], fp['$f13'], fp['$f14']],
                             [signed(stack[8]), signed(stack[16]), signed(stack[24])], stack[32]))
        return helper, rows


def unit_symbol(directory):
    text = (directory / 'host_check.cpp').read_text()
    return re.search(r'Host contract for (\S+)\.', text).group(1)


def check_unit(original, directory, scratch):
    slug = directory.name[len('w8-spell-static-init-a-'):]
    symbol = unit_symbol(directory)
    binary = scratch / slug
    subprocess.run(['clang++', '-std=c++17', '-O2', '-o', str(binary), str(directory / 'host_check.cpp')], check=True)
    run = subprocess.run([str(binary)], capture_output=True, text=True)
    assert run.returncode == 0, 'host contract failed: ' + run.stdout
    host_rows = []
    for line in run.stdout.splitlines():
        fields = line.split()
        assert fields[0] == 'ROW'
        host_rows.append((list(map(int, fields[2:10])), [int(x, 16) for x in fields[10:13]],
                          list(map(int, fields[13:16]))))
    helper, rows = original.replay(symbol)
    assert len(rows) == len(host_rows), (len(rows), len(host_rows))
    stem = slug.replace('-', '_') + '_static_init.cpp'
    source = (directory / stem).read_text()
    head, _, tail_text = source.partition('namespace {')
    globals_ = set(re.findall(r'^ParticleDefStorage (\w+);', head, re.M))
    declared = globals_ | set(re.findall(r'^ParticleDefStorage (\w+);', tail_text, re.M))
    for (obj, ints, floats, tail, callback), (h_ints, h_floats, h_tail) in zip(rows, host_rows):
        assert (ints, floats, tail) == (h_ints, h_floats, h_tail), (slug, hex(obj))
        assert callback == 0
        kind, name = original.by_addr[obj]
        assert name in declared, (slug, name)
        assert (kind.isupper()) == (name in globals_), (slug, name, kind)
    print('ok %-28s %2d constructions, helper 0x%06x' % (slug, len(rows), helper))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--original', required=True)
    parser.add_argument('--toolchain', required=True)
    parser.add_argument('--only')
    args = parser.parse_args()
    original = Original(args.original, args.toolchain)
    with tempfile.TemporaryDirectory() as scratch:
        for directory in sorted(SOURCES.glob('w8-spell-static-init-a-*')):
            if not (directory / 'host_check.cpp').exists():
                continue
            if args.only and not directory.name.endswith(args.only):
                continue
            check_unit(original, directory, Path(scratch))


if __name__ == '__main__':
    sys.exit(main())
