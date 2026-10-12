#!/usr/bin/env python3
"""Differential check of the w8-core-loop-d bodies against the private executable.

Runs one function from the private SLUS_205.65 image and the same function built
from the authored source (compiled with the reviewed EE g++ and linked at a scratch
address) in the small R5900 interpreter of combat-reactions-compare.py. Both runs
get the same randomized scenario memory and the same stubbed callees. Calls into
other functions are intercepted at their image addresses in both runs (the authored
object is linked against the image's own symbol addresses, so data lives at the
same addresses too). The stub call logs (argument registers and single-float
arguments), the return value, and every byte written outside the stack must be equal.
The executable and toolchain stay outside the repository; nothing from the
executable is stored.

  NORRATH_EXE=/path/SLUS_205.65 NORRATH_EE=/path/to/ee \\
      python3 docs/agents/w8-core-loop-d-compare.py all 500

Scenario floats are small integers so the EE's truncating rounding and the
interpreter's rounding agree. Only the opcodes the interpreter implements exist;
any other stops the run with its address.
"""
import importlib.util
import os
import random
import struct
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
_spec = importlib.util.spec_from_file_location('crc', os.path.join(HERE, 'combat-reactions-compare.py'))
crc = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(crc)
Image, Stop, M32, GP, FLAGS, P, SRC, SCRATCH, EXE = (
    crc.Image, crc.Stop, crc.M32, crc.GP, crc.FLAGS, crc.P, crc.SRC, crc.SCRATCH, crc.EXE)

TEXT = 0x02800000
STACK = 0x3F000000
SCN = 0x30000000          # scenario scratch objects live here
STACK_LO = STACK - 0x4000
STACK_HI = STACK + 0x1000


def f2b(x):
    return struct.unpack('<I', struct.pack('<f', x))[0]


def put32(mem, a, v):
    v &= M32
    for i in range(4):
        mem[a + i] = (v >> (8 * i)) & 0xFF


def putf(mem, a, x):
    put32(mem, a, f2b(x))


def fill(rng, mem, base, size):
    for i in range(size):
        mem[base + i] = rng.randrange(256)


class Machine(crc.Machine):
    """The shared interpreter plus sqrt.s (the EE form takes its operand from ft)."""

    def cop1(self, ins, pc, rs, rt, rd, sa, fn, tgt):
        if rs == 0x10 and fn == 4:
            self.setf(sa, self.fv(rt) ** 0.5)
            return None
        return super().cop1(ins, pc, rs, rt, rd, sa, fn, tgt)


class Combined:
    """Authored segments first, then the executable, for the authored run."""

    def __init__(self, authored, exe):
        self.a, self.e = authored, exe

    def byte(self, addr):
        for vaddr, blob in self.a.segs:
            if vaddr <= addr < vaddr + len(blob):
                return blob[addr - vaddr]
        return self.e.byte(addr)


def exe_symbols():
    out = {}
    for line in subprocess.check_output([P + 'nm', EXE], text=True).splitlines():
        parts = line.split()
        if len(parts) == 3:
            out.setdefault(parts[2], (int(parts[0], 16), parts[1]))
    return out


def build(rels, work, syms):
    objs = []
    for rel in rels:
        obj = os.path.join(work, os.path.basename(rel).replace('.cpp', '.o'))
        subprocess.check_call([P + 'g++', *FLAGS, '-c', os.path.join(SRC, rel), '-o', obj])
        objs.append(obj)
    defined = set()
    undefined = set()
    for obj in objs:
        for line in subprocess.check_output([P + 'nm', obj], text=True).splitlines():
            parts = line.split()
            if len(parts) == 2 and parts[0] == 'U':
                undefined.add(parts[1])
            elif len(parts) == 3:
                defined.add(parts[2])
    defsyms = {}
    stub_names = {}
    for name in sorted(undefined - defined):
        if name not in syms:
            raise SystemExit('authored import with no image symbol: ' + name)
        addr, kind = syms[name]
        defsyms[name] = addr
        if kind in 'Tt':
            stub_names[name] = addr
    out = os.path.join(work, 'linked.elf')
    cmd = [P + 'ld', '-m', 'elf32lr5900', '-Ttext=%#x' % TEXT, '--no-warn-mismatch']
    cmd += ['--defsym=%s=%#x' % kv for kv in defsyms.items()] + ['-o', out] + objs
    subprocess.check_call(cmd)
    return out, stub_names


def symbol_address(elf, name):
    for line in subprocess.check_output([P + 'nm', elf], text=True).splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[2] == name:
            return int(parts[0], 16)
    raise SystemExit('missing symbol ' + name)


