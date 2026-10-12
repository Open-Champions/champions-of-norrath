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

_orig_cop1 = Machine.cop1


def _cop1(self, ins, pc, rs, rt, rd, sa, fn, tgt):
    # COP1.S sqrt.s (4), max.s (0x28) and min.s (0x29) are not in the shared interpreter.
    if rs == 0x10 and fn in (4, 0x28, 0x29):
        import math
        a, b = self.fv(rd), self.fv(rt)
        if fn == 4:
            b = self.fv(rt)   # the R5900 takes the source in the ft field
            self.setf(sa, math.sqrt(b) if b >= 0 else float('nan'))
        elif fn == 0x28:
            self.setf(sa, max(a, b))
        else:
            self.setf(sa, min(a, b))
        return None
    return _orig_cop1(self, ins, pc, rs, rt, rd, sa, fn, tgt)


Machine.cop1 = _cop1

_orig_exe = Machine.exe


def _exe(self, ins, pc):
    # SPECIAL teq (0x34): the authored build checks division by zero, the image does not.
    if (ins >> 26) == 0 and (ins & 63) == 0x34:
        if (self.r[(ins >> 21) & 31] & crc.M64) == (self.r[(ins >> 16) & 31] & crc.M64):
            raise Stop('division by zero trap at %#x' % pc)
        return None
    return _orig_exe(self, ins, pc)


Machine.exe = _exe
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
            entry = [name] + [m.g32(4 + i) for i in sp.get('regs', range(sp.get('nargs', 0)))] + [m.f[12 + i] for i in range(sp.get('nf', 0))]
            for reg in sp.get('regs64', []):
                entry.append(m.r[reg] & crc.M64)
            for off in sp.get('stack', []):
                entry.append(m.u32(m.g32(29) + off))
            for reg in sp.get('nonnull', []):
                entry.append(m.g32(reg) != 0)
            for item in sp.get('peek', []):
                reg, n = item[0], item[1]
                entry.append(m.read(m.g32(reg) + (item[2] if len(item) > 2 else 0), n))
            for reg in sp.get('str', []):
                entry.append(cstr(m, m.g32(reg)))
            if not sp.get('silent'):
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
    for n, a in spec.get('fixed', {}).items():
        orig_stubs[n] = auth_stubs[n] = a
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


def scenario_check_descriptor(rng):
    mem = {}
    base = sym('descListMark')
    free_at = rng.choice([None, 0, 1, 15, 30, 31, rng.randrange(32)])
    for i in range(32):
        put32(mem, base + 4 * i, rng.choice([0, 1, 0xFFFFFFFE, 0x31000000, 0xFFFF, 0x7FFFFFFF]))
    if free_at is not None:
        put32(mem, base + 4 * free_at, 0xFFFFFFFF)
        if rng.random() < 0.3:
            put32(mem, base + 4 * rng.randrange(32), 0xFFFFFFFF)
    return {'mem': mem, 'args': [], 'compare': list(range(base, base + 128))}


reg('checkCanAllocDescriptor__Fv', 0x187268, 'w8-render-world-a-check-can-alloc-descriptor/check_can_alloc_descriptor.cpp',
    scenario_check_descriptor, returns=True)


def strrchr_ret(rng, m):
    a, c = m.g32(4), m.g32(5) & 0xFF
    last = 0
    i = 0
    while True:
        ch = m.u8(a + i)
        if ch == c:
            last = a + i
        if ch == 0:
            break
        i += 1
    return last


def strcmp_ret(rng, m):
    a, b = m.g32(4), m.g32(5)
    i = 0
    while True:
        x, y = m.u8(a + i), m.u8(b + i)
        if x != y or x == 0:
            return x - y
        i += 1


STRSTUBS = {'strrchr': dict(nargs=2, str=[4], ret=strrchr_ret), 'strcmp': dict(nargs=0, str=[4, 5], ret=strcmp_ret)}
STRSTUB_ADDR = {'strrchr': 0x3052D8, 'strcmp': 0x3044FC}
STRARENA = ARENA + 0x8000
NAMES = [b'a.tex', b'b.etex', b'noext', b'x.y.tex', b'c.', b'd.tex.bak', b'.tex', b'tex', b'm.etex', b'n.tex']


def put_str(mem, a, text):
    for i, c in enumerate(text + b'\0'):
        mem[a + i] = c


def scenario_lump_find_by_type(rng):
    mem = {}
    n = rng.choice([0, 1, 2, 3, 5, 8])
    d = ARENA
    put32(mem, d, n | (rng.randrange(1 << 16) << 16))
    for i in range(n):
        e = d + 4 + 12 * i
        s = STRARENA + 0x20 * i
        put_str(mem, s, rng.choice(NAMES))
        put32(mem, e, s)
        put32(mem, e + 4, rng.randrange(1 << 32))
        put32(mem, e + 8, rng.randrange(1 << 32))
    ext = STRARENA + 0x400
    put_str(mem, ext, rng.choice([b'tex', b'etex', b'', b'bak', b'y', b'zzz']))
    skip = rng.choice([0, 0, 1, 2, 3, -1, -2])
    return {'mem': mem, 'args': [d, ext, skip], 'compare': list(range(d, d + 4 + 12 * 8))}


def scenario_lump_query(rng):
    mem = {}
    base = sym('lumps')
    n = rng.choice([0, 1, 2, 4, 6])
    pool = [b'font.lmp', b'env.lmp', b'a.lmp', b'']
    put32(mem, sym('nmLumps'), n)
    for i in range(7):
        r = base + 40 * i
        fill(rng, mem, r, 40)
        put_str(mem, r, rng.choice(pool))
        put32(mem, r + 16, rng.choice([0, 0x31000000 + 16 * i, rng.randrange(1 << 32)]))
        put32(mem, r + 20, rng.choice([0, 1, 2, 3, 0xFFFFFFFE, 0xFFFFFFFF]))
    q = STRARENA
    put_str(mem, q, rng.choice(pool))
    return {'mem': mem, 'args': [q], 'compare': list(range(base, base + 280))}


