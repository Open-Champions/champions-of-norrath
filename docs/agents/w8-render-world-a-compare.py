#!/usr/bin/env python3
"""Differential check of w8-render-world-a bodies against the private executable.

Runs one function from the private SLUS_205.65 image and the same function built
from the authored source (compiled and linked with the reviewed EE toolchain at a
scratch address) in the small R5900 interpreter of combat-reactions-compare.py.
Both runs get the same randomized scenario memory and the same stubbed callees.
The stub call logs (argument registers, selected pointed-to bytes), the return
value and every compared byte must be equal. Data the function reaches by name
is placed at the image's address of the same name for both runs. The executable
and toolchain stay outside the repository and nothing from the executable is
stored.

  NORRATH_EXE=/path/SLUS_205.65 NORRATH_EE=/path/to/ee \\
      python3 docs/agents/w8-render-world-a-compare.py <function|all> [runs]

Scenario floats are small values, so the EE's truncating rounding and this
interpreter's rounding agree. Only the instructions the bodies use exist in the
interpreter; any other opcode stops the run with its address.
"""
import importlib.util
import os
import random
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
_spec = importlib.util.spec_from_file_location('crc', os.path.join(HERE, 'combat-reactions-compare.py'))
crc = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(crc)
Machine, Image, Stop = crc.Machine, crc.Image, crc.Stop
M32 = crc.M32
f2b, b2f = crc.f2b, crc.b2f

EXE, P, FLAGS, GP = crc.EXE, crc.P, crc.FLAGS, crc.GP
SRC = crc.SRC
SCRATCH = crc.SCRATCH
AUTH_BASE = 0x08000000
STUB = 0x00500000
STACK = 0x3F000000
ARENA = 0x30000000


class Combined:
    """Authored segments first (scratch address), then the executable."""

    def __init__(self, exe, auth):
        self.exe, self.auth = exe, auth

    def byte(self, addr):
        if AUTH_BASE <= addr < AUTH_BASE + 0x1000000:
            return self.auth.byte(addr)
        return self.exe.byte(addr)


def image_symbols():
    out = {}
    for l in subprocess.check_output([P + 'nm', EXE], text=True).splitlines():
        parts = l.split()
        if len(parts) == 3:
            out.setdefault(parts[2], int(parts[0], 16))
    return out


SYMS = None


def build_authored(rels, work, stub_names, alias):
    global SYMS
    if SYMS is None:
        SYMS = image_symbols()
    objs = []
    for n, rel in enumerate(rels):
        obj = os.path.join(work, '%d-%s' % (n, os.path.basename(rel).replace('.cpp', '.o')))
        subprocess.check_call([P + 'g++', *FLAGS, '-c', os.path.join(SRC, rel), '-o', obj])
        objs.append(obj)
    und = set()
    defd = set()
    for obj in objs:
        for l in subprocess.check_output([P + 'nm', obj], text=True).splitlines():
            p = l.split()
            if len(p) == 2 and p[0] == 'U':
                und.add(p[1])
            elif len(p) == 3 and p[1] != 'U':
                defd.add(p[2])
    defsyms = {}
    for name in sorted(und - defd):
        if name in stub_names:
            defsyms[name] = STUB + 0x10 * stub_names.index(name)
        elif name in alias:
            defsyms[name] = SYMS[alias[name]]
        elif name in SYMS:
            defsyms[name] = SYMS[name]
        else:
            raise SystemExit('unmapped authored import: ' + name)
    out = os.path.join(work, 'auth.elf')
    cmd = [P + 'ld', '-m', 'elf32lr5900', '-Ttext=0x%x' % AUTH_BASE, '--no-warn-mismatch']
    cmd += ['--defsym=%s=%#x' % kv for kv in defsyms.items()]
    cmd += ['-o', out] + objs
    subprocess.check_call(cmd)
    return out


def symbol_address(elf, name):
    for l in subprocess.check_output([P + 'nm', elf], text=True).splitlines():
        parts = l.split()
        if len(parts) == 3 and parts[2] == name:
            return int(parts[0], 16)
    raise SystemExit('missing symbol ' + name)


def put32(mem, a, v):
    v &= M32
    for i in range(4):
        mem[a + i] = (v >> (8 * i)) & 0xFF


def putf(mem, a, x):
    put32(mem, a, f2b(x))


def fill(rng, mem, base, size):
    for i in range(size):
        mem[base + i] = rng.randrange(256)


def cstr(m, a, limit=64):
    out = bytearray()
    while len(out) < limit:
        c = m.u8(a + len(out))
        if c == 0:
            break
        out.append(c)
    return bytes(out)


