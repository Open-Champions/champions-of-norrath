#!/usr/bin/env python3
"""Differential check of the hand-written constructors against the private executable.

Each function is run from the private SLUS_205.65 image and from the freshly built
authored object (compiled and linked with the reviewed EE toolchain at a scratch
address) in the small R5900 interpreter from combat-reactions-compare.py. Both
runs get the same randomized object memory, the same seeded globals and the same
stubbed callees. The stub call logs (argument registers, pointed-to bytes where
the callee takes a pointer to a record) and every byte of the object and of the
globals afterwards must be equal.

  NORRATH_EXE=/path/SLUS_205.65 NORRATH_EE=/path/to/ee \\
      python3 docs/agents/w8-spell-static-init-b-ctors-compare.py all 800

Scenario floats are small values so the EE's truncating rounding and this
interpreter's rounding agree. The executable and toolchain stay outside the
repository; nothing from the executable is stored.
"""
import importlib.util
import os
import random
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
spec = importlib.util.spec_from_file_location('cr_compare', os.path.join(HERE, 'combat-reactions-compare.py'))
cr = importlib.util.module_from_spec(spec)
spec.loader.exec_module(cr)

P = cr.P

# The EE's floating-point unit has no infinities: an overflow or a division by zero gives the
# largest magnitude, bits 0x7fffffff (0xffffffff when negative). The shared interpreter
# models an infinity there, which would make a correct comparison fail.
_setf = cr.Machine.setf


def _setf_ee(self, i, x):
    if x in (float('inf'), float('-inf')):
        self.f[i] = 0x7FFFFFFF if x > 0 else 0xFFFFFFFF
    else:
        _setf(self, i, x)


cr.Machine.setf = _setf_ee
SRC = cr.SRC
M32 = cr.M32
OBJ = 0x30000000
OTHER = 0x30100000
STUB = 0x00500000
STACK = cr.STACK


def image_symbols():
    syms = {}
    for l in subprocess.check_output([P + 'nm', cr.EXE], text=True).splitlines():
        p = l.split()
        if len(p) >= 3:
            syms.setdefault(p[-1], []).append(int(p[0], 16))
    return syms


def put32(mem, a, v):
    for i in range(4):
        mem[a + i] = (v >> (8 * i)) & 0xFF


def fill(rng, mem, base, size):
    for i in range(size):
        mem[base + i] = rng.randrange(256)


# callee -> logged registers: ('r', n) integer register, ('f', n) float register bits,
# ('p', n, size) bytes at the pointer in register n.
CALLS = {
    'P_CreateParticleSystem__Fv': [],
    'lightAdd__FP5Light': [('r', 4), ('p', 4, 48)],
    'lumpFindResource__FPcT0': [('s', 4), ('s', 5)],
    'getProxyMeshId__4itemPQ24item11description': [('r', 4), ('p', 4, 72)],
    '__4PropG6Point3PcN22li': [('r', 4), ('p', 5, 12), ('s', 6), ('r', 7), ('r', 8), ('r', 9), ('r', 10)],
    'objectUpdateInGrid__FP10GameObject': [('r', 4)],
    'matMakeTransYaw__FP8Matrix34G6Point3s': [('r', 4), ('p', 5, 12), ('r', 6)],
    'soundPlaySequence__FP15AnimationHeaderG6Point3f': [('r', 4), ('p', 5, 12), ('f', 12)],
}
FUNCTIONS = {}


def reg(sym, src, scenario, data=(), **kw):
    FUNCTIONS[sym] = dict(sym=sym, src=src, scenario=scenario, data=data, **kw)