reg('lumpFindByType__FPvPci', 0x166650, 'w8-render-world-a-lump-find-by-type/lump_find_by_type.cpp',
    scenario_lump_find_by_type, stubs=STRSTUBS, stub_addr=STRSTUB_ADDR, returns=True)
reg('lumpQuery__FPc', 0x166E30, 'w8-render-world-a-lump-query/lump_query.cpp', scenario_lump_query,
    stubs={'strcmp': STRSTUBS['strcmp']}, stub_addr=STRSTUB_ADDR, returns=True)


def nm_effect(rng, m):
    if rng.random() < 0.25:
        a = sym('nmLumps')
        m.w32(a, m.u32(a) + rng.choice([-1, 1, 0]))


UNLOAD_STUBS = dict(STRSTUBS)
for _n in ('freeLump__FPv', 'memFree__FPv', 'textureFreeTexture__FP8_texture', 'worldFreeWorld__FP12_worldHeader',
           'textRemoveText__FPv'):
    UNLOAD_STUBS[_n] = dict(nargs=1, effect=nm_effect)
UNLOAD_ADDR = dict(STRSTUB_ADDR, freeLump__FPv=0x1646F8, memFree__FPv=0x144558,
                   textureFreeTexture__FP8_texture=0x1729C8, worldFreeWorld__FP12_worldHeader=0x1752F8,
                   textRemoveText__FPv=0x166F98)


def scenario_lump_unload(rng):
    mem = {}
    base = sym('lumps')
    n = rng.choice([0, 1, 2, 3, 5, 7])
    put32(mem, sym('nmLumps'), n)
    texts = [b'a.tex', b'b.world', b'c.uni', b'd.bin', b'noext', b'e.tex.uni', b'f.TEX', b'.world']
    ptrs = []
    for i in range(8):
        r = base + 40 * i
        fill(rng, mem, r, 40)
        data = STRARENA + 0x1000 * i
        ptrs.append(data)
        put_str(mem, data, rng.choice(texts))
        put32(mem, r + 16, rng.choice([data, data, data, ptrs[0]]))
        put32(mem, r + 20, rng.choice([0, 1, 2, 3, 0xFFFFFFFF, 0xFFFFFFFE]))
        put32(mem, r + 36, rng.choice([0, 0, 1, 0xFFFFFFFF]))
    arg = rng.choice(ptrs[:max(n, 1)] + [STRARENA + 0x9000, ptrs[-1]])
    put_str(mem, STRARENA + 0x9000, b'zzz.tex')
    return {'mem': mem, 'args': [arg], 'compare': list(range(base, base + 320)) + list(range(sym('nmLumps'), sym('nmLumps') + 4))}


reg('lumpUnload__FPv', 0x164800, 'w8-render-world-a-lump-unload/lump_unload.cpp', scenario_lump_unload,
    stubs=UNLOAD_STUBS, stub_addr=UNLOAD_ADDR)


FLOOR_ALIAS = {'norrath_floor_sprite_packet_view': 'sprPacket', 'norrath_floor_sprite_grid_view': 'sprGrid',
               'norrath_floor_free_list_word_view': 'freeList', 'norrath_floor_sprite_count_word_view': 'spriteCount',
               'norrath_floor_fade_packet_word_view': 'fadePacket', 'norrath_floor_program_start_view': 'vu1floorstart',
               'norrath_floor_program_end_view': 'vu1floorend'}


def scenario_floor_clear(rng):
    mem = {}
    pool = sym('sprPacket')
    cmp_addrs = []
    for i in range(128):
        for off in (736, 740, 744, 748, 752, 756):
            a = pool + 768 * i + off
            put32(mem, a, rng.randrange(1 << 32))
            cmp_addrs += list(range(a, a + 4))
    for name in ('spriteCount', 'freeList', 'fadePacket'):
        put32(mem, sym(name), rng.randrange(1 << 32))
        cmp_addrs += list(range(sym(name), sym(name) + 4))
    grid = sym('sprGrid')
    put32(mem, grid, rng.randrange(1 << 32))
    cmp_addrs += list(range(grid, grid + 4))
    return {'mem': mem, 'args': [], 'compare': cmp_addrs}


reg('floorClear__Fv', 0x191DE0, ['w8-render-world-a-floor-clear/floor_clear.cpp', 'game-floor-clear-source/floor_clear.cpp'],
    scenario_floor_clear, stubs=MEMSET, stub_addr=MEMSET_ADDR, alias=FLOOR_ALIAS)


def sector_effect(rng, m):
    buf = m.g32(6)
    for i in range(2048):
        m.w8(buf + i, rng.randrange(256) if rng.random() < 0.5 else 0)
    pos = 0
    if rng.random() < 0.15:
        m.w8(buf, 0)
        return
    for _ in range(rng.choice([1, 2, 3, 6, 12])):
        nlen = rng.choice([0, 1, 5, 12, 15, 30])
        rl = 34 + nlen + rng.choice([0, 0, 1, 4])
        if pos + rl + 1 >= 2048:
            break
        m.w8(buf + pos, rl)
        m.w8(buf + pos + 32, nlen)
        for k in range(nlen):
            m.w8(buf + pos + 33 + k, rng.randrange(1, 256))
        pos += rl
    m.w8(buf + pos, 0)


