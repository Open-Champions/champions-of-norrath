#!/usr/bin/env python3
"""Differential check of the w8-fx-particles bodies against the private executable.

Each function is run twice in the small R5900 interpreter of
docs/agents/combat-reactions-compare.py: once from the private SLUS_205.65 image
and once from the authored source, compiled and linked with the reviewed EE
toolchain at a scratch address. Both runs get the same randomized scenario
memory and the same scripted callees. Every imported function is replaced by a
logging stub at its original address; imported data keeps its original
address. The runs must agree on every stub call (name, argument registers,
float registers, stack arguments), on the return registers, and on every byte
of the scenario memory afterwards, stack scratch excluded.

  NORRATH_EXE=/path/SLUS_205.65 NORRATH_EE=/path/to/ee \\
      python3 docs/agents/w8-fx-particles-compare.py all 500

The executable and toolchain stay outside the repository. Nothing from the
executable is stored; scratch files go to a fresh temporary directory. Opcodes
the interpreter lacks stop the run with the instruction address.
"""
import importlib.util
import os
import random
import struct
import subprocess
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))
_spec = importlib.util.spec_from_file_location('cr', os.path.join(_HERE, 'combat-reactions-compare.py'))
cr = importlib.util.module_from_spec(_spec)
sys.modules['cr'] = cr
_spec.loader.exec_module(cr)

M32 = cr.M32
P = cr.P
STACK = cr.STACK
GP = cr.GP
EXE_IMAGE = cr.Image(cr.EXE)
AUTH_BASE = 0x00400000

# Region used for pointer-valued scenario data; far from the image.
ARENA = 0x30000000


class Composite:
    """Authored segments first, then the original image for original data."""

    def __init__(self, auth):
        self.auth = auth

    def byte(self, addr):
        for vaddr, blob in self.auth.segs:
            if vaddr <= addr < vaddr + len(blob):
                return blob[addr - vaddr]
        return EXE_IMAGE.byte(addr)


class Mach(cr.Machine):
    """Adds the single-precision square-root family and a few compares."""

    def cop1(self, ins, pc, rs, rt, rd, sa, fn, tgt):
        if rs == 0x10 and fn in (4, 0x16, 0x28, 0x2a, 0x2b, 0x2c, 0x2e, 0x31, 0x33, 0x35, 0x37):
            a, b = self.fv(rd), self.fv(rt)
            fd = sa
            if fn == 4:
                self.setf(fd, a ** 0.5 if a > 0 else 0.0)
            elif fn == 0x16:
                self.setf(fd, a / (b ** 0.5) if b > 0 else float('inf'))
            elif fn == 0x31:
                self.cond = False
            elif fn == 0x33:
                self.cond = a == b
            elif fn == 0x35:
                self.cond = a < b
            elif fn == 0x37:
                self.cond = a <= b
            else:
                raise cr.Stop('COP1.S %#x at %#x' % (fn, pc))
            return None
        return super().cop1(ins, pc, rs, rt, rd, sa, fn, tgt)


_nm_cache = {}
# Names the symbol table repeats (local statics in other units): the object this
# package means.
OVERRIDES = {'trail': 0x780B00, 'block': 0x700080, 'nodes': 0x7AD000}


def exe_symbols():
    if not _nm_cache:
        for line in subprocess.check_output([P + 'nm', cr.EXE], text=True).splitlines():
            parts = line.split()
            if len(parts) == 3:
                _nm_cache.setdefault(parts[2], (int(parts[0], 16), parts[1]))
        for name, addr in OVERRIDES.items():
            _nm_cache[name] = (addr, 'b')
    return _nm_cache