class Stubs:
    """Logging stubs. spec: name -> dict(nargs=integer args logged, nf=float args logged, peek=[(argreg, nbytes)], str=[argreg], ret=fn(rng, m) -> int or None,
    fret=fn(rng, m) -> float or None, effect=fn(rng, m))."""

    def __init__(self, spec, rng):
        self.spec = spec
        self.rng = rng

    def handler(self, name):
        sp = self.spec[name]

        def h(m):
            entry = [name] + [m.g32(4 + i) for i in range(sp.get('nargs', 0))] + [m.f[12 + i] for i in range(sp.get('nf', 0))]
            for reg, n in sp.get('peek', []):
                entry.append(m.read(m.g32(reg), n))
            for reg in sp.get('str', []):
                entry.append(cstr(m, m.g32(reg)))
            m.log.append(tuple(entry))
            if 'effect' in sp:
                sp['effect'](self.rng, m)
            if 'ret' in sp:
                m.setr32(2, sp['ret'](self.rng, m))
            if 'fret' in sp:
                m.f[0] = f2b(sp['fret'](self.rng, m))
        return h


def run_one(image, entry, stub_addrs, sc, seed, stub_spec):
    mem = dict(sc['mem'])
    stubs = {}
    m = Machine(image, mem, stubs, GP)
    rng = random.Random(seed ^ 0xC0FFEE)
    st = Stubs(stub_spec, rng)
    for name in stub_spec:
        stubs[stub_addrs[name]] = st.handler(name)
    m.r[29] = STACK
    for i, v in enumerate(sc['args']):
        m.setr32(4 + i, v)
    for i, v in sc.get('fargs', {}).items():
        m.f[i] = v
    m.log = []
    m.run(entry, steps=sc.get('steps', 400000))
    return m


def compare(name, runs=200):
    spec = FUNCTIONS[name]
    work = os.path.join(SCRATCH, name.replace('/', '_'))
    os.makedirs(work, exist_ok=True)
    stub_names = list(spec['stubs'])
    elf = build_authored(spec['src'], work, stub_names, spec.get('alias', {}))
    img = Combined(Image(EXE), Image(elf))
    auth_entry = symbol_address(elf, spec.get('auth_symbol', spec['symbol']))
    global SYMS
    orig_stubs = {n: spec['stub_addr'][n] for n in stub_names}
    auth_stubs = {n: STUB + 0x10 * stub_names.index(n) for n in stub_names}
    bad = 0
    seen = {}
    for seed in range(runs):
        sc = spec['scenario'](random.Random(seed))
        res = []
        for entry, stubs in ((spec['orig'], orig_stubs), (auth_entry, auth_stubs)):
            try:
                m = run_one(img, entry, stubs, sc, seed, spec['stubs'])
            except Stop as e:
                print('seed %d: stopped: %s' % (seed, e))
                return 2
            region = {a: m.u8(a) for a in sc['compare']}
            ret = {None: None, True: m.g32(2), 'f': m.f[0]}[spec.get('returns')]
            res.append((m.log, region, ret))
        for e in res[0][0]:
            seen[e[0]] = seen.get(e[0], 0) + 1
        if res[0] != res[1]:
            bad += 1
            if bad <= 3:
                print('MISMATCH seed %d' % seed)
                a, b = res
                if a[0] != b[0]:
                    n = min(len(a[0]), len(b[0]))
                    k = next((i for i in range(n) if a[0][i] != b[0][i]), n)
                    print('  log differs at %d:\n   orig %s\n   auth %s' % (k, a[0][k:k + 2], b[0][k:k + 2]))
                diff = [hex(x) for x in a[1] if a[1][x] != b[1][x]][:10]
                if diff:
                    print('  memory differs at', diff)
                if a[2] != b[2]:
                    print('  return', a[2], b[2])
    print('%s: %d runs, %d mismatches' % (name, runs, bad))
    if os.environ.get('STATS'):
        print('  calls seen in the original:', seen)
    return 1 if bad else 0


FUNCTIONS = {}


def reg(symbol, orig, src, scenario, stubs=None, stub_addr=None, **kw):
    FUNCTIONS[symbol] = dict(symbol=symbol, orig=orig, src=[src] if isinstance(src, str) else src, scenario=scenario,
                             stubs=stubs or {}, stub_addr=stub_addr or {}, **kw)


# ---- scenarios ----

def sym(name):
    global SYMS
    if SYMS is None:
        SYMS = image_symbols()
    return SYMS[name]


def scenario_lump_check(rng):
    mem = {}
    put32(mem, sym('nmLumps'), rng.choice([0, 1, 5, 40, -1, -100, 300]))
    return {'mem': mem, 'args': [], 'compare': [sym('nmLumps') + i for i in range(4)]}


reg('lumpCheck__Fv', 0x1663B8, 'w8-render-world-a-lump-check/lump_check.cpp', scenario_lump_check)

# scenario and registrations for later functions are appended below

def memset_effect(rng, m):
    dst, val, n = m.g32(4), m.g32(5) & 0xFF, m.g32(6)
    if n <= 4096:
        for i in range(n):
            m.w8(dst + i, val)
    m.setr32(2, dst)


