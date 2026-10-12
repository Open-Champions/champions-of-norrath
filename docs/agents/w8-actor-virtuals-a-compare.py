#!/usr/bin/env python3
"""Differential check of the actor destructors and type-info functions against the private executable.

Runs one function from the private SLUS_205.65 image and the same function built from the
authored source (compiled with the reviewed EE toolchain and linked at a scratch address) in
the small R5900 interpreter of combat-reactions-compare.py. Both runs get the same randomized
scenario. Every call to an imported function is intercepted and logged with its argument
registers (strings read through their pointer); the logs, the bytes of the scenario object
and the return value must be equal. The executable and the toolchain stay outside the
repository and nothing from the executable is stored.

  NORRATH_EXE=/path/SLUS_205.65 NORRATH_EE=/path/to/ee \\
      python3 docs/agents/w8-actor-virtuals-a-compare.py all 1000

The data symbols the authored object imports (vtables, base type-info objects, the world
pointer) are placed at the addresses they have in the image, so a stored vtable word or a
passed type-info pointer can be compared directly.
"""
import importlib.util
import os
import random
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
spec = importlib.util.spec_from_file_location('crc', os.path.join(HERE, 'combat-reactions-compare.py'))
crc = importlib.util.module_from_spec(spec)
spec.loader.exec_module(crc)
Image, Machine, Stop, M32, P, FLAGS, GP, SRC, SCRATCH, EXE = (
    crc.Image, crc.Machine, crc.Stop, crc.M32, crc.P, crc.FLAGS, crc.GP, crc.SRC, crc.SCRATCH, crc.EXE)

OBJ = 0x30000000
OBJ_SIZE = 0x200
AUX = 0x30010000          # AUX + 0x100 * i, helper blocks the object points at
STACK = 0x3F000000
WORLD_WORD = 0x4844A0
STUB_BASE = 0x00500000

# authored import -> original address
FIXED = {
    'norrath_vt_dtor_10GameObject': 0x141658, 'norrath_block_free': 0x1447D8,
    'norrath_vt_tf_10GameObject': 0x312DD0, 'norrath_ti_10GameObject': 0x486DC8,
    '_$_8Creature': 0x112340, '_$_8AutoProp': 0x224C70, '__tf8Creature': 0x30D510, '__tf8AutoProp': 0x314EF0,
    '__ti8Creature': 0x8F0000 - 21640, '__ti8AutoProp': 0x8F0000 - 19176,
    '__rtti_si': 0x2FD348, '__builtin_delete': 0x2FEDF0, '_$_15FxCreatureState': 0x1E5DB0,
    '_$_14AnimationState': 0x164050,
    'worldSetDoorHeight__FP12_worldHeaderif': 0x17D410, 'worldReleaseDoor__FP12_worldHeaderi': 0x17D318,
    'norrath_actors_b_view_4844a0': WORLD_WORD,
}
# imported functions the check intercepts, with the number of integer argument registers they take
CALLS = {'norrath_vt_dtor_10GameObject': 2, 'norrath_block_free': 1, 'norrath_vt_tf_10GameObject': 0,
         '_$_8Creature': 2, '_$_8AutoProp': 2, '__tf8Creature': 0, '__tf8AutoProp': 0, '__rtti_si': 3,
         '__builtin_delete': 1, '_$_15FxCreatureState': 2, '_$_14AnimationState': 2,
         'worldSetDoorHeight__FP12_worldHeaderif': 2, 'worldReleaseDoor__FP12_worldHeaderi': 2}


def address_of(name):
    if name in FIXED:
        return FIXED[name]
    if '_view_' in name:
        return int(name.rsplit('_', 1)[1], 16)
    raise SystemExit('unmapped authored import: ' + name)