def build_authored(rel, work, extra_flags=()):
    base = os.path.basename(rel).replace('.cpp', '')
    obj = os.path.join(work, base + '.o')
    subprocess.check_call([P + 'g++', *cr.FLAGS, *extra_flags, '-c', os.path.join(cr.SRC, rel), '-o', obj])
    syms = exe_symbols()
    defsyms = {}
    funcs = {}
    for line in subprocess.check_output([P + 'nm', '-u', obj], text=True).splitlines():
        name = line.split()[-1]
        target = ALIASES.get(name, name)
        if target not in syms:
            raise SystemExit('authored import with no original address: ' + name)
        defsyms[name] = syms[target][0]
        funcs[name] = syms[target][1] in 'Tt'
    out = os.path.join(work, base + '.elf')
    cmd = [P + 'ld', '-m', 'elf32lr5900', '-Ttext=%#x' % AUTH_BASE, '--no-warn-mismatch']
    cmd += ['--defsym=%s=%#x' % kv for kv in defsyms.items()]
    cmd += ['-o', out, obj]
    subprocess.check_call(cmd)
    return out, {n: defsyms[n] for n, is_fn in funcs.items() if is_fn}


def object_names(elf):
    """Sorted (start, size, name) of data objects in the executable plus, when given, a linked authored image.

    Authored file-local statics carry a C++ mangling prefix, which is stripped."""
    import re
    items = []
    for image in (cr.EXE, elf):
        if not image:
            continue
        for line in subprocess.check_output([P + 'nm', '-S', image], text=True).splitlines():
            parts = line.split()
            if len(parts) == 4 and parts[2] in 'bBdD':
                name = re.sub(r'^_ZL\d+', '', parts[3])
                items.append((int(parts[0], 16), int(parts[1], 16), name))
            elif len(parts) == 3 and parts[1] in 'bBdD':
                items.append((int(parts[0], 16), 0, re.sub(r'^_ZL\d+', '', parts[2])))
    items.sort()
    return items


def symbol_address(elf, name):
    for line in subprocess.check_output([P + 'nm', elf], text=True).splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[2] == name:
            return int(parts[0], 16)
    raise SystemExit('missing symbol ' + name)


# Authored names that stand for an original function under another symbol.
ALIASES = {
    'norrath_game_object_construct': '__10GameObjectG6Point3PQ210GameObject11resourceMapPPc',
    'norrath_actor_add_run_list': 'objectAddToRunList__FP10GameObject',
    'norrath_light_add': 'lightAdd__FP5Light',
    'norrath_list_oneway_link': 'listOnewayLink__FP8ListHeadT0Pv',
}


def resolve(name):
    return exe_symbols()[ALIASES.get(name, name)]


def make_stub(name, sig, rng, log, answers, effects=None, namer=None):
    """Logging stub. sig = (int_args, float_args, stack_words[, peeks]).

    peeks is a tuple of (argument register index, byte count): the bytes the
    register points at are logged too."""
    n_int, n_float, n_stack = sig[:3]
    peeks = sig[3] if len(sig) > 3 else ()
    masked = sig[4] if len(sig) > 4 else ()     # int argument indexes not logged raw (address differs)

    def handler(m):
        def norm(v):
            if namer is not None:
                import bisect
                k = bisect.bisect_right(namer, (v, 1 << 40, '')) - 1
                if k >= 0:
                    start, size, nm = namer[k]
                    if v == start:
                        return nm
                    if v < start + size:
                        return '%s+%d' % (nm, v - start)
            return 'stack' if stack_region(v) else v
        ints = [('masked' if i in masked else norm(m.g32(4 + i))) for i in range(min(n_int, 8))]
        floats = [m.f[12 + i] for i in range(n_float)]
        sp = m.g32(29)
        stack = [norm(m.u32(sp + 8 * i)) for i in range(n_stack)]
        seen = tuple(bytes(m.u8(m.g32(4 + r) + k) for k in range(n)) for r, n in peeks)
        log.append((name, tuple(ints), tuple(floats), tuple(stack), seen))
        if effects and name in effects:
            effects[name](m, rng)
        ret = answers.get(name)
        value = ret(rng) if callable(ret) else rng.randrange(1 << 32)
        m.setr32(2, value)
        m.f[0] = rng.randrange(1 << 32) if not isinstance(ret, tuple) else ret[0]
    return handler