MEMSET = {'norrath_block_memory_set': dict(effect=memset_effect, nargs=3)}
MEMSET_ADDR = {'norrath_block_memory_set': 0x3021B0}
COPYOBJ = {'norrath_list_set_copy_object': dict(nargs=1)}
COPYOBJ_ADDR = {'norrath_list_set_copy_object': 0x194490}
BLOCK_SRC = ['w8-render-world-a-block-alloc/block_alloc.cpp', 'block-alloc-source-v2/block_alloc.cpp',
             'block-alloc-source-v2/memory_set.cpp', 'block-alloc-source-v2/list_copy.cpp']


def block_region(rng, mem, lens):
    """Four 16-byte rows at `region`: capacity, count, base, head. Free lists live in the arena."""
    region = sym('region')
    arena = ARENA
    for row in range(3):
        put32(mem, region + 16 * row, [16, 64, 256][row] * rng.choice([1, 1, 1, 2]))
        put32(mem, region + 16 * row + 4, rng.randrange(0, 40))
        put32(mem, region + 16 * row + 8, rng.randrange(1 << 32))
        n = lens[row]
        head = 0
        for k in range(n - 1, -1, -1):
            node = arena + 0x1000 * row + 0x100 * k
            put32(mem, node, head)
            for j in range(4, 16):
                mem[node + j] = rng.randrange(256)
            head = node
        put32(mem, region + 16 * row + 12, head)
    put32(mem, region + 48, rng.choice([512, 1024]))
    put32(mem, region + 52, rng.randrange(1 << 32))
    put32(mem, region + 56, rng.randrange(1 << 32))
    put32(mem, region + 60, rng.choice([0, 0x700000, 0x31000000]))
    return region


def scenario_block_free_count(rng):
    mem = {}
    region = block_region(rng, mem, [rng.choice([0, 0, 1, 2, 5, 9]) for _ in range(3)])
    return {'mem': mem, 'args': [], 'compare': list(range(region, region + 64))}


def scenario_block_alloc(rng):
    mem = {}
    fill(rng, mem, ARENA, 0x3000)
    lens = [rng.choice([0, 0, 1, 2, 5]) for _ in range(3)]
    region = block_region(rng, mem, lens)
    put32(mem, region + 48, 0x7FFFFFFF)
    put32(mem, region + 60, rng.choice([0x700000, 0x31000000]))
    size = rng.choice([0, 1, 15, 16, 17, 63, 64, 65, 255, 256, 257, 512, 1000, -1, -20])
    cmp_addrs = list(range(region, region + 64)) + list(range(ARENA, ARENA + 0x3000))
    return {'mem': mem, 'args': [size], 'compare': cmp_addrs}


def scenario_block_init(rng):
    mem = {}
    region = sym('region')
    for row in range(3):
        put32(mem, region + 16 * row, 4 * rng.choice([1, 2, 4, 16, 33]))
        put32(mem, region + 16 * row + 4, rng.choice([1, 2, 3, 10, 25]))
        for j in (8, 12):
            put32(mem, region + 16 * row + j, rng.randrange(1 << 32))
    for j in (48, 52, 56, 60):
        put32(mem, region + j, rng.randrange(1 << 32))
    pool = sym('dataBlock')
    fill(rng, mem, pool, 0x1000)
    return {'mem': mem, 'args': [], 'compare': list(range(region, region + 64)) + list(range(pool, pool + 0x1000))}


reg('blockGetNmFree__Fv', 0x144778, ['w8-render-world-a-block-get-nm-free/block_get_nm_free.cpp',
                                     'block-allocator-lifecycle-build/block_get_number_free.cpp'],
    scenario_block_free_count, returns=True, alias={'norrath_block_region_view': 'region'})


reg('blockAlloc__Fi', 0x1441B8, ['w8-render-world-a-block-alloc/block_alloc.cpp', 'block-alloc-source-v2/block_alloc.cpp'],
    scenario_block_alloc, stubs={**MEMSET, **COPYOBJ}, stub_addr={**MEMSET_ADDR, **COPYOBJ_ADDR}, returns=True,
    alias={'norrath_block_region_view': 'region'})
reg('blockAllocInit__Fv', 0x1440D8, ['w8-render-world-a-block-alloc-init/block_alloc_init.cpp',
                                     'block-alloc-source-v2/initialization.cpp'],
    scenario_block_init, stubs=MEMSET, stub_addr=MEMSET_ADDR,
    alias={'norrath_block_region_view': 'region', 'norrath_block_data_pool_view': 'dataBlock'})


def scenario_hermite(rng):
    vals = [rng.choice([0.0, 1.0, 0.5, -1.0, 2.5, rng.uniform(-100, 100), rng.uniform(-1, 1)]) for _ in range(5)]
    return {'mem': {}, 'args': [], 'fargs': {12 + i: f2b(v) for i, v in enumerate(vals)}, 'compare': []}


reg('evalHermite__Ffffff', 0x159330, 'w8-render-world-a-eval-hermite/eval_hermite.cpp', scenario_hermite, returns='f')


if __name__ == '__main__':
    runs = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    names = list(FUNCTIONS) if sys.argv[1] == 'all' else [sys.argv[1]]
    worst = 0
    for n in names:
        worst = max(worst, compare(n, runs))
    sys.exit(worst)
