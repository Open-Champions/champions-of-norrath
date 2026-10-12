#!/usr/bin/env python3
"""Differential check of the static-initializer sources against the private executable.

For each authored unit under experimental/sources/w8-spell-static-init-b-*/ this runs
the wrapper (`_GLOBAL_$I$...`) from the private SLUS_205.65 image and from the
freshly compiled and linked authored object in a small R5900 interpreter. Every
callee is replaced by a stub that records the argument registers that callee
really reads (objects are logged by order of first use, strings by content), and
the two logs must be equal. The unit's own static-initialization routine is also
run directly with the four combinations of (initialize, priority) in {0,1} x
{0xffff, 0}. Writes outside the stack must match as well.

  NORRATH_EXE=/path/SLUS_205.65 NORRATH_EE=/path/to/ee \\
      python3 docs/agents/w8-spell-static-init-b-compare.py all

The interpreter is the one in combat-reactions-compare.py in this directory. The
executable and toolchain stay outside the repository; nothing from the executable
is stored, and scratch files go to a fresh temporary directory.
"""
import glob
import importlib.util
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
spec = importlib.util.spec_from_file_location('cr_compare', os.path.join(HERE, 'combat-reactions-compare.py'))
cr = importlib.util.module_from_spec(spec)
spec.loader.exec_module(cr)

P = cr.P
SRC = cr.SRC
STACK = cr.STACK
STUB = 0x00500000

# callee -> what its call is logged with. 'o' object (ordinal of first use),
# 'i' integer register, 's' C string, 'f' float register, 'k' stack word.
ARGS = {
    '__11ParticleDefiiiiiiiifffiiiPFPv_v': [('r', 4, 'o')] + [('r', n, 'i') for n in range(5, 12)]
        + [('k', 0)] + [('f', n) for n in (12, 13, 14)] + [('k', o) for o in (8, 16, 24, 32)],
    '__11ResourceMapPcT1': [('r', 4, 'o'), ('r', 5, 's'), ('r', 6, 's')],
}


def spec_for(name):
    if name in ARGS:
        return ARGS[name]
    if re.match(r'__\d+(Skill|Spell)', name):
        return [('r', 4, 'o')]
    raise SystemExit('no argument layout for callee ' + name)


def image_symbols():
    out = subprocess.check_output([P + 'nm', cr.EXE], text=True).splitlines()
    syms = {}
    for l in out:
        p = l.split()
        if len(p) == 3:
            syms.setdefault(p[2], []).append(int(p[0], 16))
    return syms


def cstr(m, a):
    out = bytearray()
    while len(out) < 80:
        c = m.u8(a + len(out))
        if c == 0:
            break
        out.append(c)
    return bytes(out)


def make_stub(name, m, objs):
    lay = spec_for(name)

    def stub(mm):
        sp = mm.g32(29)
        row = [name]
        for item in lay:
            if item[0] == 'r':
                v = mm.g32(item[1])
                if item[2] == 'o':
                    if v not in objs:
                        objs[v] = len(objs)
                    row.append(('obj', objs[v]))
                elif item[2] == 's':
                    row.append(cstr(mm, v))
                else:
                    row.append(v)
            elif item[0] == 'f':
                row.append(mm.f[item[1]])
            else:
                row.append(mm.u32(sp + item[1]))
        m.log.append(tuple(row))
    return stub


def run(image, entry, stub_addrs, args):
    mem = {}
    stubs = {}
    m = cr.Machine(image, mem, stubs, cr.GP)
    objs = {}
    for addr, name in stub_addrs.items():
        stubs[addr] = make_stub(name, m, objs)
    m.r[29] = STACK
    for i, v in enumerate(args):
        m.r[4 + i] = v
    m.run(entry)
    writes = {a: v for a, v in mem.items() if not (STACK - 0x2000 <= a < STACK + 0x100)}
    return m.log, writes


def wrapper_target(exe, addr):
    for k in range(8):
        w = exe.byte(addr + 4 * k) | exe.byte(addr + 4 * k + 1) << 8 | exe.byte(addr + 4 * k + 2) << 16 | exe.byte(addr + 4 * k + 3) << 24
        if w >> 26 == 3:
            return ((addr + 4 * k + 4) & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
    raise SystemExit('no call in wrapper at %#x' % addr)


def compare(path, exe, syms):
    text = open(path).read()
    sym = re.search(r'void (_GLOBAL_\$I\$\w+)\(\)', text).group(1)
    work = os.path.join(cr.SCRATCH, os.path.basename(os.path.dirname(path)))
    os.makedirs(work, exist_ok=True)
    obj = os.path.join(work, 'init.o')
    subprocess.check_call([P + 'g++', *cr.FLAGS, '-c', path, '-o', obj])
    und = [l.split()[-1] for l in subprocess.check_output([P + 'nm', '-u', obj], text=True).splitlines() if l.strip()]
    orig_stubs = {}
    auth_stubs = {}
    defs = {}
    for n, name in enumerate(und):
        if len(syms.get(name, [])) != 1:
            raise SystemExit('import %s is not one image symbol' % name)
        orig_stubs[syms[name][0]] = name
        defs[name] = STUB + 0x10 * n
        auth_stubs[defs[name]] = name
    elf = os.path.join(work, 'init.elf')
    cmd = [P + 'ld', '-m', 'elf32lr5900', '-Ttext=0x00400000', '--no-warn-mismatch']
    cmd += ['--defsym=%s=%#x' % kv for kv in defs.items()] + ['-o', elf, obj]
    subprocess.check_call(cmd)
    auth = cr.Image(elf)
    names = {}
    for l in subprocess.check_output([P + 'nm', elf], text=True).splitlines():
        p = l.split()
        if len(p) == 3:
            names[p[2]] = int(p[0], 16)
    a_wrap = names[sym]
    a_loc = names['norrath_static_init']
    o_wrap = syms[sym][0]
    o_loc = wrapper_target(exe, o_wrap)
    bad = 0
    cases = [('wrapper', o_wrap, a_wrap, [])]
    for init in (1, 0):
        for pri in (0xffff, 0):
            cases.append(('local(%d,%#x)' % (init, pri), o_loc, a_loc, [init, pri]))
    for label, oe, ae, args in cases:
        ra = run(exe, oe, orig_stubs, args)
        rb = run(auth, ae, auth_stubs, args)
        if ra != rb:
            bad += 1
            n = min(len(ra[0]), len(rb[0]))
            k = next((i for i in range(n) if ra[0][i] != rb[0][i]), n)
            print('MISMATCH %s %s at call %d:\n  image %s\n  built %s' % (sym, label, k, ra[0][k:k + 1], rb[0][k:k + 1]))
        if label == 'wrapper' and not ra[0]:
            bad += 1
            print('%s: the image wrapper made no calls; the check proves nothing' % sym)
    print('%s: %d calls, %s' % (sym, len(ra[0]) if label == 'wrapper' else len(run(exe, o_wrap, orig_stubs, [])[0]), 'FAIL' if bad else 'equal'))
    return bad


if __name__ == '__main__':
    exe = cr.Image(cr.EXE)
    syms = image_symbols()
    paths = sorted(glob.glob(os.path.join(SRC, 'w8-spell-static-init-b-*', 'init.cpp')))
    if sys.argv[1] != 'all':
        paths = [p for p in paths if sys.argv[1] in p]
    worst = 0
    for p in paths:
        worst = max(worst, 1 if compare(p, exe, syms) else 0)
    sys.exit(worst)