def stack_region(addr):
    return STACK - 0x10000 <= addr < STACK + 0x1000


def run_image(image, entry, func_addrs, sigs, answers, scenario, seed, auth, effects=None, namer=None):
    mem = dict(scenario['mem'])
    log = []
    rng = random.Random(seed ^ 0xface)
    stubs = {}
    m = Mach(Composite(image) if auth else image, mem, stubs, GP)
    for name, addr in func_addrs.items():
        stubs[addr] = make_stub(name, sigs[name], rng, log, answers, effects, namer)
    m.r[29] = STACK
    for i, v in enumerate(scenario.get('args', [])):
        m.r[4 + i] = v & M32
    for i, v in scenario.get('fargs', {}).items():
        m.f[i] = v
    for off, v in scenario.get('stack', {}).items():
        m.w32(STACK + off, v)
    m.run(entry, steps=scenario.get('steps', 400000))
    state = {a: v for a, v in m.mem.items() if not stack_region(a)}
    return log, state, m.g32(2), m.f[0]


def compare(name, runs):
    spec = FUNCTIONS[name]
    work = os.path.join(cr.SCRATCH, name)
    os.makedirs(work, exist_ok=True)
    elf, funcs = build_authored(spec['src'], work, spec.get('flags', ()))
    auth = cr.Image(elf)
    entry = symbol_address(elf, spec['symbol'])
    sigs = spec['stubs']
    # Every imported function must have a signature; an unknown one is an error.
    for fname in funcs:
        if fname not in sigs:
            raise SystemExit('no stub signature for import ' + fname)
    # The original may also call functions the authored source inlines or does
    # not import; stub the ones named in the spec on both sides.
    orig_funcs = {n: resolve(n)[0] for n in sigs if n not in spec.get('callbacks', {}).values()}
    auth_funcs = {n: a for n, a in orig_funcs.items() if n in funcs}
    # Callback targets the scenario plants in memory: stubbed at fixed addresses on both sides.
    for addr, cb in spec.get('callbacks', {}).items():
        orig_funcs[cb] = addr
        auth_funcs[cb] = addr
    orig_names = auth_names = object_names(elf) if spec.get('name_objects') else None
    bad = 0
    seen = {}
    for seed in range(runs):
        sc = spec['scenario'](random.Random(seed), seed)
        res = []
        skipped = False
        for image, e, fa, is_auth in ((EXE_IMAGE, spec['orig'], orig_funcs, False), (auth, entry, auth_funcs, True)):
            try:
                if is_auth:
                    sc = dict(sc, steps=sc.get('steps', 400000) * 6)
                res.append(run_image(image, e, fa, sigs, spec.get('answers', {}), sc, seed, is_auth, spec.get('effects'),
                                     auth_names if is_auth else orig_names))
            except cr.Stop as exc:
                if not is_auth and 'step limit' in str(exc):
                    skipped = True      # the original itself runs too long on this scenario
                    break
                print('seed %d: stopped (%s): %s' % (seed, 'authored' if is_auth else 'original', exc))
                return 2
        if skipped:
            seen['skipped_long_scenarios'] = seen.get('skipped_long_scenarios', 0) + 1
            continue
        for ent in res[0][0]:
            seen[ent[0]] = seen.get(ent[0], 0) + 1
        a, b = res
        ok = a[0] == b[0] and a[1] == b[1]
        if spec.get('returns'):
            ok = ok and a[2] == b[2]
        if spec.get('freturns'):
            ok = ok and a[3] == b[3]
        if not ok:
            bad += 1
            if bad <= 3:
                print('MISMATCH seed %d' % seed)
                n = min(len(a[0]), len(b[0]))
                k = next((i for i in range(n) if a[0][i] != b[0][i]), n)
                if a[0] != b[0]:
                    print('  log differs at %d:\n   orig %s\n   auth %s' % (k, a[0][k:k + 2], b[0][k:k + 2]))
                diff = sorted(x for x in set(a[1]) | set(b[1]) if a[1].get(x) != b[1].get(x))[:10]
                if diff:
                    print('  memory differs at', [hex(x) for x in diff])
                if spec.get('returns') and a[2] != b[2]:
                    print('  v0', a[2], b[2])
                if spec.get('freturns') and a[3] != b[3]:
                    print('  f0', a[3], b[3])
    print('%s: %d runs, %d mismatches; original stub calls %s' % (name, runs, bad, seen))
    return 1 if bad else 0


