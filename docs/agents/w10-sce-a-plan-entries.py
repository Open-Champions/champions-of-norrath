#!/usr/bin/env python3
"""Print the build-plan entries for the w10-sce-a package, and optionally write a private
plan/snapshot copy that contains them.

The shared experimental/build-plan.json and experimental/snapshot.json are not edited by
this package; the lead integrator merges the printed entries. With --private-root DIR the
script writes DIR/snapshot.json, DIR/build-plan.json and a DIR/sources copy of
experimental/sources, so that

  python3 tools/build_partial.py --source-root DIR --plan DIR/build-plan.json \\
      --toolchain /path/to/reviewed/ee --output work/<new-dir>

compiles the shared selection plus these units. Nothing is written inside the repository.

The units are every experimental/sources/w10-sce-a-* directory plus the earlier packages'
sources listed in OTHER below, which this package gave a host check. The .S files use the
authored_entry_asm profile, the rest authored_cpp.

  python3 docs/agents/w10-sce-a-plan-entries.py --toolchain /path/to/ee [--private-root DIR]
"""
import argparse
import hashlib
import importlib.util
import json
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
EXP = REPO / 'experimental'
OWN = 'w10-sce-a-'
# (directory, source file, header closure) of sources written by other packages and left out of the
# shared plan for want of a committed host test; this package added one.
OTHER = [
    ('w8-startup-sce-d-sif-get-sreg', 'sif_get_sreg.cpp', []),
    ('w9-startup-crt-f-mc-read', 'mc_read.cpp', []),
    ('w9-startup-crt-f-mc-write', 'mc_write.cpp', []),
    ('w9-startup-crt-f-pad-read', 'pad_read.cpp', []),
    ('w9-startup-crt-f-pad-set-act-direct', 'pad_set_act_direct.cpp', []),
    ('w9-startup-crt-f-tty-state', 'tty_state.cpp', ['sources/w9-startup-crt-f-tty-state/tty_state.hpp']),
    ('w9-startup-crt-f-tty-init', 'tty_init.cpp', ['sources/w9-startup-crt-f-tty-state/tty_state.hpp']),
    ('w9-startup-crt-f-tty-read', 'tty_read.cpp', ['sources/w9-startup-crt-f-tty-state/tty_state.hpp']),
    ('w9-startup-crt-f-tty-write', 'tty_write.cpp', ['sources/w9-startup-crt-f-tty-state/tty_state.hpp']),
    ('simloop-services-pad-dma-buffer', 'pad_dma_buffer.cpp', []),
]
NOT_INPUTS = ('host_check.cpp', 'host_contract.cpp', 'host_test.cpp')


def discover():
    units = []
    for d in sorted((EXP / 'sources').glob(OWN + '*')):
        files = [f for f in d.iterdir() if f.suffix in ('.cpp', '.S') and f.name not in NOT_INPUTS]
        assert len(files) == 1, (d, files)
        units.append((d.name, files[0].name, []))
    return units + [(d, s, h) for d, s, h in OTHER]


def sha256(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def load_driver():
    spec = importlib.util.spec_from_file_location('build_partial', REPO / 'tools' / 'build_partial.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def entries(toolchain):
    driver = load_driver()
    plan = json.loads((EXP / 'build-plan.json').read_text())
    prefix = plan['toolchain']['target']
    tc = Path(toolchain).resolve()
    out = []
    with tempfile.TemporaryDirectory(prefix='w10sce-plan-') as scratch:
        for directory, source, headers in discover():
            profile_name = 'authored_entry_asm' if source.endswith('.S') else 'authored_cpp'
            profile = plan['profiles'][profile_name]
            flags = [f.replace('{toolchain}', str(tc)) for f in profile['flags']]
            rel = 'sources/%s/%s' % (directory, source)
            object_name = Path(source).stem + '.o'
            obj = Path(scratch) / (directory + '.o')
            subprocess.check_call([str(tc / 'bin' / (prefix + '-' + profile['driver'])), *flags, '-c', str(EXP / rel),
                                   '-o', str(obj)], cwd=str(EXP / 'sources' / directory))
            inventory = driver.elf_inventory(obj)
            out.append({
                'header_closure': headers,
                'kind': 'independently_authored_source',
                'object_name': object_name,
                'profile': profile_name,
                'sha256': sha256(EXP / rel),
                'source': rel,
                'emitted_functions': [{'name': f['name'], 'compiled_body_bytes': f['compiled_body_bytes']}
                                      for f in inventory['functions']],
            })
    return out


def private_copy(root, new_entries):
    root = Path(root)
    root.mkdir(parents=True, exist_ok=False)
    shutil.copytree(EXP / 'sources', root / 'sources')
    plan = json.loads((EXP / 'build-plan.json').read_text())
    snapshot = json.loads((EXP / 'snapshot.json').read_text())
    for entry in new_entries:
        order = len(plan['inputs'])
        plan_entry = dict(entry, input_order=order)
        plan['inputs'].append(plan_entry)
        snapshot['inputs'].append({
            'input_order': order,
            'component': Path(entry['source']).parent.name,
            'object_name': entry['object_name'],
            'private_object_sha256': '0' * 64,
            'emitted_functions': entry['emitted_functions'],
            'kind': entry['kind'],
            'source': entry['source'],
        })
        for name in [entry['source'], *entry['header_closure']]:
            digest = sha256(EXP / name)
            plan['files'][name] = digest
            snapshot['files'][name] = digest
    plan['default_authored_inputs'] += len(new_entries)
    (root / 'snapshot.json').write_text(json.dumps(snapshot, indent=2) + '\n')
    plan['snapshot_sha256'] = sha256(root / 'snapshot.json')
    (root / 'build-plan.json').write_text(json.dumps(plan, indent=2) + '\n')
    return root


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--toolchain', required=True)
    parser.add_argument('--private-root')
    args = parser.parse_args()
    new_entries = entries(args.toolchain)
    for entry in new_entries:
        print(json.dumps(entry, sort_keys=True))
    if args.private_root:
        private_copy(args.private_root, new_entries)
        print('private plan written to', args.private_root, file=sys.stderr)


if __name__ == '__main__':
    main()