def build(rel, work, bss=None):
    base = os.path.basename(rel).replace('.cpp', '')
    obj = os.path.join(work, base + '.o')
    subprocess.check_call([P + 'g++', *FLAGS, '-c', os.path.join(SRC, rel), '-o', obj])
    und = [l.split()[-1] for l in subprocess.check_output([P + 'nm', '-u', obj], text=True).splitlines() if l.strip()]
    defs = {n: address_of(n) for n in und}
    elf = os.path.join(work, base + '.elf')
    cmd = [P + 'ld', '-m', 'elf32lr5900', '-Ttext=0x00400000', '--no-warn-mismatch']
    if bss:
        cmd.append('--section-start=.bss=%#x' % bss)
    cmd += ['--defsym=%s=%#x' % kv for kv in defs.items()] + ['-o', elf, obj]
    subprocess.check_call(cmd)
    return elf, set(und)


def symbol_address(elf, name):
    for l in subprocess.check_output([P + 'nm', elf], text=True).splitlines():
        parts = l.split()
        if len(parts) == 3 and parts[2] == name:
            return int(parts[0], 16)
    raise SystemExit('missing symbol ' + name)


def cstr(m, a):
    out = bytearray()
    while len(out) < 40:
        c = m.u8(a + len(out))
        if c == 0:
            break
        out.append(c)
    return bytes(out)


def make_stubs(m, rng, authored_stub_of):
    """authored_stub_of maps an import name to the address the stub is installed at."""
    def log_call(name):
        def h(mm):
            if name == '__rtti_si':
                mm.log.append((name, mm.g32(4), cstr(mm, mm.g32(5)), mm.g32(6)))
            else:
                entry = (name,) + tuple(mm.g32(4 + i) for i in range(CALLS[name]))
                if name.startswith('worldSetDoorHeight'):
                    entry += (mm.f[12],)
                mm.log.append(entry)
            if name.startswith('__tf'):
                mm.setr32(2, 0x8F0000)
            else:
                mm.setr32(2, rng.randrange(1 << 32))
        return h
    for name, at in authored_stub_of.items():
        m.stubs[at] = log_call(name)


def fill(rng, mem, base, size):
    for i in range(size):
        mem[base + i] = rng.randrange(256)


def put32(mem, a, v):
    for i in range(4):
        mem[a + i] = (v >> (8 * i)) & 0xFF


def put16(mem, a, v):
    mem[a] = v & 0xFF
    mem[a + 1] = (v >> 8) & 0xFF


FLAGS_POOL = [0, 1, 2, 3, 4, 5, 0x80, 0xFFFFFFFF, 0xFFFFFFFE, 0x100, 0x101]


def scenario_dtor(rng):
    mem = {}
    fill(rng, mem, OBJ, OBJ_SIZE)
    return {'mem': mem, 'args': [OBJ, rng.choice(FLAGS_POOL) if rng.random() < 0.7 else rng.randrange(1 << 32)],
            'compare': list(range(OBJ, OBJ + OBJ_SIZE))}


def scenario_tf(rng):
    mem = {}
    ti = SCEN['ti']
    for i in range(12):
        mem[ti + i] = rng.choice([0, 0, rng.randrange(256)])
    if rng.random() < 0.5:
        for i in range(4):
            mem[ti + i] = 0
    return {'mem': mem, 'args': [], 'compare': list(range(ti, ti + 12)), 'returns': True}


def scenario_world_part(rng):
    mem = {}
    fill(rng, mem, OBJ, OBJ_SIZE)
    put32(mem, WORLD_WORD, rng.choice([0, 0x31000000, rng.randrange(1 << 32)]))
    put16(mem, OBJ + 76, rng.choice([0, 1, 7, 0x7FFF, 0xFFFF, 0x8000, rng.randrange(1 << 16)]))
    put16(mem, OBJ + 78, rng.choice([0, 1, 100, 0x7FFF, 0xFFFF, 0x8000, rng.randrange(1 << 16)]))
    return {'mem': mem, 'args': [OBJ, rng.choice(FLAGS_POOL)], 'compare': list(range(OBJ, OBJ + OBJ_SIZE)) + list(range(WORLD_WORD, WORLD_WORD + 4))}