# ---- scenario helpers ----

def put32(mem, a, v):
    for i in range(4):
        mem[a + i] = (v >> (8 * i)) & 0xFF


def get32(mem, a):
    return sum(mem[a + i] << (8 * i) for i in range(4))


def fill(rng, mem, base, size):
    for i in range(size):
        mem[base + i] = rng.randrange(256)


def f2b(x):
    return struct.unpack('<I', struct.pack('<f', x))[0]


FUNCTIONS = {}


def reg(symbol, orig, src, scenario, stubs=None, **kw):
    FUNCTIONS[symbol] = dict(symbol=symbol, orig=orig, src=src, scenario=scenario, stubs=stubs or {}, **kw)


# ---- P_DeleteParticleSystem__Fi ----
PS = 0x6F5AD0
FREE_BLOCKS = 0x4869F0


def scenario_delete_ps(rng, seed):
    mem = {}
    fill(rng, mem, PS, 16 * 312)
    node = ARENA
    index = rng.randrange(16) if rng.random() < 0.9 else rng.randrange(16, 40)
    high = rng.choice([0, 0, 0x100, 0x2300])
    ident = index | high
    if rng.random() < 0.1:
        ident = 0
    rec = PS + (ident & 0xFF) * 312
    # Seed the record with 16 list entries, each an empty or a chain.
    for entry in range(16):
        base = rec + entry * 16
        count = rng.choice([0, 0, 1, 2, 3, 5])
        if count:
            nodes = [node + 64 * k for k in range(count)]
            node += 64 * count
            for k, n in enumerate(nodes):
                put32(mem, n, nodes[k + 1] if k + 1 < count else rng.randrange(1 << 32))
                put32(mem, n + 4, rng.randrange(1 << 32))
            put32(mem, base, nodes[0])
            put32(mem, base + 4, nodes[-1])
        else:
            put32(mem, base, 0)
            put32(mem, base + 4, rng.randrange(1 << 32))
    owner = ident if rng.random() < 0.8 else rng.randrange(1 << 32)
    put32(mem, rec + 304, owner)
    put32(mem, FREE_BLOCKS, node + 0x10000 if rng.random() < 0.7 else 0)
    return {'mem': mem, 'args': [ident]}


reg('P_DeleteParticleSystem__Fi', 0x180468, 'w8-fx-particles-delete-particle-system/delete_particle_system.cpp',
    scenario_delete_ps)

# ---- lightPushSceneLights__FG6Point3iiiiiiiii ----
SCENE = 0x4F4060
SAVED_SCENE = 0x4F4098
INTERFACE_MODE = 0x483AAC


def scenario_push_scene_lights(rng, seed):
    mem = {}
    fill(rng, mem, SCENE, 56)
    fill(rng, mem, SAVED_SCENE, 56)
    put32(mem, INTERFACE_MODE, rng.choice([0, 0, 0, 1, 5, 0xFFFFFFFF, 0xFFFFFFFE, 10]))
    point = ARENA
    for k in range(3):
        put32(mem, point + 4 * k, f2b(float(rng.choice([-3, -1, 0, 1, 2, 4, 7, 12])) + rng.choice([0.0, 0.5])))
    if all(get32(mem, point + 4 * k) in (0, 0x80000000) for k in range(3)):
        put32(mem, point, f2b(1.0))
    ints = [rng.randrange(1 << 32) if rng.random() < 0.5 else rng.randrange(300) for _ in range(9)]
    return {'mem': mem, 'args': [point] + ints[:7], 'stack': {0: ints[7], 8: ints[8]}}