# Argument registers that carry a value; the others hold leftovers and are not logged.
ARGC = {'memCheckHeapReal__FPCci': 2, 'strncmp': 3, 'strcmp': 2, 'strlen': 1, 'strrchr': 2, 'memcpy': 3,
        'memmove': 3, 'scePrintf': 2, 'initFile__FPcPvi': 3, 'animMapAnimationSounds__FP15AnimationHeader': 1}
STRING_ARGS = {'memCheckHeapReal__FPCci': (0,), 'scePrintf': (0,), 'strncmp': (0, 1), 'strcmp': (0, 1), 'strlen': (0,), 'strrchr': (0,)}


def _memcpy(mm, a, cstr):
    data = mm.read(a[1], a[2])
    mm.write(a[0], data)
    return a[0]


def _strlen(mm, a, cstr):
    return len(cstr(mm, a[0], 1 << 16))


def _strncmp(mm, a, cstr):
    for i in range(a[2]):
        x, y = mm.u8(a[0] + i), mm.u8(a[1] + i)
        if x != y:
            return x - y
        if x == 0:
            return 0
    return 0


def _strcmp(mm, a, cstr):
    return _strncmp(mm, [a[0], a[1], 1 << 16], cstr)


def _strrchr(mm, a, cstr):
    text = cstr(mm, a[0], 1 << 16)
    c = a[1] & 0xFF
    if c == 0:
        return a[0] + len(text)
    k = text.rfind(bytes([c]))
    return 0 if k < 0 else a[0] + k


LIBC = {'memcpy': _memcpy, 'memmove': _memcpy, 'strlen': _strlen, 'strncmp': _strncmp, 'strcmp': _strcmp,
        'strrchr': _strrchr}


def run_once(image, entry, stub_addrs, sc):
    mem = dict(sc['mem'])
    stubs = {}
    m = Machine(image, mem, stubs, GP)
    m.log = []
    counts = {}

    def cstr(mm, a, limit=96):
        out = b''
        while len(out) < limit:
            c = mm.u8(a + len(out))
            if c == 0:
                break
            out += bytes([c])
        return out

    def make(name):
        def handler(mm):
            n = counts.get(name, 0)
            counts[name] = n + 1
            args = [mm.g32(4 + i) for i in range(4)]
            shown = list(args)[:ARGC.get(name, 4)]
            for idx in STRING_ARGS.get(name, ()):
                shown[idx] = cstr(mm, args[idx])
            mm.log.append((name, tuple(shown), mm.f[12], mm.f[13]))
            if name in LIBC:
                mm.setr32(2, LIBC[name](mm, args, cstr))
                return
            rets = sc.get('stub_ret', {}).get(name)
            mm.setr32(2, rets[n % len(rets)] if rets else 0)
        return handler
    for name, addr in stub_addrs.items():
        stubs[addr] = make(name)
    m.r[29] = STACK
    for i, v in enumerate(sc.get('args', [])):
        m.r[4 + i] = v & M32
    for i, v in sc.get('fargs', {}).items():
        m.f[i] = v
    m.run(entry)
    return m


def compare(name, runs):
    spec = SPECS[name]
    work = os.path.join(SCRATCH, name)
    os.makedirs(work, exist_ok=True)
    syms = exe_symbols()
    elf, stub_names = build(spec['src'], work, syms)
    exe = Image(EXE)
    auth = Combined(Image(elf), exe)
    entry_auth = symbol_address(elf, spec['symbol'])
    entry_orig = syms[spec['symbol']][0]
    bad = 0
    total_calls = 0
    seen = {}
    for seed in range(runs):
        sc = spec['scenario'](random.Random(seed), syms)
        res = []
        for image, entry in ((exe, entry_orig), (auth, entry_auth)):
            try:
                m = run_once(image, entry, stub_names, sc)
            except Stop as e:
                print('seed %d: stopped: %s' % (seed, e))
                return 2
            res.append(m)
        a, b = res
        written = sorted(x for x in set(a.mem) | set(b.mem) if not STACK_LO <= x < STACK_HI)
        ra = [(x, a.u8(x)) for x in written]
        rb = [(x, b.u8(x)) for x in written]
        ret = (a.g32(2), b.g32(2)) if spec.get('returns') else (0, 0)
        fret = (a.f[0], b.f[0]) if spec.get('freturns') else (0, 0)
        total_calls += len(a.log)
        for entry in a.log:
            seen[entry[0]] = seen.get(entry[0], 0) + 1
        if a.log != b.log or ra != rb or ret[0] != ret[1] or fret[0] != fret[1]:
            bad += 1
            if bad <= 3:
                print('MISMATCH seed %d' % seed)
                if a.log != b.log:
                    k = next((i for i in range(min(len(a.log), len(b.log))) if a.log[i] != b.log[i]),
                             min(len(a.log), len(b.log)))
                    print('  log differs at %d:\n   orig %s\n   auth %s' % (k, a.log[k:k + 2], b.log[k:k + 2]))
                diff = [hex(x) for (x, v), (_, w) in zip(ra, rb) if v != w][:10]
                if diff:
                    print('  memory differs at', diff)
                if ret[0] != ret[1] or fret[0] != fret[1]:
                    print('  return', ret, fret)
    print('%s: %d runs, %d stub calls seen, %d mismatches' % (name, runs, total_calls, bad))
    if os.environ.get('STATS'):
        print('  calls seen in the image run:', seen)
    return 1 if bad else 0