LOADDIR_STUBS = {'sceCdRead': dict(regs=[0, 1, 3], effect=sector_effect, ret=lambda r, m: 1),
                 'sceCdSync': dict(nargs=1, ret=lambda r, m: 0),
                 'strcpy': dict(regs=[0], str=[5], effect=lambda r, m: [m.w8(m.g32(4) + i, m.u8(m.g32(5) + i)) for i in range(len(cstr(m, m.g32(5), 400)) + 1)] and None)}
LOADDIR_ADDR = {'sceCdRead': 0x2E38E8, 'sceCdSync': 0x2E2ED8, 'strcpy': 0x304640}


def scenario_load_directory(rng):
    mem = {}
    d = ARENA
    fill(rng, mem, d, 24580)
    n = rng.choice([0, 1, 2, 3])
    return {'mem': mem, 'args': [d, rng.randrange(0, 1 << 20), n], 'compare': list(range(d, d + 24580)), 'steps': 3000000}


reg('loadDirectory__FP9directoryii', 0x164C70, 'w8-render-world-a-load-directory/load_directory.cpp', scenario_load_directory,
    stubs=LOADDIR_STUBS, stub_addr=LOADDIR_ADDR)


def scenario_megatile_center(rng):
    mem = {}
    t = sym('mt_tiledata')
    n = rng.choice([0, 1, 3, 8, 32])
    for i in range(32):
        rec = [rng.randrange(-20, 20) for _ in range(10)]
        rec[7], rec[8] = rng.randrange(-8, 8), rng.randrange(-8, 8)
        rec[4], rec[3] = rng.randrange(-1, 8), rng.randrange(-1, 8)
        rec[2] = -1 if i == n else rng.choice([0, 1, 5, -2, 127])
        for k, v in enumerate(rec):
            mem[t + 10 * i + k] = v & 0xFF
    res, pt = ARENA, ARENA + 0x100
    fill(rng, mem, res, 16)
    x = rng.choice([rng.uniform(-10, 10), float(rng.randrange(-8, 8))]) * 720.0
    y = rng.choice([rng.uniform(-10, 10), float(rng.randrange(-8, 8))]) * 720.0
    if n > 0 and rng.random() < 0.5:
        # land on a rectangle edge of one of the listed records
        k = rng.randrange(n)
        sb = lambda a: (mem[t + 10 * k + a] ^ 0x80) - 0x80
        x = float(rng.choice([sb(7), sb(7) + sb(4)])) * 720.0
        y = float(rng.choice([sb(8), sb(8) + sb(3)])) * 720.0
    putf(mem, pt, x)
    putf(mem, pt + 4, y)
    return {'mem': mem, 'args': [res, pt], 'compare': list(range(res, res + 16)) + list(range(pt, pt + 8))}


reg('getMegatileCenter__FG6Point2', 0x265810, 'w8-render-world-a-get-megatile-center/get_megatile_center.cpp',
    scenario_megatile_center)
reg('getMegatileOriginalCenter__FG6Point2', 0x265968,
    'w8-render-world-a-get-megatile-original-center/get_megatile_original_center.cpp', scenario_megatile_center)


def draw_texture_effect(rng, m):
    cur = m.u32(m.g32(4))
    n = rng.choice([0, 1, 2, 5, 9])
    half = rng.random() < 0.15
    for i in range(16 * n + (8 if half else 0)):
        m.w8(cur + i, rng.randrange(256))
    m.w32(m.g32(4), cur + 16 * n + (8 if half else 0))


TDC_STUBS = {'drawTexture__FR6PacketP8_textureiiiiii': dict(regs=[1, 2, 3, 4, 5, 6, 7], peek=[(4, 4)], effect=draw_texture_effect),
             'drawAddToList__FPviP8_textureP11_drawRecordi': dict(nargs=5)}
TDC_ADDR = {'drawTexture__FR6PacketP8_textureiiiiii': 0x16DC60, 'drawAddToList__FPviP8_textureP11_drawRecordi': 0x15EB80}


def scenario_texture_draw_color(rng):
    mem = {}
    start = ARENA + 0x2000 * rng.randrange(1, 3)
    gstart = ARENA + 0x1000 if rng.random() < 0.5 else start - 0x400
    put32(mem, sym('glistp'), start)
    put32(mem, sym('gstart'), start - rng.choice([0x1000, 0x2000, 8, 0x40]))
    put32(mem, sym('GLISTSIZE'), rng.choice([0x10000, 3000 + 0x400, 3000 + 0x3FF, 3000 + 0x401, 100]) )
    tex = ARENA + 0x20000
    fill(rng, mem, tex, 16)
    fill(rng, mem, start, 0x400)
    cmp_addrs = list(range(start, start + 0x400)) + list(range(sym('glistp'), sym('glistp') + 4))
    return {'mem': mem, 'args': [tex, rng.randrange(-100, 100), rng.randrange(-100, 100), rng.randrange(0, 50),
                                 rng.randrange(1 << 32)],
            'compare': cmp_addrs}


reg('_textureDrawColor__FP8_textureiiiUi', 0x16E658, 'w8-render-world-a-texture-draw-color/texture_draw_color.cpp',
    scenario_texture_draw_color, stubs=TDC_STUBS, stub_addr=TDC_ADDR)


def trig_fret(rng, m):
    angle = crc.s32(m.g32(4))
    return crc.fround(((angle * 7919) % 1000) / 500.0 - 1.0)


def anim_advance_effect(rng, m):
    if rng.random() < 0.5:
        putf(m.mem, m.g32(6), float(rng.randrange(-6, 7)))
        putf(m.mem, m.g32(6) + 4, float(rng.randrange(-6, 7)))
        m.w16(m.g32(7), rng.randrange(-300, 300))