reg('lightPushSceneLights__FG6Point3iiiiiiiii', 0x13CCC8,
    'w8-fx-particles-light-push-scene-lights/light_push_scene_lights.cpp', scenario_push_scene_lights,
    stubs={'transformSceneLightTask__Fv': (0, 0, 0), 'lightResetOptionalParams__Fv': (0, 0, 0)})

# ---- __eq__19ParticleEmitterDataRC19ParticleEmitterData ----
def scenario_emitter_eq(rng, seed):
    mem = {}
    left, right = ARENA, ARENA + 0x100
    for k in range(20):
        v = rng.randrange(256)
        mem[left + k] = v
        mem[right + k] = v
    for _ in range(rng.choice([0, 0, 1, 1, 2])):
        mem[right + rng.randrange(20)] ^= 1 << rng.randrange(8)
    return {'mem': mem, 'args': [left, right]}


reg('__eq__19ParticleEmitterDataRC19ParticleEmitterData', 0x186A70,
    'w8-fx-particles-emitter-data-eq/emitter_data_eq.cpp', scenario_emitter_eq, returns=True)


# ---- __19ParticleMorphEffectP8CreatureT1 ----
FRAME_NM = 0x4844A4


def scenario_morph_ctor(rng, seed):
    mem = {}
    fill(rng, mem, ARENA, 0x200)
    put32(mem, FRAME_NM, rng.randrange(1 << 32))
    return {'mem': mem, 'args': [ARENA, rng.randrange(1 << 32), rng.randrange(1 << 32)]}


reg('__19ParticleMorphEffectP8CreatureT1', 0x155F08, 'w8-fx-particles-morph-effect-ctor/morph_effect_ctor.cpp',
    scenario_morph_ctor, returns=True,
    stubs={'norrath_game_object_construct': (4, 0, 0, ((1, 12),)), 'norrath_actor_add_run_list': (1, 0, 0)})

# ---- updateTrails__Fv ----
TRAIL = 0x780B00


def scenario_update_trails(rng, seed):
    mem = {}
    for n in range(64):
        rec = TRAIL + n * 1040
        for k in range(1040):
            mem[rec + k] = rng.randrange(256)
        for e in range(128):
            stamp = rng.randrange(0, 60) if rng.random() < 0.9 else rng.randrange(0, 0x10000)
            mem[rec + 6 + e * 8] = stamp & 0xFF
            mem[rec + 6 + e * 8 + 1] = stamp >> 8
        count = rng.choice([0, 1, 5, 0x100, 0xFFFF])
        mem[rec + 1024] = count & 0xFF
        mem[rec + 1025] = count >> 8
        age = rng.choice([0, 3, 20, 50, 70, 0x7FFF, 0xFFFF, 0x8000])
        mem[rec + 1026] = age & 0xFF
        mem[rec + 1027] = age >> 8
        flags = rng.choice([0, 0, 2, 2, 0x10, 0x12, 1, 3, 0x13, rng.randrange(256)])
        start = rng.choice([rng.randrange(128), rng.randrange(128), 126, 127, rng.randrange(256)])
        end = rng.choice([rng.randrange(128), rng.randrange(128), 0, 3, start, (start + 4) & 0x7F])
        if flags & 2 and end < 128:
            end = (end & ~1) | (start & 1)     # a step of 2 keeps the parity of start
        life = rng.choice([0, 1, 5, 10, 30, 0xFFFB, 0x7FFF])
        if start >= 128 or end >= 128:
            life = 0x7FFF                       # the scan stops at the first entry
        mem[rec + 1028] = start
        mem[rec + 1029] = end
        mem[rec + 1031] = flags
        mem[rec + 1032] = life & 0xFF
        mem[rec + 1033] = life >> 8
    return {'mem': mem, 'args': [], 'steps': 3000000}