def build(spec_, work, syms):
    base = os.path.basename(spec_['src']).replace('.cpp', '')
    obj = os.path.join(work, base + '.o')
    subprocess.check_call([P + 'g++', *cr.FLAGS, '-c', os.path.join(SRC, spec_['src']), '-o', obj])
    und = [l.split()[-1] for l in subprocess.check_output([P + 'nm', '-u', obj], text=True).splitlines() if l.strip()]
    defs, callee_addrs = {}, {}
    for n, name in enumerate(und):
        if len(syms.get(name, [])) != 1:
            raise SystemExit('import %s is not exactly one image symbol' % name)
        if name in CALLS:
            defs[name] = STUB + 0x10 * n
            callee_addrs[name] = (syms[name][0], defs[name])
        else:
            defs[name] = syms[name][0]      # data symbol: same address in both runs
    elf = os.path.join(work, base + '.elf')
    cmd = [P + 'ld', '-m', 'elf32lr5900', '-Ttext=0x00400000', '--no-warn-mismatch']
    cmd += ['--defsym=%s=%#x' % kv for kv in defs.items()] + ['-o', elf, obj]
    subprocess.check_call(cmd)
    return elf, callee_addrs


def entry_of(elf, name):
    for l in subprocess.check_output([P + 'nm', elf], text=True).splitlines():
        p = l.split()
        if len(p) == 3 and p[2] == name:
            return int(p[0], 16)
    raise SystemExit('missing ' + name)


def run(image, entry, stub_addrs, sc, seed):
    mem = dict(sc['mem'])
    stubs = {}
    m = cr.Machine(image, mem, stubs, cr.GP)
    rng = random.Random(seed ^ 0xC0FFEE)
    for name, addr in stub_addrs.items():
        def stub(mm, name=name):
            row = [name]
            for item in CALLS[name]:
                if item[0] == 'r':
                    row.append(mm.g32(item[1]))
                elif item[0] == 'f':
                    row.append(mm.f[item[1]])
                elif item[0] == 's':
                    out = bytearray()
                    while len(out) < 64 and mm.u8(mm.g32(item[1]) + len(out)):
                        out.append(mm.u8(mm.g32(item[1]) + len(out)))
                    row.append(bytes(out))
                else:
                    row.append(mm.read(mm.g32(item[1]), item[2]))
            m.log.append(tuple(row))
            mm.setr32(2, rng.randrange(8) if name in sc.get('small', ()) else sc.get('returns', {}).get(name, rng.randrange(1 << 32)))
            if name in sc.get('writes', {}):
                sc['writes'][name](mm, rng)
        stubs[addr] = stub
    m.r[29] = STACK
    for i, v in enumerate(sc['args']):
        m.r[4 + i] = v & M32
    m.log = []
    m.run(entry)
    return m


def compare(sym, runs, syms, exe):
    spec_ = FUNCTIONS[sym]
    work = os.path.join(cr.SCRATCH, sym)
    os.makedirs(work, exist_ok=True)
    elf, callee_addrs = build(spec_, work, syms)
    auth = cr.Image(elf)
    a_entry = entry_of(elf, sym)
    o_entry = syms[sym][0]
    o_stubs = {n: a[0] for n, a in callee_addrs.items()}
    a_stubs = {n: a[1] for n, a in callee_addrs.items()}
    bad = 0
    for seed in range(runs):
        sc = spec_['scenario'](random.Random(seed))
        res = []
        for image, entry, stubs in ((exe, o_entry, o_stubs), (auth, a_entry, a_stubs)):
            try:
                m = run(image, entry, stubs, sc, seed)
            except cr.Stop as e:
                print('seed %d: stopped: %s' % (seed, e))
                return 2
            res.append((m.log, {a: m.u8(a) for a in sc['compare']}))
        if res[0] != res[1]:
            bad += 1
            if bad <= 3:
                print('MISMATCH seed %d' % seed)
                if res[0][0] != res[1][0]:
                    a, b = res[0][0], res[1][0]
                    k = next((i for i in range(min(len(a), len(b))) if a[i] != b[i]), min(len(a), len(b)))
                    print('  log differs at call %d (of %d and %d):\n   image %s\n   built %s' % (k, len(a), len(b), a[k:k + 1], b[k:k + 1]))
                diff = [hex(x) for x in res[0][1] if res[0][1][x] != res[1][1][x]][:10]
                if diff:
                    print('  memory differs at', diff)
    print('%s: %d runs, %d mismatches' % (sym, runs, bad))
    return 1 if bad else 0