CREATURE_STUBS = {
    'animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps': dict(
        nargs=4, nf=1, effect=anim_advance_effect, ret=lambda r, m: r.choice([0, 1, 2, 7, -1])),
    'icos__Fi': dict(nargs=1, fret=trig_fret),
    'isin__Fi': dict(nargs=1, fret=trig_fret)}
CREATURE_ADDR = {'animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps': 0x160830, 'icos__Fi': 0x15A070,
                 'isin__Fi': 0x15A100}


def scenario_creature_advance(rng):
    mem = {}
    obj = ARENA + 0x4000
    fill(rng, mem, obj, 0x400)
    for off in (180, 184, 224):
        putf(mem, obj + off, float(rng.randrange(-8, 9)) * rng.choice([1.0, 0.5, 0.25]))
    put32(mem, obj + 236, rng.choice([0, 0, 1, 0x31000000]))
    put32(mem, obj + 160, rng.choice([0, 0x02000000, 0x01000000, 0xFFFFFFFF]))
    put32(mem, obj + 164, rng.choice([0, 0xFFFFFFFF]))
    mem[obj + 800] = rng.choice([0, 1, 2, 30, 59, 60, 61, 127, 128, 255, rng.randrange(256)])
    mem[obj + 56] = rng.randrange(256); mem[obj + 57] = rng.randrange(256)
    w16 = rng.randrange(-300, 300) & 0xFFFF
    mem[obj + 188] = w16 & 0xFF; mem[obj + 189] = w16 >> 8
    step = float(rng.randrange(-4, 8)) * rng.choice([1.0, 0.5])
    return {'mem': mem, 'args': [obj], 'fargs': {12: f2b(step)}, 'compare': list(range(obj, obj + 0x400)),
            'returns': True}


reg('creatureAdvanceAnimation__8Creaturef', 0x111830,
    'w8-render-world-a-creature-advance-animation/creature_advance_animation.cpp', scenario_creature_advance,
    stubs=CREATURE_STUBS, stub_addr=CREATURE_ADDR, returns=True)


def mg_effect(rng, m):
    if rng.random() < 0.12:
        m.w32(sym('mgrg'), ARENA + 0x6000)


MEGA_STUBS = {'megatileBankInit__FP16MegatileInstBankPv': dict(nargs=2, effect=mg_effect),
              'megatileItemInit__FP8MegatilePv': dict(nargs=2, effect=mg_effect),
              'mtcInitialize__Fv': dict(nargs=0)}
MEGA_ADDR = {'megatileBankInit__FP16MegatileInstBankPv': 0x2660A8, 'megatileItemInit__FP8MegatilePv': 0x266100,
             'mtcInitialize__Fv': 0x266EE8}


def scenario_megatile_init(rng):
    mem = {}
    root = ARENA + 0x3000
    base = ARENA + 0x10000
    for a in (root, ARENA + 0x6000):
        fill(rng, mem, a, 0x400)
        put32(mem, a + 28, rng.choice([0, 1, 2, 3]))
        put32(mem, a + 36, rng.choice([0, 1, 2]))
        put32(mem, a + 32, 0x100)
        put32(mem, a + 40, 0x400)
        put32(mem, a + 52, rng.choice([0, 0x2000, 0x2000]))
        w = rng.choice([0, 1, 2, 3, 4, 0xFFFF, 5])
        mem[a + 48] = w & 0xFF; mem[a + 49] = w >> 8
        put32(mem, a + 56, rng.choice([0, 1, 2, 2, 3]))
        put32(mem, a + 84, rng.choice([0, 1, 2, 3, 5]) if rng.random() < 0.9 else 0x100)
        put32(mem, a + 760, rng.choice([0, 1, 1, 2]))
        put32(mem, a + 764, rng.choice([0, 1, 4, 8]))
        for e in range(8):
            mem[a + 768 + 16 * e + 16] = rng.choice([0, 1, 1, 2, 0xFF]) if e else rng.choice([0, 1, 1, 2, 0xFF])
    put32(mem, sym('mgrg'), rng.choice([0, 0x12345678]))
    # blocks reached through the relocated pointers
    fill(rng, mem, base + 0x100, 0x100)
    fill(rng, mem, base + 0x400, 0x400)
    fill(rng, mem, base + 0x2000, 0x400)
    cmp_addrs = list(range(root, root + 0x400)) + list(range(ARENA + 0x6000, ARENA + 0x6400)) + \
        list(range(base + 0x100, base + 0x200)) + list(range(base + 0x400, base + 0x800)) + \
        list(range(base + 0x2000, base + 0x2400)) + list(range(sym('mgrg'), sym('mgrg') + 4))
    return {'mem': mem, 'args': [root, base], 'compare': cmp_addrs, 'steps': 3000000}


reg('megatileInit__FP12MegatileRootPv', 0x25E0E0, 'w8-render-world-a-megatile-init/megatile_init.cpp', scenario_megatile_init,
    stubs=MEGA_STUBS, stub_addr=MEGA_ADDR)


def scenario_global_init(rng):
    mem = {}
    z = sym('zeroTile')
    fill(rng, mem, z, 0x48)
    return {'mem': mem, 'args': [], 'compare': list(range(z, z + 0x48)) + list(range(sym('edgearray'), sym('edgearray') + 256))}


reg('_GLOBAL_$I$edgearray', 0x266EC8, 'w8-render-world-a-global-init-edgearray/global_init_edgearray.cpp', scenario_global_init)
reg('_GLOBAL_$I$modelClippingOn', 0x16CE68,
    'w8-render-world-a-global-init-model-clipping-on/global_init_model_clipping_on.cpp', scenario_global_init)


WT_STUBS = {'__11ParticleDef': dict(nargs=1),
            '__11ParticleDefiiiiiiiifffiiiPFPv_v': dict(nargs=8, nf=3, stack=[0, 8, 16, 24, 32]),
            '_$_14AnimationState': dict(nargs=2), '_$_5Light': dict(nargs=2), 'norrath_list_destroy': dict(nargs=1)}