reg('updateTrails__Fv', 0x1840C0, 'w8-fx-particles-update-trails/update_trails.cpp', scenario_update_trails)

# ---- updatePS__FP14ParticleSystem ----
BLOCK_LIST = 0x780080
BLOCK = 0x700080
DESC_LIST = 0x780300
DESC_MARK = 0x780280
P_FRAME_NUM = 0x4869F4
CB_A, CB_B = 0x00600010, 0x00600020


def scenario_update_ps(rng, seed):
    mem = {}
    system = ARENA + 0x100000
    frame = rng.choice([0, 5, 40, 100, 200, 0x7FF0, 0xFFF0])
    mem[P_FRAME_NUM] = frame & 0xFF
    mem[P_FRAME_NUM + 1] = frame >> 8
    life = [11, 30, 60, 90]
    nodes = rng.sample(range(0, 128), 40)
    for k in range(128):
        put32(mem, BLOCK_LIST + 4 * k, 0)
    # Descriptors.
    descs = []
    for d in range(6):
        rec = ARENA + 0x200000 + 0x100 * d
        put32(mem, rec + 4, d)
        put32(mem, rec + 8, rng.choice([1, 1, 2, 3, 5]))
        put32(mem, rec + 12, rng.choice([0, CB_A, CB_B]))
        put32(mem, DESC_LIST + 64 * d + 24, rec)
        put32(mem, DESC_MARK + 4 * d, rng.randrange(1 << 32))
        descs.append(rec)
    used = 0
    for g in range(4):
        for e in range(4):
            base = system + g * 64 + e * 16
            if rng.random() < 0.3:
                for k in range(4):
                    put32(mem, base + 4 * k, rng.randrange(1 << 32) if k else 0)
                continue
            count = rng.choice([1, 1, 1, 2])
            chain = [nodes[used + i] for i in range(count)]
            used += count
            addrs = [BLOCK_LIST + 4 * n for n in chain]
            for i, n in enumerate(chain):
                put32(mem, addrs[i], addrs[i + 1] if i + 1 < count else rng.choice([0, rng.randrange(1 << 32)]))
                page = BLOCK + 4096 * n
                for slot in range(256):
                    p = page + 16 * slot
                    for k in range(16):
                        mem[p + k] = rng.randrange(256)
                    stamp = (frame - life[e] + rng.randrange(-4, 5)) if rng.random() < 0.8 else rng.randrange(0x10000)
                    # younger toward the end of a chain, as the scan expects
                    stamp = stamp + (3 if rng.random() < 0.2 else 0)
                    mem[p + 6] = stamp & 0xFF
                    mem[p + 7] = (stamp >> 8) & 0xFF
                    h14 = (rng.randrange(0, 6) << 2) | rng.randrange(4)
                    mem[p + 14] = h14
                    mem[p + 15] = 0
            start = rng.choice([1, 1, 2, 30, 31, 32, 33, 62, 100, 254, 255])
            if count == 1:
                end_slot = rng.choice([start, start + 1, start + 5, 200, 255, 257, 1])
            else:
                end_slot = rng.choice([1, 3, 255, 257, 40])
            put32(mem, base, addrs[0])
            put32(mem, base + 4, addrs[-1])
            put32(mem, base + 8, start)
            put32(mem, base + 12, end_slot)
    put32(mem, FREE_BLOCKS, rng.choice([0, BLOCK_LIST + 4 * 100]))
    return {'mem': mem, 'args': [system], 'steps': 1500000}


reg('updatePS__FP14ParticleSystem', 0x181080, 'w8-fx-particles-update-ps/update_ps.cpp', scenario_update_ps,
    returns=True, stubs={'callback_a': (1, 0, 0, ((0, 16),)), 'callback_b': (1, 0, 0, ((0, 16),))},
    callbacks={CB_A: 'callback_a', CB_B: 'callback_b'})

