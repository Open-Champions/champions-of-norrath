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
        a = self.fv(rt)  # the R5900 sqrt.s takes ft
        self.setf(sa, a ** 0.5 if a >= 0 else float('nan'))
        return None
    return _old_cop1(self, ins, pc, rs, rt, rd, sa, fn, tgt)


Machine.cop1 = _cop1

# --- Machine additions: 64-bit variable shifts -----------------------------------
_old_exe = Machine.exe


def _exe(self, ins, pc):
    if (ins >> 26) == 0 and (ins & 63) in (0x14, 0x16, 0x17):
        fn = ins & 63
        rs, rt, rd = (ins >> 21) & 31, (ins >> 16) & 31, (ins >> 11) & 31
        amount = self.g32(rs) & 63
        if fn == 0x14:
            self.setr(rd, self.r[rt] << amount)
        elif fn == 0x16:
            self.setr(rd, (self.r[rt] & cr.M64) >> amount)
        else:
            self.setr(rd, cr.s64(self.r[rt]) >> amount)
        return None
    return _old_exe(self, ins, pc)


Machine.exe = _exe

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
DATA = {'norrath_actors_b_view_4844a0': 0x4844A0, 'player': 0x408600, 'nmPlayers': 0x484538}


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
            elif kind == 'q':
                m.setr(2, val)
            elif kind == 'f':
                m.f[0] = cr.f2b(float(val))
    return handler


