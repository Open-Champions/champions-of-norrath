#!/usr/bin/env python3
"""Differential check of the w8-actor-virtuals-c bodies against the private executable.

Runs one function from the private SLUS_205.65 image and the same function built
from the authored source (compiled and linked with the reviewed EE toolchain at a
scratch address) in the small R5900 interpreter of combat-reactions-compare.py.
Both runs get the same randomized scenario memory and the same stubbed callees.
The stub call logs (selected argument registers, pointed-to bytes, float
arguments) and every compared byte of scenario memory afterwards must be equal.
Stubs answer from a seeded generator, identically for both runs. Callees are
looked up by their original ELF symbol name in the executable; the authored
object may call a differently named body, which ALIAS maps back.

  NORRATH_EXE=/path/SLUS_205.65 NORRATH_EE=/path/to/ee \\
      python3 docs/agents/w8-actor-virtuals-c-compare.py <symbol|all> [runs]

Scenario floats are small integers, so the EE's truncating rounding and the
interpreter's round-to-nearest agree. Any opcode the interpreter lacks stops the
run with its address. Nothing from the executable is stored; scratch files go to
a fresh temporary directory.
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
Machine, Image, Stop = cr.Machine, cr.Image, cr.Stop
M32 = cr.M32
P, FLAGS, GP, EXE, SRC = cr.P, cr.FLAGS, cr.GP, cr.EXE, cr.SRC
SCRATCH = cr.SCRATCH
put32, putf, fill = cr.put32, cr.putf, cr.fill

# --- Machine additions: sqrt.s -------------------------------------------------
_old_cop1 = Machine.cop1


def _cop1(self, ins, pc, rs, rt, rd, sa, fn, tgt):
    if rs == 0x10 and fn == 4:
        a = self.fv(rd)
        self.setf(sa, a ** 0.5 if a >= 0 else float('nan'))
        return None
    return _old_cop1(self, ins, pc, rs, rt, rd, sa, fn, tgt)


Machine.cop1 = _cop1

OBJ = 0x30000000
OBJ_SIZE = 0x400
OTHER = 0x30010000
VT = 0x30020000
EXTRA = 0x30040000
STACK = 0x3F000000
STUB = 0x00500000
VSTUB = 0x00600000

# authored callee name -> original ELF name, when the authored side calls a body
# defined under a different name
ALIAS = {
    'norrath_list_destroy': 'listDestroy__FP8ListHead',
    'norrath_actor_add_run_list': 'objectAddToRunList__FP10GameObject',
    'norrath_actor_remove_from_run_list': 'objectRemoveFromRunList__FP10GameObject',
    'norrath_sound_play_sequence': 'soundPlaySequence__FP15AnimationHeaderP10GameObjectf',
    'norrath_lump_find_resource': 'lumpFindResource__FPcT0',
    'norrath_animation_set_to_end': 'animSetToEnd__FP14AnimationState',
    'norrath_object_add_to_delete_list': 'objectAddToDeleteList__FP10GameObject',
}
# data symbols the authored object may import -> original address
DATA = {}


def exe_symbols():
    out = {}
    for line in subprocess.check_output([P + 'nm', EXE], text=True).splitlines():
        parts = line.split()
        if len(parts) == 3:
            out.setdefault(parts[2], int(parts[0], 16))
    return out


EXESYM = None


def cstr(m, a, limit=64):
    out = bytearray()
    while len(out) < limit:
        c = m.u8(a + len(out))
        if c == 0:
            break
        out.append(c)
    return bytes(out)


def make_iterator(name, objects):
    """Stub for a callback-iterating callee: calls a0 once per scenario object."""
    def handler(m):
        callback = m.g32(4)
        m.log.append((name,))
        saved = (m.pc, list(m.r))
        for obj in objects:
            m.log.append(('visit', obj))
            m.r[4] = obj
            m.run(callback)
            m.r = list(saved[1])
        m.pc = saved[0]
    return handler


def make_stub(name, kinds, ret, rng):
    """kinds: one token per argument register: i int, o object address (as value),
    s<N> N bytes at pointer, c C string, x ignored, f float register."""
    def handler(m):
        entry = [name]
        ai = 0
        fi = 12
        for tok in kinds:
            if tok == 'f':
                entry.append(('f', m.f[fi]))
                fi += 1
                continue
            v = m.g32(4 + ai)
            if tok == 'i' or tok == 'o':
                entry.append(v)
            elif tok == 'x':
                pass
            elif tok == 'c':
                entry.append(cstr(m, v))
            elif tok[0] == 's':
                entry.append(m.read(v, int(tok[1:])))
            ai += 1
        m.log.append(tuple(entry))
        if ret is not None:
            kind, pool = ret
            val = rng.choice(pool)
            if kind == 'i':
                m.setr32(2, val)
            elif kind == 'f':
                m.f[0] = cr.f2b(float(val))
    return handler


def build_authored(rel, work, stubnames):
    base = os.path.basename(rel).replace('.cpp', '')
    obj = os.path.join(work, base + '.o')
    subprocess.check_call([P + 'g++', *FLAGS, '-c', os.path.join(SRC, rel), '-o', obj])
    und = [l.split()[-1] for l in subprocess.check_output([P + 'nm', '-u', obj], text=True).splitlines() if l.strip()]
    defsyms = {}
    for name in und:
        if name in DATA:
            defsyms[name] = DATA[name]
        else:
            orig = ALIAS.get(name, name)
            if orig not in stubnames:
                raise SystemExit('unmapped authored import: ' + name)
            defsyms[name] = STUB + 0x10 * stubnames.index(orig)
    out = os.path.join(work, base + '.elf')
    cmd = [P + 'ld', '-m', 'elf32lr5900', '-Ttext=0x00400000', '--no-warn-mismatch']
    cmd += ['--defsym=%s=%#x' % kv for kv in defsyms.items()]
    cmd += ['-o', out, obj]
    subprocess.check_call(cmd)
    return out


def symbol_address(elf, name):
    for l in subprocess.check_output([P + 'nm', elf], text=True).splitlines():
        parts = l.split()
        if len(parts) == 3 and parts[2] == name:
            return int(parts[0], 16)
    raise SystemExit('missing symbol ' + name)


def run_one(image, entry, stub_addrs, spec, scenario, seed):
    mem = dict(scenario['mem'])
    stubs = {}
    m = Machine(image, mem, stubs, GP)
    rng = random.Random(seed ^ 0x5eed)
    for name, (kinds, ret) in spec['stubs'].items():
        if kinds == 'iterate':
            stubs[stub_addrs[name]] = make_iterator(name, scenario['iterate'])
        else:
            stubs[stub_addrs[name]] = make_stub(name, kinds, ret, rng)
    for addr, (name, kinds, ret) in scenario.get('vstubs', {}).items():
        stubs[addr] = make_stub(name, kinds, ret, rng)
    m.r[29] = STACK
    for i, v in enumerate(scenario['args']):
        m.r[4 + i] = v & M32
    for i, v in scenario.get('fargs', {}).items():
        m.f[i] = v
    m.log = []
    m.run(entry)
    return m


def compare(name, runs=200):
    global EXESYM
    spec = FUNCTIONS[name]
    work = os.path.join(SCRATCH, name)
    os.makedirs(work, exist_ok=True)
    if EXESYM is None:
        EXESYM = exe_symbols()
    exe = Image(EXE)
    names = list(spec['stubs'])
    elf = build_authored(spec['src'], work, names)
    auth = Image(elf)
    auth_entry = symbol_address(elf, spec.get('auth_symbol', name))
    orig_entry = EXESYM[spec.get('orig_symbol', name)]
    orig_stubs = {k: EXESYM[k] for k in names}
    auth_stubs = {k: STUB + 0x10 * names.index(k) for k in names}
    bad = 0
    stats = {}
    for seed in range(runs):
        sc = spec['scenario'](random.Random(seed))
        results = []
        for image, entry, stubs in ((exe, orig_entry, orig_stubs), (auth, auth_entry, auth_stubs)):
            try:
                m = run_one(image, entry, stubs, spec, sc, seed)
            except Stop as e:
                print('seed %d: stopped: %s' % (seed, e))
                return 2
            region = {a: m.u8(a) for a in sc['compare']}
            results.append((m.log, region, m.g32(2) if spec.get('returns') else None))
        for e in results[0][0]:
            stats[e[0]] = stats.get(e[0], 0) + 1
        if results[0] != results[1]:
            bad += 1
            if bad <= 3:
                print('MISMATCH seed %d' % seed)
                a, b = results
                if a[0] != b[0]:
                    n = min(len(a[0]), len(b[0]))
                    k = next((i for i in range(n) if a[0][i] != b[0][i]), n)
                    print('  log differs at %d:\n   orig %s\n   auth %s' % (k, a[0][k:k + 2], b[0][k:k + 2]))
                diff = [hex(x) for x in a[1] if a[1][x] != b[1][x]][:10]
                if diff:
                    print('  memory differs at', diff)
                if a[2] != b[2]:
                    print('  return', a[2], b[2])
    print('%s: %d runs, %d mismatches; original calls %s' % (name, runs, bad, stats))
    return 1 if bad else 0


FUNCTIONS = {}


def reg(symbol, src, scenario, stubs, **kw):
    FUNCTIONS[symbol] = dict(symbol=symbol, src=src, scenario=scenario, stubs=stubs, **kw)


# ---- scenario helpers ----

def new_object(rng):
    mem = {}
    fill(rng, mem, OBJ, OBJ_SIZE)
    fill(rng, mem, OTHER, 0x400)
    return mem


def whole_object():
    return list(range(OBJ, OBJ + OBJ_SIZE)) + list(range(OTHER, OTHER + 0x400))


def vtable(rng, mem, slots):
    """Install virtual-table slots: {byte_offset: (stub address, adjustment)}."""
    fill(rng, mem, VT, 0x100)
    for off, (addr, adj) in slots.items():
        mem[VT + off] = adj & 0xFF
        mem[VT + off + 1] = (adj >> 8) & 0xFF
        put32(mem, VT + off + 4, addr)
    put32(mem, OBJ + 72, VT)


# ---- scenarios ----

def sc_wake(rng):
    mem = new_object(rng)
    objs = [OBJ, OTHER, OTHER + 0x100]
    for o in objs:
        put32(mem, o + 12, rng.choice([0, 0x20, 0x40, 0x60, 0xFFFFFFFF, 0xFFFFFFDF, 0x2000020]))
        mem[o + 570] = rng.choice([0, 0x10, 0xFF, 0xEF, 0x11, 0x80])
    return {'mem': mem, 'args': [], 'compare': whole_object(), 'iterate': objs}


def sc_sleep(rng):
    mem = new_object(rng)
    put32(mem, OBJ + 12, rng.choice([0, 0x20, 0x40, 0x60, 0xFFFFFFFF, 0xFFFFFFDF, 0x2000020, 0x20]))
    mem[OBJ + 570] = rng.choice([0, 0, 0x10, 0xFF, 0x01, 0x80])
    put32(mem, OBJ + 572, rng.choice([10, 9, 11, 0x1000A, 0, 0xFFFF000A]))
    vtable(rng, mem, {184: (VSTUB, rng.choice([0, 8, -8, 0x70]))})
    return {'mem': mem, 'args': [OBJ], 'compare': whole_object(),
            'vstubs': {VSTUB: ('virt184', ['o', 'i'], None)}}


def sc_save_activate(rng):
    mem = new_object(rng)
    return {'mem': mem, 'args': [OBJ], 'compare': whole_object()}


def sc_noop(rng):
    mem = new_object(rng)
    return {'mem': mem, 'args': [OBJ, rng.randrange(1 << 32)], 'compare': whole_object()}


def sc_save_buffer(rng):
    mem = new_object(rng)
    fill(rng, mem, EXTRA, 0x200)
    return {'mem': mem, 'args': [OBJ, EXTRA + rng.choice([0, 1, 2, 3, 4, 8])],
            'compare': whole_object() + list(range(EXTRA, EXTRA + 0x200))}


def sc_obj_int(rng):
    mem = new_object(rng)
    return {'mem': mem, 'args': [OBJ, rng.choice([0, 1, -1, 2, 7, 0x100])], 'compare': whole_object()}


def sc_obj_bool(rng):
    mem = new_object(rng)
    put32(mem, OBJ + 12, rng.choice([0, 1, 0xFFFFFFFE, 0xFFFFFFFF, 0x100]))
    return {'mem': mem, 'args': [OBJ, rng.choice([0, 1, 2, 0x100])], 'compare': whole_object()}


ONE_ARG_OBJ = ['o']
RUNLIST = ('objectAddToRunList__FP10GameObject', ['o'], ('i', [0, 1, 0x30001234, 0xFFFFFFFF]))

reg('wakeMonsters__8creaturev', 'w8-actor-virtuals-c-wake-monsters/wake_monsters.cpp', sc_wake,
    {'objectIterateRunLists__FPFP10GameObject_v': ('iterate', None)})

if __name__ == '__main__':
    runs = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    names = list(FUNCTIONS) if sys.argv[1] == 'all' else [sys.argv[1]]
    worst = 0
    for n in names:
        worst = max(worst, compare(n, runs))
    sys.exit(worst)