# ---- setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v ----
def rand_float_bits(rng):
    return f2b(rng.choice([0.0, 1.0, 2.5, 0.125, 10.0, -3.0, 7.0, 0.5, 100.0]))


def scenario_set_particle_def(rng, seed):
    mem = {}
    fill(rng, mem, ARENA, 0x100)
    ints = [rng.choice([0, 1, 5, 100, 255, -7, 4096, 90000]) & M32 for _ in range(8)]
    mode = rng.choice([-2, -1, 0, 1, 2, 3, 4, 9])
    tier = rng.choice([-2, 0, 1, 2, 2, 3, 3, 4, 9])
    grid = rng.choice([0, 1, 0x11, 0x2F, 0x55, 0xFF, -16 & M32, -1 & M32, 0x123])
    callback = rng.choice([0, 0, 0, 0x1234])
    return {'mem': mem, 'args': [ARENA] + ints[:7],
            'fargs': {12: rand_float_bits(rng), 13: rand_float_bits(rng), 14: rand_float_bits(rng)},
            'stack': {0: ints[7], 8: mode & M32, 16: tier & M32, 24: grid, 32: callback}}


reg('setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v', 0x180820,
    'w8-fx-particles-set-particle-def/set_particle_def.cpp', scenario_set_particle_def,
    stubs={'__assert__FPCcUiT0i': (4, 0, 0, ((0, 12), (2, 21)), (0, 2))})

reg('__11ParticleDefiiiiiiiifffiiiPFPv_v', 0x180D78, 'w8-fx-particles-particle-def-ctor/particle_def_ctor.cpp',
    scenario_set_particle_def,
    stubs={'setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v': (8, 3, 5)})

# ---- __11AddParticleG6Point3PPc ----
def effect_light_init(m, rng):
    # lightInit decides whether there is a light: word +84 and the light inputs.
    obj = m.g32(4)
    m.w32(obj + 84, rng.choice([0, 0, 1, 0x1234]))
    m.w32(obj + 324, rng.choice([0, 1, 5, 100, 0xFFFFFFFB]))
    for off in (340, 344, 348, 352):
        m.w32(obj + off, rng.randrange(1 << 32))


def scenario_add_particle(rng, seed):
    mem = {}
    fill(rng, mem, ARENA, 0x180)
    point = ARENA + 0x1000
    for k in range(3):
        put32(mem, point + 4 * k, f2b(float(rng.randrange(-500, 500))))
    return {'mem': mem, 'args': [ARENA, point, ARENA + 0x2000]}


reg('__11AddParticleG6Point3PPc', 0x222A40, 'w8-fx-particles-add-particle-ctor/add_particle_ctor.cpp',
    scenario_add_particle, effects={'lightInit__11AddParticlePPc': effect_light_init},
    stubs={'norrath_game_object_construct': (4, 0, 0, ((1, 12),)), '__11ParticleDef': (1, 0, 0),
           'particleInit__11AddParticlePPc': (2, 0, 0), 'lightInit__11AddParticlePPc': (2, 0, 0),
           'norrath_light_add': (1, 0, 0)})

# ---- __11LightEffectG6Point3ffffffffffffP10GameObjecti ----
def scenario_light_effect(rng, seed):
    mem = {}
    fill(rng, mem, ARENA, 0x200)
    point = ARENA + 0x1000
    for k in range(3):
        put32(mem, point + 4 * k, f2b(float(rng.randrange(-500, 500))))
    put32(mem, FRAME_NM, rng.randrange(1 << 32))
    vals = [rng.choice([0.0, 1.0, -1.5, 100.7, -250.2, 3.0, 40000.0, 12.25]) for _ in range(12)]
    vals[8] = rng.choice([1.0, 2.0, 0.5, 30.0, -4.0, 300.0])           # radius
    vals[11] = rng.choice([0.0, 0.01, 0.5, 3.0, -1.0, 1.0])            # seconds
    bits = [f2b(v) for v in vals]
    return {'mem': mem, 'args': [ARENA, point, rng.choice([0, ARENA + 0x3000]), rng.randrange(1 << 32)],
            'fargs': {12 + i: bits[i] for i in range(8)},
            'stack': {0: bits[8], 8: bits[9], 16: bits[10], 24: bits[11]}}