WT_ADDR = {'__11ParticleDef': 0x180D08, '__11ParticleDefiiiiiiiifffiiiPFPv_v': 0x180D78, '_$_14AnimationState': 0x164050,
           '_$_5Light': 0x13EAC8, 'norrath_list_destroy': 0x194300}


def scenario_world_tile(rng):
    mem = {}
    cmp_addrs = []
    for name, size in (('ParticleEmitterArray', 0x980), ('DynamicLightArray', 0x440), ('routeQueue', 0x1800),
                       ('worldAnimation', 0xC00)):
        a = sym(name)
        fill(rng, mem, a, size)
        cmp_addrs += list(range(a, a + size))
    return {'mem': mem, 'args': [], 'compare': cmp_addrs}


def scenario_world_tile_init(rng):
    sc = scenario_world_tile(rng)
    return sc


def scenario_world_tile_destroy(rng):
    return scenario_world_tile(rng)


reg('_GLOBAL_$I$worldTileEntityLightScale', 0x17DD70, 'w8-render-world-a-global-init-world-tile/global_init_world_tile.cpp',
    scenario_world_tile, stubs=WT_STUBS, stub_addr=WT_ADDR)


reg('_GLOBAL_$D$worldTileEntityLightScale', 0x17DD90,
    'w8-render-world-a-global-init-world-tile/global_init_world_tile.cpp', scenario_world_tile, stubs=WT_STUBS,
    stub_addr=WT_ADDR)

def rnd_float(rng):
    return rng.choice([0.0, 0.5, 1.0, 2.0, -1.0, 0.25, 3.5, 1.5, -0.5])


def draw_stubs():
    def f_rand_ptr(r, m):
        return r.choice([0, 0x31000000, 0x31000040])

    def mat_effect(r, m):
        for i in range(48):
            m.w8(m.g32(4) + i, r.randrange(256))
        if r.random() < 0.3:
            putf(m.mem, m.g32(5) + 8, rnd_float(r))

    def shadow_pos_effect(r, m):
        for i in range(12):
            m.w8(m.g32(9) + i, r.randrange(256))
        putf(m.mem, m.g32(10), rnd_float(r))

    def iter_ctor(r, m):
        m.w32(m.g32(4) + 4, m.u32(m.g32(5)))

    def iter_next(r, m):
        cur = m.u32(m.g32(4) + 4)
        m.w32(m.g32(4) + 4, m.u32(cur) if cur else 0)

    def sscanf_effect(r, m):
        for reg in (6, 7, 8, 9, 10):
            m.w32(m.g32(reg), r.choice([0, 1, 2, 5, 255, 256, 0x1FF, 0x8001, 0xFFFF, 100]))
        putf(m.mem, m.g32(11), rnd_float(r))

    return {
        'icos__Fi': dict(nargs=1, fret=trig_fret),
        'isin__Fi': dict(nargs=1, fret=trig_fret),
        'matMakeTransYawScale__FP8Matrix34G6Point3sf': dict(regs=[2], nf=1, peek=[(5, 12)], effect=mat_effect),
        'lightSetAmbientColor__FPs': dict(peek=[(4, 8)]),
        'lightSetColorScale__Ffff': dict(nf=3),
        'runStandardGlow__8Creature': dict(nargs=1),
        'modelGetNmShadowPoints__FP12_modelHeader': dict(nargs=1, ret=lambda r, m: r.choice([0, 1, 2, 3])),
        'modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f': dict(
            regs=[0, 1, 3, 4, 7], nf=1, peek=[(6, 12)], effect=shadow_pos_effect),
        'P_FaceSprite__FG6Point3ffiiiiP8_texturesi': dict(regs=[1, 2, 3, 4, 5, 6, 7], nf=2, peek=[(4, 12)]),
        'lightSetAmbientOffset__FPs': dict(peek=[(4, 8)]),
        'fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi': dict(
            regs=[0, 1, 2, 4, 5], nf=4, peek=[(7, 12), (10, 8), (11, 8)], stack=[0, 8]),
        'gameIsPaused__Fv': dict(ret=lambda r, m: r.choice([0, 0, 1])),
        '__16ListSafeIteratorP8ListHeadi': dict(regs=[1, 2], effect=iter_ctor),
        'next__16ListSafeIterator': dict(nargs=0, effect=iter_next),
        '_$_16ListSafeIterator': dict(regs=[1]),
        'vfunc': dict(nargs=1),
        'modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec': dict(
            regs=[0, 1, 3, 4], peek=[(6, 48)], ret=f_rand_ptr),
        'findTagString__10GameObjectPci': dict(regs=[0, 2], str=[5], ret=lambda r, m: r.choice([0, STRARENA + 0x2000,
                                                                                                  STRARENA + 0x2100])),
        'sscanf': dict(str=[4, 5], effect=sscanf_effect, ret=lambda r, m: 6),
        'setDirLight__Fsssssfb': dict(regs=[0, 1, 2, 3, 4, 5], nf=1),
        'modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi': dict(
            regs=[0, 1, 2, 5, 7], regs64=[10], peek=[(7, 12), (8, 48)], stack=[0], ret=f_rand_ptr),
        'lightResetOptionalParams__Fv': dict(nargs=0),
        'memset': dict(silent=True, effect=memset_effect),
    }