# ---- scenarios ----

def putf(mem, a, x):
    put32(mem, a, cr.f2b(x))


def scenario_planar(rng):
    mem = {}
    fill(rng, mem, OBJ, 128)
    fill(rng, mem, OTHER, 64)
    for k in range(3):
        putf(mem, OTHER + 16 + 4 * k, rng.choice([0.0, -1.5, 2.25, 100.0, rng.uniform(-50, 50)]))
    putf(mem, OTHER + 32, rng.choice([0.0, 1.0, -3.7, 5.5, 10000.0, rng.uniform(-100, 100)]))
    put32(mem, 0x483E5C, rng.choice([0, 1, 0xFFFFFFFF, rng.randrange(1 << 32)]))
    put32(mem, 0x4844A4, rng.choice([0, 1000, 0xFFFFFF00, rng.randrange(1 << 32)]))
    # data the callee names are read from
    args = [OBJ, rng.choice([0, 1, 255, 256, -1, rng.randrange(1 << 32)]), OTHER, rng.choice([0, OTHER + 0x100, rng.randrange(1 << 32)])]
    return {'mem': mem, 'args': args, 'compare': list(range(OBJ, OBJ + 128)) + list(range(0x483E5C, 0x483E60))
            + list(range(0x4844A4, 0x4844A8))}



def scenario_horn(rng):
    mem = {}
    fill(rng, mem, OBJ, 64)
    put32(mem, 0x483E5C, rng.choice([0, 1, 0xFFFFFFFF, rng.randrange(1 << 32)]))        # syncRandState
    put32(mem, 0x4844A4, rng.choice([0, 1000, 0xFFFFFF00, rng.randrange(1 << 32)]))     # frameNm
    args = [OBJ, rng.choice([0, 1, 255, 256, 0x1FF, -1, rng.randrange(1 << 32)]),
            rng.choice([0, OTHER, rng.randrange(1 << 32)]), rng.choice([0, OTHER + 0x100, rng.randrange(1 << 32)])]
    return {'mem': mem, 'args': args, 'compare': list(range(OBJ, OBJ + 64)) + list(range(0x483E5C, 0x483E60))
            + list(range(0x4844A4, 0x4844A8))}


reg('__19SpellHornOmenEffectiP8CreatureP5Skill', 'w8-spell-static-init-b-hornomen-ctor/ctor.cpp', scenario_horn)

def scenario_item_prop(rng):
    mem = {}
    fill(rng, mem, OBJ, 512)
    fill(rng, mem, OTHER, 128)
    fill(rng, mem, OTHER + 0x400, 12)
    for i in range(8):                       # first records of _4item$globalItem
        for j in range(4):
            put32(mem, 0x31F540 + 16 * i + 4 * j, rng.randrange(1 << 32))
    # make the tested bit and flag word vary
    mem[OTHER + 12] = rng.choice([0, 0x80, 0x7F, 0xFF, rng.randrange(256)])
    args = [OBJ, OTHER + 0x400, OTHER, rng.choice([0, 1, -1, 77, rng.randrange(1 << 32)])]
    return {'mem': mem, 'args': args,
            'compare': list(range(OBJ, OBJ + 512)),
            'small': ['getProxyMeshId__4itemPQ24item11description']}


reg('__22SpellPlanarSteelEffectiP8CreatureP5Skill', 'w8-spell-static-init-b-planarsteel-ctor/ctor.cpp', scenario_planar)
reg('__8ItemPropG6Point3PQ24item11descriptioni', 'w8-spell-static-init-b-itemprop-ctor/ctor.cpp', scenario_item_prop)

if __name__ == '__main__':
    runs = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    exe = cr.Image(cr.EXE)
    syms = image_symbols()
    names = list(FUNCTIONS) if sys.argv[1] == 'all' else [n for n in FUNCTIONS if sys.argv[1] in n]
    worst = 0
    for n in names:
        worst = max(worst, compare(n, runs, syms, exe))
    sys.exit(worst)
