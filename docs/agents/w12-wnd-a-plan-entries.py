#!/usr/bin/env python3
"""Print build-plan entries (one JSON object per line) for the w12-wnd-a sources and the
imports that they open. Compiles each unit with the authored_cpp flags; writes nothing in the repo.

  python3 docs/agents/w12-wnd-a-plan-entries.py --toolchain /path/to/ee
"""
import argparse, hashlib, json, subprocess, tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
SRC = REPO / 'experimental' / 'sources'
FLAGS = ['-G0', '-O2', '-ffreestanding', '-fno-asynchronous-unwind-tables', '-fno-exceptions', '-fno-pic',
         '-fno-rtti', '-fno-stack-protector', '-fno-unwind-tables', '-fomit-frame-pointer', '-fwrapv',
         '-mabi=eabi', '-march=r5900', '-mfp32', '-mgp64', '-mlong32', '-mno-abicalls', '-msingle-float',
         '-std=c++17', '-nostdinc', '-nostdinc++', '-x', 'c++']

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--toolchain', required=True)
    args = ap.parse_args()
    tc = Path(args.toolchain) / 'bin' / 'mips64r5900el-ps2-elf'
    defined, undefined, entries = set(), set(), []
    tmp = Path(tempfile.mkdtemp(prefix='w12wnda_'))
    for d in sorted(SRC.glob('w12-wnd-a-*')):
        files = [f for f in d.glob('*.cpp') if f.name not in ('host_check.cpp', 'host_test.cpp')]
        assert len(files) == 1, d
        f = files[0]
        obj = tmp / (d.name + '.o')
        subprocess.check_call([str(tc) + '-g++'] + FLAGS + ['-c', str(f), '-o', str(obj)])
        nm = subprocess.check_output([str(tc) + '-nm', '-S', str(obj)]).decode().split('\n')
        funcs = []
        for line in nm:
            p = line.split()
            if len(p) == 4 and p[2] in 'Tt':
                funcs.append({'compiled_body_bytes': int(p[1], 16), 'name': p[3]})
            if len(p) == 4 and p[2] in 'DdBbRr' and not p[3].startswith('.'):
                defined.add(p[3])
        for line in subprocess.check_output([str(tc) + '-nm', '-u', str(obj)]).decode().split('\n'):
            if line.strip(): undefined.add(line.split()[-1])
        defined.update(x['name'] for x in funcs)
        entries.append({'emitted_functions': sorted(funcs, key=lambda x: x['name']), 'header_closure': [],
                        'kind': 'independently_authored_source',
                        'object_name': 'w12wnda_' + d.name[len('w12-wnd-a-'):].replace('-', '_') + '.o',
                        'profile': 'authored_cpp', 'sha256': hashlib.sha256(f.read_bytes()).hexdigest(),
                        'source': 'sources/%s/%s' % (d.name, f.name)})
    for e in entries:
        print(json.dumps(e, sort_keys=True))
    print('# defined:', ' '.join(sorted(defined)))
    print('# opened imports:', ' '.join(sorted(undefined - defined)))

main()