DRAW_ADDR = {
    'icos__Fi': 0x15A070, 'isin__Fi': 0x15A100, 'matMakeTransYawScale__FP8Matrix34G6Point3sf': 0x1577D0,
    'lightSetAmbientColor__FPs': 0x13E7A8, 'lightSetColorScale__Ffff': 0x13CA40, 'runStandardGlow__8Creature': 0x109710,
    'modelGetNmShadowPoints__FP12_modelHeader': 0x16CD50,
    'modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f': 0x16C238,
    'P_FaceSprite__FG6Point3ffiiiiP8_texturesi': 0x1815C8, 'lightSetAmbientOffset__FPs': 0x13E730,
    'fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi': 0x1DB2D8,
    'gameIsPaused__Fv': 0x1D0E70, '__16ListSafeIteratorP8ListHeadi': 0x194660, 'next__16ListSafeIterator': 0x194750,
    '_$_16ListSafeIterator': 0x194700, 'vfunc': 0x600000,
    'modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec': 0x169848, 'findTagString__10GameObjectPci': 0x143A80,
    'sscanf': 0x303F50, 'setDirLight__Fsssssfb': 0x109568,
    'modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi': 0x16AE78,
    'lightResetOptionalParams__Fv': 0x13CB88, 'memset': 0x3021B0}


def scenario_creature_draw(rng):
    mem = {}
    obj = ARENA + 0x4000
    fill(rng, mem, obj, 0x400)
    for off in (16, 20, 24, 60, 32, 224, 228, 108, 112, 116, 120):
        putf(mem, obj + off, rnd_float(rng))
    for off in (860, 868, 872):
        putf(mem, obj + off, rng.choice([0.0, 0.0, 0.5, 1.0, 2.0, -1.0, 0.9]))
    put32(mem, obj + 12, rng.choice([0, 0x100, 0x02000000, 0x02000100, rng.randrange(1 << 32)]))
    for off in (136, 144, 160, 552):
        put32(mem, obj + off, rng.choice([0, 1, 0xFFFFFFFF, rng.randrange(1 << 32)]))
        put32(mem, obj + off + 4, rng.choice([0, 0, 1, rng.randrange(1 << 32)]))
    put32(mem, obj + 152, rng.choice([0, 0, 5]))
    put32(mem, obj + 160, rng.choice([0, 0x200000, 0x40000000, 0x40200000, 0x00400000]))
    put32(mem, obj + 164, rng.choice([0, 0, 1]))
    put32(mem, obj + 252, rng.choice([0, 100, 4000]))
    w = rng.randrange(1 << 16)
    mem[obj + 56] = w & 0xFF; mem[obj + 57] = w >> 8
    for k in range(16):
        put32(mem, obj + 896 + 4 * k, rng.choice([0, 0, 0, 0, 1, 7, 90, 1000]) if rng.random() < 0.4 else 0)
    put32(mem, obj + 924, rng.choice([0, 0, 0, 0, 0, 0, 0, 3]))
    for off in (900, 904, 916, 920, 928, 932, 936, 940, 948):
        if rng.random() < 0.12:
            put32(mem, obj + off, rng.choice([1, 5, 200, 100000]))
    put32(mem, obj + 888, ARENA + 0x8000 if rng.random() < 0.7 else 0)
    # object list: node = [next, ?, ?, object], objects with a vtable entry at +24 (delta halfword, function pointer)
    vt = ARENA + 0xB000
    put32(mem, vt + 24, 0x00000010 if rng.random() < 0.5 else 0xFFF0)
    put32(mem, vt + 28, 0x600000)
    for i in range(3):
        node = ARENA + 0x8000 + 0x20 * i
        put32(mem, node, ARENA + 0x8000 + 0x20 * (i + 1) if i < rng.choice([0, 1, 2]) else 0)
        put32(mem, node + 12, ARENA + 0x9000 + 0x100 * i)
        put32(mem, ARENA + 0x9000 + 0x100 * i + 20, vt)
    # the list head word at obj+888 is the first node
    put32(mem, obj + 888, ARENA + 0x8000 if rng.random() < 0.7 else 0)
    put32(mem, sym('frameNm'), rng.choice([0, 1, 100, 4100, 70000, 0xFFFF, 0x12345]))
    put32(mem, sym('haloTexture1'), rng.randrange(1 << 32))
    put32(mem, sym('haloTexture2'), rng.randrange(1 << 32))
    model, tex, shadow = 0x31100000, 0x31200000, rng.choice([0, 0x31300000])
    put_str(mem, STRARENA + 0x2000, b'1,2,3,4,5,0.5')
    put_str(mem, STRARENA + 0x2100, b'9,8,7,6,5,0.25')
    cmp_addrs = list(range(obj, obj + 0x400)) + list(range(sym('frameNm'), sym('frameNm') + 4))
    return {'mem': mem, 'args': [obj, model, tex, shadow], 'compare': cmp_addrs, 'steps': 600000}


reg('draw__8CreatureP12_modelHeaderP8_textureT1', 0x109808, 'w8-render-world-a-creature-draw/creature_draw.cpp',
    scenario_creature_draw, stubs=draw_stubs(), stub_addr=DRAW_ADDR, fixed={'vfunc': 0x600000}, returns=True)

