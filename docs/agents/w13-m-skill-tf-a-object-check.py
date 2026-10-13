#!/usr/bin/env python3
"""Compile each w13-m-skill-tf-a unit with the authored_cpp flags and check its symbols.

usage: w13-m-skill-tf-a-object-check.py EE_BIN_PREFIX SOURCES_ROOT OUTDIR [UNIT ...]

For each experimental/sources/w13-m-skill-tf-a-tf-*/typeinfo.cpp this checks that the
object defines the global function __tf<Class> and the 12-byte node __ti<Class> in
bss, and that its only undefined symbols are __tf5Skill, __rtti_si and __ti5Skill.
It also runs the host check (host_check.cpp) with -Werror. Prints one PASS/FAIL line per
unit. Nothing from the private image is read or written by this script.
"""
import glob, os, re, subprocess, sys

T, ROOT, OUT = sys.argv[1:4]
only = set(sys.argv[4:])
FLAGS = ("-G0 -O2 -ffreestanding -fno-asynchronous-unwind-tables -fno-exceptions -fno-pic -fno-rtti "
         "-fno-stack-protector -fno-unwind-tables -fomit-frame-pointer -fwrapv -mabi=eabi -march=r5900 "
         "-mfp32 -mgp64 -mlong32 -mno-abicalls -msingle-float -std=c++17 -nostdinc -nostdinc++ -x c++").split()
ALLOWED_UNDEF = {'__tf5Skill', '__rtti_si', '__ti5Skill'}
os.makedirs(OUT, exist_ok=True)
fails = 0
for src in sorted(glob.glob(os.path.join(ROOT, 'w13-m-skill-tf-a-tf-*', 'typeinfo.cpp'))):
    unit = os.path.basename(os.path.dirname(src))
    if only and unit not in only:
        continue
    d = os.path.dirname(src)
    host = os.path.join(OUT, unit + '-host')
    ok = True
    try:
        subprocess.check_call(['c++', '-std=c++17', '-Wall', '-Wextra', '-Werror', os.path.join(d, 'host_check.cpp'), '-o', host])
        subprocess.check_call([host], stdout=subprocess.DEVNULL)
    except subprocess.CalledProcessError:
        ok = False
    o = os.path.join(OUT, unit + '.o')
    subprocess.check_call([T + '-g++'] + FLAGS + ['-c', src, '-o', o])
    syms = subprocess.check_output([T + '-nm', '-S', o]).decode().splitlines()
    defined, undef, cls = {}, set(), None
    for l in syms:
        p = l.split()
        if len(p) == 2 and p[0] == 'U':
            undef.add(p[1])
        elif len(p) == 4:
            defined[p[3]] = (p[2], int(p[1], 16))
    tf = [n for n in defined if n.startswith('__tf') and n != '__tf5Skill']
    if len(tf) != 1 or defined[tf[0]][0] != 'T':
        ok = False
    else:
        cls = tf[0][len('__tf'):]
        if defined.get('__ti' + cls, ('?', 0)) != ('B', 12):
            ok = False
    if not undef <= ALLOWED_UNDEF:
        ok = False
    print(('PASS' if ok else 'FAIL'), unit, cls, sorted(undef))
    fails += 0 if ok else 1
sys.exit(1 if fails else 0)