reg('__11LightEffectG6Point3ffffffffffffP10GameObjecti', 0x1E5278,
    'w8-fx-particles-light-effect-ctor/light_effect_ctor.cpp', scenario_light_effect,
    stubs={'norrath_game_object_construct': (4, 0, 0, ((1, 12),)), 'norrath_light_add': (1, 0, 0),
           'norrath_list_oneway_link': (3, 0, 0), 'norrath_actor_add_run_list': (1, 0, 0)})

# ---- static initializers: _GLOBAL_$I$ and _GLOBAL_$D$ wrappers ----
# (translation unit, original $I$ address, original $D$ address or None, source directory, arrays the
# initializer or destructor touches as (address, bytes))
INIT_UNITS = [
    ('fxResourceMap', 0x1E6B10, None, 'fx', []),
    ('cloudgiantResourceMap', 0x1E9BA0, None, 'cloudgiant', []),
    ('lavaMonsterResourceMap', 0x1EE370, None, 'lava-monster', []),
    ('soulResourceMap', 0x1F5030, None, 'soul', []),
    ('innoruukResourceMap', 0x1FF800, None, 'innoruuk', []),
    ('cyclopsResourceMap', 0x203CD8, None, 'cyclops', []),
    ('wraithResourceMap', 0x205328, 0x205348, 'wraith', [(0x42BD18, 240)]),
    ('antResourceMap', 0x208D60, None, 'ant', []),
    ('spiderResourceMap', 0x20FF28, None, 'spider', []),
    ('fireBeetleResourceMap', 0x2126A0, None, 'fire-beetle', []),
    ('hasLightOrParticleNumber__FPPci', 0x2256E8, None, 'has-light-or-particle-number', []),
    ('orc1ResourceMap', 0x22BAD0, 0x22BAF0, 'orc1', [(0x42CE10, 80)]),
    ('goblinResourceMap', 0x22F570, None, 'goblin', []),
    ('mummyResourceMap', 0x23C310, None, 'mummy', []),
    ('udkResourceMap', 0x23FA58, None, 'udk', []),
    ('vampireResourceMap', 0x243848, None, 'vampire', []),
    ('wesResourceMap', 0x249F70, 0x249F90, 'wes', [(0x42DD90, 40)]),
    ('scriptParticles', 0x13C508, None, 'script-particles', []),
]
INIT_STUBS = {
    '__11ParticleDefiiiiiiiifffiiiPFPv_v': (8, 3, 5),
    'listDestroy__FP8ListHead': (1, 0, 0),
    '__11ResourceMapPcT1': (3, 0, 0, ((1, 16), (2, 16)), (1, 2)),
}


def make_init_scenario(arrays):
    def scenario(rng, seed):
        mem = {}
        for addr, size in arrays:
            fill(rng, mem, addr, size)
        return {'mem': mem, 'args': []}
    return scenario


for _tu, _i, _d, _dir, _arrays in INIT_UNITS:
    reg('_GLOBAL_$I$' + _tu, _i, 'w8-fx-particles-init-%s/static_init.cpp' % _dir, make_init_scenario(_arrays),
        stubs=INIT_STUBS, name_objects=True)
    if _d:
        reg('_GLOBAL_$D$' + _tu, _d, 'w8-fx-particles-init-%s/static_init.cpp' % _dir, make_init_scenario(_arrays),
            stubs=INIT_STUBS, name_objects=True)


if __name__ == '__main__':
    runs = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    names = list(FUNCTIONS) if sys.argv[1] == 'all' else [sys.argv[1]]
    worst = 0
    for n in names:
        worst = max(worst, compare(n, runs))
    sys.exit(worst)