def ef_stubs():
    def hit_fret(r, m):
        return r.choice([-0.5, 0.0, 0.3, 0.5, 0.9, 1.0, 1.5, 0.75, 0.01])

    def get_vert_ret(r, m):
        if r.random() < 0.15:
            return 0
        return ARENA + 0x20000 + 0x40 * r.randrange(4)

    def init_pos_effect(r, m):
        m.w32(m.g32(4) + 32, r.choice([0, 1, 2, 5]))

    def stricmp_ret(r, m):
        a, b = cstr(m, m.g32(4)).lower(), cstr(m, m.g32(5)).lower()
        return (a > b) - (a < b)

    def strcpy_effect(r, m):
        src = cstr(m, m.g32(5), 400)
        for i, c in enumerate(src + b'\0'):
            m.w8(m.g32(4) + i, c)

    return {
        'vfunc_a': dict(regs=[0], peek=[(5, 12)]),
        'vfunc_b': dict(regs=[0], ret=lambda r, m: STRARENA + 0x3000),
        'findTagString__10GameObjectPci': dict(regs=[0, 2], str=[5], ret=lambda r, m: r.choice([0, STRARENA + 0x2000])),
        'gameLineInitPathState__FP17gameLinePathStatePcP6Point3': dict(regs=[2], str=[5]),
        'gameLineGet__FPc': dict(str=[4], ret=lambda r, m: ARENA + 0x1F000),
        'initPosition__FP17gameLinePathState': dict(effect=init_pos_effect),
        'gameLineGetVert__Fii': dict(nargs=2, ret=get_vert_ret),
        'objectUpdateInGrid__FP10GameObject': dict(nargs=1),
        'resetRouting__8Creature': dict(nargs=1),
        'addEffect__FiG6Point3': dict(nargs=1, peek=[(5, 12)]),
        'eRand__10GameObject': dict(nargs=1, ret=lambda r, m: r.choice([0, 1, 7, 99, 12345, 0x7FFFFFFF, 0xFFFFFFF0])),
        'gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb': dict(
            nargs=6, nf=2, nonnull=[10], peek=[(10, 16, 16), (10, 32, 32)], regs=[0, 1, 2, 3, 4, 5, 7]),
        'iatan2__Fff': dict(nf=2, ret=lambda r, m: r.choice([0, 100, 4000, 70000, 0xFFFF8123, 32767, 5])),
        'icos__Fi': dict(nargs=1, fret=trig_fret),
        'isin__Fi': dict(nargs=1, fret=trig_fret),
        'worldHitscan__FP12_worldHeaderG6Point3T1fi': dict(regs=[0, 3], nf=1, peek=[(5, 12), (6, 12)], fret=hit_fret),
        'netCheckActivation__FG6Point3f': dict(peek=[(4, 12)], nf=1, ret=lambda r, m: r.choice([0, 1])),
        'netGetLocalObject__FP8Creature': dict(nargs=1, ret=lambda r, m: r.choice([ARENA + 0x4000, ARENA + 0x4000, 0x31000000])),
        'isBoss__FP10GameObject': dict(nargs=1, ret=lambda r, m: r.choice([0, 1])),
        'fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff': dict(
            regs=[0, 1, 3, 4], peek=[(6, 12)], nf=4),
        'fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi': dict(
            regs=[0, 1, 3, 4, 5], peek=[(6, 12)], nf=3),
        'strcasecmp': dict(str=[4, 5], ret=stricmp_ret),
        'strcpy': dict(regs=[0], str=[5], effect=strcpy_effect),
        'getClosestLivingPlayer__FG6Point3f': dict(peek=[(4, 12)], nf=1, ret=lambda r, m: r.choice([-1, 0, 1, 2, 3])),
    }


EF_ADDR = {
    'vfunc_a': 0x600000, 'vfunc_b': 0x600010,
    'findTagString__10GameObjectPci': 0x143A80, 'gameLineInitPathState__FP17gameLinePathStatePcP6Point3': 0x279B58,
    'gameLineGet__FPc': 0x279AD0, 'initPosition__FP17gameLinePathState': 0x278B48, 'gameLineGetVert__Fii': 0x279BC8,
    'objectUpdateInGrid__FP10GameObject': 0x143630, 'resetRouting__8Creature': 0x113460, 'addEffect__FiG6Point3': 0x1D5630,
    'eRand__10GameObject': 0x153FA8,
    'gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb': 0x1CE218, 'iatan2__Fff': 0x15A198,
    'icos__Fi': 0x15A070, 'isin__Fi': 0x15A100, 'worldHitscan__FP12_worldHeaderG6Point3T1fi': 0x17B7E0,
    'netCheckActivation__FG6Point3f': 0x135658, 'netGetLocalObject__FP8Creature': 0x1353D8, 'isBoss__FP10GameObject': 0x10B1C8,
    'fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff': 0x1DACE0,
    'fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi': 0x1DBBB0,
    'strcasecmp': 0x304168, 'strcpy': 0x304640, 'getClosestLivingPlayer__FG6Point3f': 0x10EB18}


