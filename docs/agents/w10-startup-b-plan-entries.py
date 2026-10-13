#!/usr/bin/env python3
"""Print the build-plan entries for the w10-startup-b sources (and two already-authored
units that close names in this package's scope but are not in the shared plan yet), and
optionally write a private plan/snapshot copy that contains them.

The shared experimental/build-plan.json and experimental/snapshot.json are not edited by
this package; the lead integrator merges the printed entries. For a private check build,
--private-root DIR writes DIR/snapshot.json, DIR/build-plan.json and a DIR/sources copy of
experimental/sources, so that

  python3 tools/build_partial.py --source-root DIR --plan DIR/build-plan.json \\
      --toolchain /path/to/reviewed/ee --output work/<new-dir>

compiles the shared selection plus these units. Nothing is written inside the repository.
Units are discovered from the directories named w10-startup-b-*; host_contract.cpp,
host_check files and check_*.py are tests, not build inputs. Assembly sources use the
authored_entry_asm profile.

  python3 docs/agents/w10-startup-b-plan-entries.py --toolchain /path/to/ee [--private-root DIR]
"""
import argparse
import re
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
P = 'w10-startup-b-'
# Already-authored units, owned elsewhere, that define names in this package's scope.
EXTRA = [
    ('w8-startup-sce-b-k-exit-tlb-handler', 'k_exit_tlb_handler.cpp', 'w10sb_k_exit_tlb_handler.o'),
    ('sony-cd-vblank-views', 'views.cpp', 'w10sb_vblank_views.o'),
]
SKIP = {'host_contract.cpp', 'host_check.cpp', 'host_test.cpp'}


def own_units():
    units = []
    for directory in sorted((EXP / 'sources').iterdir()):
        if not directory.name.startswith(P):
            continue
        files = [f.name for f in directory.iterdir() if f.suffix in ('.cpp', '.S') and f.name not in SKIP]
        assert len(files) == 1, (directory, files)
        suffix = directory.name[len(P):].replace('-', '_')
        units.append((directory.name, files[0], 'w10sb_%s.o' % suffix))
    return units


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
    with tempfile.TemporaryDirectory(prefix='w10sb-plan-') as scratch:
        for directory, source, object_name in own_units() + EXTRA:
            rel = 'sources/%s/%s' % (directory, source)
            profile_name = 'authored_entry_asm' if source.endswith('.S') else 'authored_cpp'
            profile = plan['profiles'][profile_name]
            flags = [f.replace('{toolchain}', str(tc)) for f in profile['flags']]
            obj = Path(scratch) / object_name
            subprocess.check_call([str(tc / 'bin' / (prefix + '-' + profile['driver'])), *flags, '-c', str(EXP / rel),
                                   '-o', str(obj)], cwd=scratch)
            inventory = driver.elf_inventory(obj)
            out.append({
                'header_closure': [],
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
