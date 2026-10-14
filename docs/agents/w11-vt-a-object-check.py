#!/usr/bin/env python3
"""Compare the compiled vtable objects with the private image (local reference only).

usage: w11-vt-a-object-check.py EE_BIN_PREFIX IMAGE SOURCES_ROOT OUTDIR
Compiles each experimental/sources/w11-vt-a-vtable-*/vt_*.cpp with the authored_cpp
flags, then checks that the object defines the _vt$ symbol with the image's size and
that every relocated slot word names the function symbol found at the same slot of the
image table. Prints only pass/fail per table; nothing from the image is written out.
"""
import glob, os, re, struct, subprocess, sys

T, IMG, ROOT, OUT = sys.argv[1:5]
FLAGS = ("-G0 -O2 -ffreestanding -fno-asynchronous-unwind-tables -fno-exceptions -fno-pic -fno-rtti "
         "-fno-stack-protector -fno-unwind-tables -fomit-frame-pointer -fwrapv -mabi=eabi -march=r5900 "
         "-mfp32 -mgp64 -mlong32 -mno-abicalls -msingle-float -std=c++17 -nostdinc -nostdinc++ -x c++").split()
os.makedirs(OUT, exist_ok=True)
nm = subprocess.check_output([T + '-nm', '-S', '-n', IMG]).decode().split('\n')
funcs, tabs = {}, {}
for l in nm:
    p = l.split()
    if len(p) == 4:
        a, sz, ty, name = int(p[0], 16), int(p[1], 16), p[2], p[3]
        if ty in 'TtWw' and not name.startswith(('gcc2', '__gnu')):
            funcs.setdefault(a, []).append(name)
        if name.startswith('_vt$'):
            tabs[name] = (a, sz)
def img_words(a, n):
    out = subprocess.check_output([T + '-objdump', '-s', '--start-address=0x%x' % a,
                                   '--stop-address=0x%x' % (a + n), IMG]).decode().split('\n')
    b = b''
    for l in out:
        toks = [t for t in l.split()[1:5] if re.fullmatch(r'[0-9a-f]{8}', t)]
        if toks and re.match(r'\s*[0-9a-f]+\s', l):
            b += bytes.fromhex(''.join(toks))
    return struct.unpack('<%dI' % (n // 4), b[:n])
bad = 0
for src in sorted(glob.glob(ROOT + '/w11-vt-a-vtable-*/vt_*.cpp')):
    o = os.path.join(OUT, os.path.basename(src) + '.o')
    subprocess.check_call([T + '-g++'] + FLAGS + ['-c', src, '-o', o])
    syms = subprocess.check_output([T + '-nm', '-S', o]).decode().split('\n')
    vt = [l.split() for l in syms if len(l.split()) == 4 and l.split()[3].startswith('_vt$')]
    assert len(vt) == 1, src
    name, size = vt[0][3], int(vt[0][1], 16)
    ia, isz = tabs[name]
    w = img_words(ia, isz)
    # relocations of .data: offset -> symbol
    rel = {}
    for l in subprocess.check_output([T + '-readelf', '-r', '-W', o]).decode().split('\n'):
        m = re.match(r'([0-9a-f]{8})\s+[0-9a-f]+\s+R_MIPS_32\s+[0-9a-f]+\s+(\S+)', l)
        if m: rel[int(m.group(1), 16)] = m.group(2)
    ok = size == isz
    for i in range(isz // 8):
        want = funcs.get(w[2 * i + 1], []) if w[2 * i + 1] else []
        got = rel.get(8 * i + 4)
        if w[2 * i] != 0: ok = False
        if want == [] and got is None: continue
        if got not in want: ok = False
    ok = ok and len(rel) == sum(1 for i in range(isz // 8) if w[2 * i + 1])
    print(('PASS' if ok else 'FAIL'), name)
    bad += not ok
sys.exit(1 if bad else 0)