def scenario_every_frame(rng):
    mem = {}
    obj = ARENA + 0x4000
    fill(rng, mem, obj, 0x400)
    pick = rng.choice
    for off in (16, 20, 24, 32, 60, 64, 92, 104, 224, 228, 560, 564, 776, 780, 784):
        putf(mem, obj + off, rnd_float(rng) if rng.random() < 0.7 else pick([60.0, 100.0, -100.0, 700.0]))
    put32(mem, obj + 12, pick([0, 0, 0x40, 8, 0x4000000, 0x4000040, 0x48, rng.randrange(1 << 32)]))
    flags = pick([0, 1, 0x08000000, 0x00400000, 0x00800000, 0x00C00000, 0x80000001, 0x08000001, 0x00C00001])
    put32(mem, obj + 160, flags)
    put32(mem, obj + 164, pick([0, 0, 0xFFFFFFFF]))
    put32(mem, obj + 72, ARENA + 0x6000)
    put32(mem, ARENA + 0x6000 + 136, 0x00000008 if rng.random() < 0.5 else 0xFFF8)
    put32(mem, ARENA + 0x6000 + 140, 0x600000)
    put32(mem, ARENA + 0x6000 + 152, 0x00000010)
    put32(mem, ARENA + 0x6000 + 156, 0x600010)
    for base in (140, 148):
        for i in range(4):
            v = pick([0, 1, 2, 3, -1, -2, -3, 100, -100, 32767, -32768])
            mem[obj + base + 2 * i] = v & 0xFF
            mem[obj + base + 2 * i + 1] = (v >> 8) & 0xFF
    def h(off, vals):
        v = pick(vals) & 0xFFFF
        mem[obj + off] = v & 0xFF
        mem[obj + off + 1] = v >> 8
    h(792, [0, 0, 1, 2, -1, 5]); h(796, [0, 0, 1, 3]); h(794, [0, 1, 3]); h(56, [0, 100, 0x7FFF, 0xFFFF, 0x1234])
    h(880, [0, 2, 5, -3]); h(882, [0, 2, 9, 5]); h(884, [1, 3]); h(886, [1, 4, 7])
    v808 = pick([0, 50, 99, 100, 1000, 1001])
    put32(mem, obj + 804, pick([0, 100, 1000])); put32(mem, obj + 808, v808)
    put32(mem, obj + 80, pick([0, 600, 1100, 2000, v808, v808 + 600]))
    mem[obj + 84] = pick([0, 3, 5])
    mem[obj + 85] = pick([0xFF, 0, 1, 2, 5, 0x7F, 3])
    mem[obj + 40] = pick([0, 7, 200])
    mem[obj + 800] = pick([60, 0, 1, 20, 29, 30, 31, 59, 100, 200, 0xF0])
    put32(mem, obj + 236, pick([0, 0, 1, 0x31000000]))
    put32(mem, obj + 252, pick([0, 100, 970, 990]))
    put32(mem, obj + 924, pick([0, 0, 0, 5, 30, 31, 50, -3, 1000]))
    for off in (856, 864, 860, 868, 872, 876):
        putf(mem, obj + off, pick([0.0, 0.0, 0.01, 0.5, 1.0, 1.5, 70.0, -1.0, 0.03]))
    put32(mem, obj + 788, 0)
    putf(mem, obj + 788, pick([0.0, 100.0, 500.0, 5.0, 50.0]))
    put32(mem, sym('frameNm'), pick([0, 4, 32, 33, 64, 100, 128, 1000, 31, 3]))
    put32(mem, sym('nmPlayers'), pick([0, 1, 2, 4]))
    put32(mem, sym('localNmPlayersAlive'), pick([0, 1, 2, 4]))
    for i in range(4):
        put32(mem, sym('localPlayersAlive') + 4 * i, pick([0, 1, 2, 3]))
    put32(mem, sym('levelFlags'), pick([0, 0x100, 0x1FF]))
    mem[sym('netNetworkMode')] = pick([0, 1])
    put32(mem, sym('world'), pick([0, 0x31000000]))
    pl = sym('player')
    for i in range(4):
        o = ARENA + 0x10000 + 0x200 * i
        fill(rng, mem, o, 0x80)
        put32(mem, pl + 23984 * i + 21872, pick([0, o, o]))
        put32(mem, o + 12, pick([0, 8, 1, 0x18]))
        for off in (16, 20, 24):
            putf(mem, o + off, rnd_float(rng))
        w = rng.randrange(1 << 16)
        mem[o + 56] = w & 0xFF; mem[o + 57] = w >> 8
    for a, txt in ((sym('levelName') + 81, rng.choice([b'kelethin', b'KELETHIN', b'other', b''])),):
        put_str(mem, a, txt)
    put_str(mem, STRARENA + 0x2000, b'path1')
    put_str(mem, STRARENA + 0x3000, rng.choice([b'boss name', b'']))
    for k in range(4):
        v = ARENA + 0x20000 + 0x40 * k
        fill(rng, mem, v, 0x40)
        for off in (0, 4, 8):
            putf(mem, v + off, rnd_float(rng) if rng.random() < 0.6 else pick([60.0, -100.0, 0.0, 700.0]))
        mem[v + 60] = pick([0, 3, 5])
    put32(mem, sym('bossMeterCurrent'), 77); put32(mem, sym('bossMeterMax'), 88)
    cmp_addrs = (list(range(obj, obj + 0x400)) + list(range(sym('bossMeterCurrent'), sym('bossMeterCurrent') + 4)) +
                 list(range(sym('bossMeterMax'), sym('bossMeterMax') + 4)) +
                 list(range(sym('bossMeterName'), sym('bossMeterName') + 128)))
    return {'mem': mem, 'args': [obj, 0x31100000], 'compare': cmp_addrs, 'steps': 800000}


reg('everyFrame__8CreatureP12_modelHeader', 0x10EBF8, 'w8-render-world-a-creature-every-frame/creature_every_frame.cpp',
    scenario_every_frame, stubs=ef_stubs(), stub_addr=EF_ADDR, fixed={'vfunc_a': 0x600000, 'vfunc_b': 0x600010},
    returns=True)


def scenario_standard_glow(rng):
    mem = {}
    obj = ARENA + 0x4000
    fill(rng, mem, obj, 0x200)
    mem[obj + 156] = rng.choice([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xFF, 0x80, rng.randrange(256)])
    for off in (140, 142, 144, 146):
        v = rng.choice([-30, -1, 0, 5, 10, 14, 15, 19, 20, 29, 30, 49, 50, 51, 32767, -32768, rng.randrange(-100, 100)]) & 0xFFFF
        mem[obj + off] = v & 0xFF; mem[obj + off + 1] = v >> 8
    return {'mem': mem, 'args': [obj], 'compare': list(range(obj, obj + 0x200))}


reg('runStandardGlow__8Creature', 0x109710, 'w8-render-world-a-run-standard-glow/run_standard_glow.cpp', scenario_standard_glow)


if __name__ == '__main__':
    runs = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    names = list(FUNCTIONS) if sys.argv[1] == 'all' else [sys.argv[1]]
    worst = 0
    for n in names:
        worst = max(worst, compare(n, runs))
    sys.exit(worst)