# ---- scenarios ----

def sym(syms, name):
    return syms[name][0]


def scenario_unscramble(rng, syms):
    mem = {}
    fill(rng, mem, sym(syms, 'unscrambleTable'), 256)
    return {'mem': mem}


def scenario_pan(rng, syms):
    mem = {}
    pos = sym(syms, 'worldCameraPos')
    mat = sym(syms, 'worldCameraMatrix')
    for i in range(3):
        putf(mem, pos + 4 * i, float(rng.randrange(-200, 200)))
        putf(mem, SCN + 4 * i, float(rng.choice([rng.randrange(-700, 700), rng.randrange(-300, 300)])))
    for i in range(9):
        putf(mem, mat + 4 * i, float(rng.choice([-1, 0, 0, 1, 2, -2, 0.5, 4])))
    fill(rng, mem, SCN + 0x100, 8)
    a = float(rng.choice([0, 100, 360, 400, 500, -50]))
    b = float(rng.choice([360, 840, 500, 1000, 100, 1200]))
    if a == b:
        b += 1.0
    return {'mem': mem, 'args': [SCN, SCN + 0x100, SCN + 0x104], 'fargs': {12: f2b(a), 13: f2b(b)}}


def scenario_init_lump(rng, syms):
    mem = {}
    fill(rng, mem, SCN, 0x3000)
    count = rng.choice([0, 1, 2, 3, 4, 5, 6, 7, 8])
    mem[SCN] = count & 0xFF
    mem[SCN + 1] = count >> 8
    names = [b'ntsc_a.anm', b'pal_a.anm', b'pal_ntsc_b.anm', b'c.vif', b'd', b'ntsc_pal_x.anm', b'e.anm', b'pal_f.vif',
             b'anm', b'g.h.anm', b'ntsc_']
    data = 0x1000
    last = 0
    for k in range(count):
        row = SCN + 4 + 12 * k
        name = rng.choice(names) + b'\0'
        for j, c in enumerate(name):
            mem[SCN + 0x200 + 0x20 * k + j] = c
        put32(mem, row, 0x200 + 0x20 * k)
        r = rng.random()
        if r < 0.2 and k:
            off = last                      # alias of the previous data
        elif r < 0.3 and k:
            off = last - 0x40               # not above the previous one: skipped
        else:
            data += rng.choice([0, 0x20, 0x80, 0x100])
            off = data
        size = rng.choice([0, 0x40, 0x80, 0x90, 0x100, 0x7f, 0x81])
        data += size if off == data else 0
        last = off
        put32(mem, row + 4, off)
        put32(mem, row + 8, size)
    if rng.random() < 0.3:
        put32(mem, SCN + 4 + 12 * count, rng.randrange(1 << 32))
    return {'mem': mem, 'args': [SCN],
            'stub_ret': {'initFile__FPcPvi': [rng.choice([0, 0x20, 0x40, 0x80, 0xc0, 0x100, 0x180, 0x1f, 0x7f]) for _ in range(8)]}}


SPECS = {
    'makeUnscrambleTable__Fv': dict(src=['w8-core-loop-d-unscramble-table/unscramble_table.cpp'],
                                    symbol='makeUnscrambleTable__Fv', scenario=scenario_unscramble),
    'pan__FG6Point3ffPfT3': dict(src=['w8-core-loop-d-pan/pan.cpp'], symbol='pan__FG6Point3ffPfT3',
                                 scenario=scenario_pan),
    'initLump__FPv': dict(src=['w8-core-loop-d-init-lump/init_lump.cpp'], symbol='initLump__FPv',
                          scenario=scenario_init_lump, returns=True),
}


def main():
    which = sys.argv[1] if len(sys.argv) > 1 else 'all'
    runs = int(sys.argv[2]) if len(sys.argv) > 2 else 200
    names = list(SPECS) if which == 'all' else [which]
    rc = 0
    for n in names:
        rc |= compare(n, runs)
    return rc


if __name__ == '__main__':
    sys.exit(main())