def build_authored(spec, work, stubnames):
    objs = []
    for rel in [spec['src']] + spec.get('extra', []):
        obj = os.path.join(work, os.path.basename(os.path.dirname(rel)) + '-' + os.path.basename(rel).replace('.cpp', '.o'))
        subprocess.check_call([P + 'g++', *FLAGS, '-c', os.path.join(SRC, rel), '-o', obj])
        objs.append(obj)
    und, defined = set(), set()
    for obj in objs:
        for l in subprocess.check_output([P + 'nm', obj], text=True).splitlines():
            parts = l.split()
            if len(parts) == 2 and parts[0] == 'U':
                und.add(parts[1])
            elif len(parts) == 3 and parts[1] != 'U':
                defined.add(parts[2])
    defsyms = {}
    for name in sorted(und - defined):
        if name in DATA:
            defsyms[name] = DATA[name]
        else:
            orig = ALIAS.get(name, name)
            if orig not in stubnames:
                raise SystemExit('unmapped authored import: ' + name)
            defsyms[name] = STUB + 0x10 * stubnames.index(orig)
    out = os.path.join(work, 'linked.elf')
    cmd = [P + 'ld', '-m', 'elf32lr5900', '-Ttext=0x00400000', '--no-warn-mismatch']
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
    elf = build_authored(spec, work, names)
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
    objs = [OBJ, OTHER, OTHER + 0x100]
    vtable(rng, mem, {184: (VSTUB, rng.choice([0, 8, -8, 0x70]))})
    for o in objs:
        put32(mem, o + 12, rng.choice([0, 0x20, 0x20, 0x40, 0x60, 0xFFFFFFFF, 0xFFFFFFDF, 0x2000020]))
        mem[o + 570] = rng.choice([0, 0, 0, 0x10, 0xFF, 0x01, 0x80])
        put32(mem, o + 572, rng.choice([10, 9, 11, 0x1000A, 0, 0xFFFF000A, 0x7FFF000A]))
        put32(mem, o + 72, VT)
    return {'mem': mem, 'args': [], 'compare': whole_object(), 'iterate': objs,
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

reg('sleepMonsters__8creaturev', 'w8-actor-virtuals-c-sleep-monsters/sleep_monsters.cpp', sc_sleep,
    {'objectIterateRunLists__FPFP10GameObject_v': ('iterate', None),
     'listDestroy__FP8ListHead': (['o'], None)})

def sc_auto_passable(rng):
    mem = new_object(rng)
    put32(mem, OBJ + 12, rng.choice([0, 1, 0xFFFFFFFE, 0xFFFFFFFF, 0x100, rng.randrange(1 << 32)]))
    put32(mem, OBJ + 284, rng.choice([0, 1, 2, 3, 0xFFFFFFFE, 0xFFFFFFFF]))
    put32(mem, OBJ + 396, rng.choice([0, 5, 0xFFFFFFFF, 1000]))
    put32(mem, 0x4844A0, rng.choice([0, 0x30050000, 0x12345678]))
    return {'mem': mem, 'args': [OBJ, rng.choice([0, 1, 1, 0x100, -1])],
            'compare': whole_object() + list(range(0x4844A0, 0x4844A4))}


def sc_auto_set_state(rng):
    mem = new_object(rng)
    vtable(rng, mem, {152: (VSTUB, rng.choice([0, 8, -8, 0x70]))})
    put32(mem, OBJ + 288, rng.choice([5, 8, 34, 40, 0, 0xFFFFFFFF, 3]))
    for i in range(0, 42):
        # keep table words inside the compared 0x400 object: 296 + 8*i < 0x400 only for i < 96
        put32(mem, OBJ + 296 + 8 * i, rng.choice([0, 0, 0x31000000 + 16 * i]))
        put32(mem, OBJ + 300 + 8 * i, rng.randrange(1 << 32))
    state = rng.choice([0, 1, 2, 3, 4, 7, 30, 31, 32, 33, 39, -1, -3, 5])
    return {'mem': mem, 'args': [OBJ, state], 'compare': whole_object(),
            'vstubs': {VSTUB: ('virt152', ['o', 'i'], None)}}


def sc_auto_trigger(rng):
    mem = new_object(rng)
    vtable(rng, mem, {136: (VSTUB, rng.choice([0, 8, -8, 0x70]))})
    put32(mem, OBJ + 284, rng.choice([0x40, 0x40, 0x40, 0, 0x3F, 0xFFFFFFFF]))
    put32(mem, OBJ + 288, rng.choice([0, 1, 1, 5]))
    put32(mem, OBJ + 376, rng.choice([0, 1, 1, 1, 2, 0xFFFFFFFF]))
    name = rng.choice([b'gothtrap', b'GothTrap', b'GOTHTRAP', b'gothtrapx', b'fayactuator', b'FayActuator',
                       b'fayactuato', b'', b'zzz'])
    for i, c in enumerate(name + b'\0'):
        mem[OBJ + 76 + i] = c
    return {'mem': mem, 'args': [OBJ, rng.choice([0, 7, -1])], 'compare': whole_object(),
            'vstubs': {VSTUB: ('virt136', ['o'], None)}}


PLAYER = 0x408600
NMPLAYERS = 0x484538
PLAYER_OBJS = [0x30100000 + 0x1000 * k for k in range(4)]


def sc_auto_run(rng):
    mem = new_object(rng)
    put32(mem, OBJ + 280, rng.choice([0, 1, 5]))
    mem[OBJ + 40] = rng.choice([0, 3, 255])
    mem[OBJ + 156] = rng.choice([0, 0, 1, 0xFF, 0x80])
    put32(mem, OBJ + 192, rng.choice([0, 1, 2, 3, 0xFFFFFFFF, 1, 1]))
    put32(mem, OBJ + 292, rng.choice([0, 1, 7, 1, 1]))
    put32(mem, OBJ + 380, rng.choice([0, 5, 0xFFFFFFFF]))
    put32(mem, OBJ + 56, rng.choice([0, 0x1000, 0xFFFF8000, 0x7FFF, 0x4000, 0x12345678]))
    for k, o in enumerate((16, 20, 24)):
        putf(mem, OBJ + o, float(rng.randrange(-30, 30)))
    put32(mem, NMPLAYERS, rng.choice([-1, 0, 1, 2, 3, 4, 4, 5]))
    for k, o in enumerate(PLAYER_OBJS):
        fill(rng, mem, o, 0x400)
        put32(mem, o + 12, rng.choice([0, 8, 0x10, 0xFFFFFFF7, 0xFFFFFFFF]))
        near = rng.random() < 0.7
        for q in (16, 20, 24):
            putf(mem, o + q, float(rng.randrange(-24, 24) if near else rng.randrange(-60, 60)))
        put32(mem, PLAYER + 21872 + 23984 * k, rng.choice([o, o, o, 0]))
    cmp_addrs = whole_object() + list(range(NMPLAYERS, NMPLAYERS + 4))
    for o in PLAYER_OBJS:
        cmp_addrs += list(range(o, o + 0x400))
    return {'mem': mem, 'args': [OBJ], 'compare': cmp_addrs}


def sc_door_state(rng):
    mem = new_object(rng)
    put32(mem, OBJ + 376, rng.choice([0, 1, 2, 3, 0xFFFFFFFF, 5]))
    put32(mem, OBJ + 396, rng.choice([0, 5, 0xFFFFFFFF, 1000]))
    for off in (400, 408):
        putf(mem, OBJ + off, float(rng.randrange(-300, 300)))
    put32(mem, 0x4844A0, rng.choice([0, 0x30050000, 0x12345678]))
    return {'mem': mem, 'args': [OBJ, rng.choice([0, 1, 1, 0x100, -1])],
            'compare': whole_object() + list(range(0x4844A0, 0x4844A4))}


DOOR_STUBS = {'worldSetDoorCollision__FP12_worldHeaderii': (['i', 'i', 'i'], ('i', [0, 1, 99])),
              'worldMoveDoor__FP12_worldHeaderiG6Point3s': (['i', 'i', 's12', 'i'], ('i', [0, 5, 77])),
              'worldSetDoorHeight__FP12_worldHeaderif': (['i', 'i', 'f'], ('i', [0, 3, 0xFFFFFFFF]))}

def sc_container_save(rng):
    mem = new_object(rng)
    fill(rng, mem, EXTRA, 0x200)
    for off in (76, 80, 84, 88, 92):
        put32(mem, OBJ + off, rng.choice([0, 0x31F00000 + off, rng.randrange(1 << 32)]))
    for off in (28, 32):
        putf(mem, OBJ + off, rng.choice([0.0, 1.5, -1.5, 100.25, -70000.75, 40000.0, 32767.5, -32768.5, 65536.0, 123456.0]))
    return {'mem': mem, 'args': [OBJ, EXTRA + rng.choice([0, 1, 2, 3, 4, 8])],
            'compare': whole_object() + list(range(EXTRA, EXTRA + 0x200))}


W = 'w8-actor-virtuals-c-'
reg('saveActivate__9Savepoint', W + 'save-activate/save_activate.cpp', sc_save_activate, {})
reg('setState__10Teleporteri', W + 'teleporter-set-state/set_state.cpp', sc_obj_int, {})
reg('setActiveChunks__10Teleporter', W + 'teleporter-set-active-chunks/set_active_chunks.cpp', sc_noop, {})
reg('msg_run__8GothTrap', W + 'goth-trap-run/run.cpp', sc_noop, {})
reg('msg_run__8HateHole', W + 'hate-hole-run/run.cpp', sc_noop, {'msg_run__8AutoProp': (['o'], None)})
reg('msg_save__7ShooterPc', W + 'shooter-save/save.cpp', sc_save_buffer, {}, returns=True)
reg('msg_save__7TriggerPc', W + 'trigger-save/save.cpp', sc_save_buffer, {}, returns=True)
reg('msg_save__8AutoPropPc', W + 'auto-prop-save/save.cpp', sc_save_buffer, {}, returns=True)
reg('msg_trigger__11MissileTrapi', W + 'missile-trap-trigger/trigger.cpp', sc_obj_int,
    {'objectAddToRunList__FP10GameObject': (['o'], ('i', [0, 1, 0x30001234, 0xFFFFFFFF])),
     'objectRemoveFromRunList__FP10GameObject': (['o'], ('i', [0, 7, 0x40000000, 0xFFFFFFFF]))}, returns=True)
reg('setPassableState__8HackDoorb', W + 'hack-door-passable/set_passable_state.cpp', sc_obj_bool,
    {'setPassableState__8AutoPropb': (['o', 'i'], None)})

reg('setPassableState__8AutoPropb', W + 'auto-prop-passable/set_passable_state.cpp', sc_auto_passable,
    {'worldSetDoorCollision__FP12_worldHeaderii': (['i', 'i', 'i'], ('i', [0, 1, 99])),
     'worldMoveDoor__FP12_worldHeaderiG6Point3s': (['i', 'i', 's12', 'i'], ('i', [0, 5, 77]))})

reg('setState__8AutoPropi', W + 'auto-prop-set-state/set_state.cpp', sc_auto_set_state,
    {'modelGetActiveChunks__FP12_modelHeaderi': (['i', 'i'], ('q', [0, 1, 0x8877665544332211, 0xFFFFFFFFFFFFFFFF, 0x80000000])),
     'animAdd__FP14AnimationStateP15AnimationHeaderi': (['o', 'i', 'i'], None),
     'animSetToEnd__FP14AnimationState': (['o'], None)})

reg('msg_trigger__8AutoPropi', W + 'auto-prop-trigger/trigger.cpp', sc_auto_trigger,
    {'strcasecmp': (['c', 'c'], ('i', [0, 0, 1, -1, 32])),
     'lumpFindResource__FPcT0': (['c', 'c'], ('i', [0x31F00000, 0x31F00010])),
     'soundPlaySequence__FP15AnimationHeaderP10GameObjectf': (['i', 'o', 'f'], None)})

reg('msg_run__8AutoProp', W + 'auto-prop-run/run.cpp', sc_auto_run,
    {'objectActiveProcessing__FP10GameObjectii': (['o', 'i', 'i'], ('i', [0, 1, 1, 1, 1, 1, 1, 1, 5, 0xFFFFFFFF])),
     'animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps': (['o', 'o', 'i', 'i'], ('i', [0, 1, 2, 3, 5, 0xFFFF])),
     'strticmp__FPCcT0': (['c', 'c'], ('i', [0, 0, 0, 0, 0, 0, 0, 1, 0xFFFFFFFF])),
     'icos__Fi': (['i'], ('f', [0, 0.25, -0.25, 0.5, -0.5, 1, -1])),
     'isin__Fi': (['i'], ('f', [0, 0.25, -0.25, 0.5, -0.5, 1, -1])),
     'gameDifficultyScale__Fv': ([], ('f', [1, 2, 0.5])),
     'iatan2__Fff': (['f', 'f'], ('i', [0, 0x4000, 0x8000, 0xC000, 0xFFFF, 0x12345, 0xFFFF8000, 0x7FFF, 5])),
     'gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb':
         (['o', 'o', 'f', 'f', 'i', 'i', 'i', 'i', 'i', 'i'], None)})

reg('setPassableState__8GothTrapb', W + 'goth-trap-passable/set_passable_state.cpp', sc_door_state, DOOR_STUBS,
    extra=['vehicle-actors-goth-trap-passable/set_passable_state.cpp'])
reg('setPassableState__10HateBridgeb', W + 'hate-bridge-passable/set_passable_state.cpp', sc_door_state, DOOR_STUBS,
    extra=['vehicle-actors-hate-bridge-passable/set_passable_state.cpp'])

reg('setPassableState__8HateHoleb', W + 'hate-hole-passable/set_passable_state.cpp', sc_door_state, DOOR_STUBS)

reg('msg_save__9ContainerPc', W + 'container-save/save.cpp', sc_container_save,
    {'objectSaveAddToStringPool__FPCc': (['i'], ('i', [0, 1, 0x1234, 0xFFFFFFFF, 77, 78]))}, returns=True)

if __name__ == '__main__':
    runs = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    names = list(FUNCTIONS) if sys.argv[1] == 'all' else [sys.argv[1]]
    worst = 0
    for n in names:
        worst = max(worst, compare(n, runs))
    sys.exit(worst)