def scenario_anim_state(rng):
    mem = {}
    fill(rng, mem, OBJ, OBJ_SIZE)
    for i in range(12):
        fill(rng, mem, AUX + 0x800 * i, 0x800)
    # owner pointer at +0 (null half of the time); child pointers at 12 + 32 * i
    put32(mem, OBJ, rng.choice([0, AUX]))
    mem[OBJ + 4] = rng.randrange(256)
    mem[OBJ + 5] = rng.choice([0, 1, 2, 3, 4, 5, 6, 0x7F, 0x80, 0xFF, 7])
    for i in range(7):
        put32(mem, OBJ + 12 + 32 * i, rng.choice([0, AUX + 0x800 * (i + 1)]))
    # the owner's back-pointer lives 1552 bytes in, the child's at +20
    return {'mem': mem, 'args': [OBJ, rng.choice(FLAGS_POOL)],
            'compare': list(range(OBJ, OBJ + OBJ_SIZE + 0x40)) + list(range(AUX, AUX + 0x800 * 8))}


FUNCTIONS = {}
SCEN = {}


def reg(orig_sym, orig_addr, src, auth_sym, scenario, ti=None, extra_compare_returns=True):
    FUNCTIONS[orig_sym] = dict(orig=orig_addr, src=src, auth=auth_sym, scenario=scenario, ti=ti)


def compare(name, runs):
    spec = FUNCTIONS[name]
    work = os.path.join(SCRATCH, name.replace('$', 'S'))
    os.makedirs(work, exist_ok=True)
    exe = Image(EXE)
    elf, imports = build(spec['src'], work, bss=spec['ti'])
    auth = Image(elf)
    auth_entry = symbol_address(elf, spec['auth'])
    SCEN['ti'] = spec['ti']
    calls = sorted(n for n in imports if n in CALLS)
    bad = 0
    for seed in range(runs):
        sc = spec['scenario'](random.Random(seed))
        results = []
        for image, entry, side in ((exe, spec['orig'], 'orig'), (auth, auth_entry, 'auth')):
            mem = dict(sc['mem'])
            m = Machine(image, mem, {}, GP)
            rng = random.Random(seed ^ 0x77)
            # both sides intercept the same logical imports; stub entry = the import's image address
            make_stubs(m, rng, {n: address_of(n) for n in calls})
            m.r[29] = STACK
            for i, v in enumerate(sc['args']):
                m.r[4 + i] = v & M32
            m.log = []
            try:
                m.run(entry)
            except Stop as e:
                print('seed %d: stopped: %s' % (seed, e))
                return 2
            results.append((m.log, {a: m.u8(a) for a in sc['compare']}, m.g32(2) if sc.get('returns') else None))
        if results[0] != results[1]:
            bad += 1
            if bad <= 3:
                a, b = results
                print('MISMATCH seed %d' % seed)
                if a[0] != b[0]:
                    print('  log orig', a[0][:4], '\n  log auth', b[0][:4])
                diff = [hex(x) for x in a[1] if a[1][x] != b[1][x]][:10]
                if diff:
                    print('  memory differs at', diff)
                if a[2] != b[2]:
                    print('  v0', a[2], b[2])
    print('%s: %d runs, %d mismatches' % (name, runs, bad))
    return 1 if bad else 0


TI_ADDR = lambda a: a


def load_registry():
    path = os.path.join(HERE, 'w8-actor-virtuals-a-compare.json')
    import json
    for e in json.load(open(path)):
        scen = {'dtor': scenario_dtor, 'tf': scenario_tf, 'world_part': scenario_world_part,
                'anim_state': scenario_anim_state}[e['scenario']]
        reg(e['orig_sym'], e['orig_addr'], e['src'], e['auth_sym'], scen, e.get('ti'))


if __name__ == '__main__':
    load_registry()
    runs = int(sys.argv[2]) if len(sys.argv) > 2 else 500
    names = list(FUNCTIONS) if sys.argv[1] == 'all' else [sys.argv[1]]
    worst = 0
    for n in names:
        worst = max(worst, compare(n, runs))
    sys.exit(worst)
