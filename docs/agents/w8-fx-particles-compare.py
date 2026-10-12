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


def exe_symbols():
    if not _nm_cache:
        for line in subprocess.check_output([P + 'nm', cr.EXE], text=True).splitlines():
            parts = line.split()
            if len(parts) == 3:
                _nm_cache.setdefault(parts[2], (int(parts[0], 16), parts[1]))
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
        if name not in syms:
            raise SystemExit('authored import with no original address: ' + name)
        defsyms[name] = syms[name][0]
        funcs[name] = syms[name][1] in 'Tt'
    out = os.path.join(work, base + '.elf')
    cmd = [P + 'ld', '-m', 'elf32lr5900', '-Ttext=%#x' % AUTH_BASE, '--no-warn-mismatch']
    cmd += ['--defsym=%s=%#x' % kv for kv in defsyms.items()]
    cmd += ['-o', out, obj]
    subprocess.check_call(cmd)
    return out, {n: syms[n][0] for n, is_fn in funcs.items() if is_fn}


def symbol_address(elf, name):
    for line in subprocess.check_output([P + 'nm', elf], text=True).splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[2] == name:
            return int(parts[0], 16)
    raise SystemExit('missing symbol ' + name)


def make_stub(name, sig, rng, log, answers):
    """Logging stub. sig = (int_args, float_args, stack_words)."""
    n_int, n_float, n_stack = sig

    def handler(m):
        ints = [m.g32(4 + i) for i in range(min(n_int, 8))]
        floats = [m.f[12 + i] for i in range(n_float)]
        sp = m.g32(29)
        stack = [m.u32(sp + 8 * i) for i in range(n_stack)]
        log.append((name, tuple(ints), tuple(floats), tuple(stack)))
        ret = answers.get(name)
        value = ret(rng) if callable(ret) else rng.randrange(1 << 32)
        m.setr32(2, value)
        m.f[0] = rng.randrange(1 << 32) if not isinstance(ret, tuple) else ret[0]
    return handler


def stack_region(addr):
    return STACK - 0x10000 <= addr < STACK + 0x1000


def run_image(image, entry, func_addrs, sigs, answers, scenario, seed, auth):
    mem = dict(scenario['mem'])
    log = []
    rng = random.Random(seed ^ 0xface)
    stubs = {}
    m = Mach(Composite(image) if auth else image, mem, stubs, GP)
    for name, addr in func_addrs.items():
        stubs[addr] = make_stub(name, sigs[name], rng, log, answers)
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
    orig_funcs = {n: exe_symbols()[n][0] for n in sigs}
    auth_funcs = {n: a for n, a in orig_funcs.items() if n in funcs}
    bad = 0
    seen = {}
    for seed in range(runs):
        sc = spec['scenario'](random.Random(seed), seed)
        res = []
        for image, e, fa, is_auth in ((EXE_IMAGE, spec['orig'], orig_funcs, False), (auth, entry, auth_funcs, True)):
            try:
                res.append(run_image(image, e, fa, sigs, spec.get('answers', {}), sc, seed, is_auth))
            except cr.Stop as exc:
                print('seed %d: stopped (%s): %s' % (seed, 'authored' if is_auth else 'original', exc))
                return 2
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


if __name__ == '__main__':
    runs = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    names = list(FUNCTIONS) if sys.argv[1] == 'all' else [sys.argv[1]]
    worst = 0
    for n in names:
        worst = max(worst, compare(n, runs))
    sys.exit(worst)
