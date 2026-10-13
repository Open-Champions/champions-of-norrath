#!/usr/bin/env python3
"""Compare compiled w9-ctors-wraith-a view objects with the private reference executable.

usage: w9-ctors-wraith-a-compare.py EXE TOOLCHAIN_PREFIX OBJECT_DIR
Prints only pass/fail lines; no game bytes are written anywhere. Every object's
symbol must have the reference symbol's size, equal non-pointer words, string
pointers that select equal strings, function pointers whose relocation names the
reference function symbol at that address, and zero words with no relocation.
"""
import re, struct, subprocess, sys, tempfile, os

exe = open(sys.argv[1], 'rb').read()
tc, objdir = sys.argv[2], sys.argv[3]
SECS = [(0x31f480, 0x220480, 0x1344f0), (0x454300, 0x355300, 0x2b4f4), (0x47fa00, 0x380a00, 0x379c), (0x483200, 0x384200, 0x35de)]

def ref(addr, n):
    for a, o, s in SECS:
        if a <= addr and addr + n <= a + s:
            return exe[o + addr - a:o + addr - a + n]
    raise KeyError(hex(addr))

def cstr(buf, off):
    return buf[off:buf.index(b'\0', off)]

def ref_cstr(addr):
    out = b''
    while True:
        c = ref(addr + len(out), 1)
        if c == b'\0':
            return out
        out += c

def run(*a):
    return subprocess.run(list(a), check=True, capture_output=True, text=True).stdout

nm = {}
for l in run(tc + '-nm', '-S', '-n', sys.argv[1]).splitlines():
    p = l.split()
    if len(p) == 4:
        nm.setdefault(int(p[0], 16), []).append((int(p[1], 16), p[2], p[3]))

bad = 0
for fn in sorted(os.listdir(objdir)):
    m = re.fullmatch(r'norrath_ctors_lava_wraith_view_([0-9a-f]+)\.o', fn)
    if not m:
        continue
    addr = int(m.group(1), 16)
    path = os.path.join(objdir, fn)
    sym = 'norrath_ctors_lava_wraith_view_' + m.group(1)
    info = [l.split() for l in run(tc + '-nm', '-S', path).splitlines() if l.endswith(' ' + sym)][0]
    problems = []
    # the interior view of `skills` has no symbol of its own in the reference
    refsize = 4 if addr == 0x34b474 else [s for s, t, n in nm[addr]][0]
    objsize = int(info[1], 16)
    if objsize != refsize:
        problems.append('size %d != %d' % (objsize, refsize))
    sect = {}
    for l in run(tc + '-readelf', '-S', '-W', path).splitlines():
        mm = re.match(r'\s*\[\s*\d+\]\s+(\S+)\s+\S+\s+\S+\s+([0-9a-f]+)\s+([0-9a-f]+)', l)
        if mm:
            sect[mm.group(1)] = (int(mm.group(2), 16), int(mm.group(3), 16))
    with tempfile.NamedTemporaryFile() as t:
        subprocess.run([tc + '-objcopy', '-O', 'binary', '--only-section=.data', path, t.name], check=True)
        data = open(t.name, 'rb').read()
    symoff = int(info[0], 16)
    relocs = {}
    for l in run(tc + '-readelf', '-r', '-W', path).splitlines():
        mm = re.match(r'([0-9a-f]{8})\s+\S+\s+R_MIPS_32\s+\S+\s+(\S+)', l)
        if mm:
            relocs[int(mm.group(1), 16)] = mm.group(2)
    if info[2] in 'dD':
        words = [(i, struct.unpack('<I', data[symoff + i:symoff + i + 4])[0]) for i in range(0, refsize, 4)]
        r = ref(addr, refsize) if addr != 0x34b474 else struct.pack('<I', 0x602de8)
        for i, w in words:
            rw = struct.unpack('<I', r[i:i + 4])[0]
            rel = relocs.get(symoff + i)
            if rw == 0:
                if w or rel:
                    problems.append('word %d should be null' % (i // 4))
            elif addr == 0x34b474:
                if rel != 'skillFireWeapons':
                    problems.append('word target %s' % rel)
            elif rw >= 0x31f480:
                # pointer into string data (.data, .rodata or .sdata)
                if rel != '.data':
                    problems.append('slot %d has no string relocation' % (i // 4))
                elif cstr(data, w) != ref_cstr(rw):
                    problems.append('slot %d string differs' % (i // 4))
            elif 0x100000 <= rw < 0x31b510:
                names = [n for s, t_, n in nm.get(rw, []) if t_ in 'TtWw']
                if rel not in names:
                    problems.append('slot %d function %s not in %s' % (i // 4, rel, names))
            elif w != rw:
                problems.append('word %d differs' % (i // 4))
    else:
        r = ref(addr, refsize)
        if any(r):
            problems.append('reference is not all zero')
    print(('FAIL ' if problems else 'ok   ') + fn, '; '.join(problems))
    bad += bool(problems)
sys.exit(1 if bad else 0)
