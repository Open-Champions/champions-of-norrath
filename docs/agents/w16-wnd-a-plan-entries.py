#!/usr/bin/env python3
"""Print the build-plan entries for the w16-wnd-a sources, and optionally
write a private plan/snapshot copy that contains them.

The shared experimental/build-plan.json and experimental/snapshot.json are not edited by
this package; the lead integrator merges the printed entries. With --private-root DIR the
script writes DIR/snapshot.json, DIR/build-plan.json and a DIR/sources copy of
experimental/sources, so that

  python3 tools/build_partial.py --source-root DIR --plan DIR/build-plan.json \\
      --toolchain /path/to/reviewed/ee --output work/<new-dir>

compiles the shared selection plus these units. Nothing is written inside the repository.
Each unit not yet selected (w12-wnd-a-*, two w9-render-math-a units, w16-wnd-a-*) is one directory whose only
source file that is not host_check.cpp is the plan input.

  python3 docs/agents/w16-wnd-a-plan-entries.py --toolchain /path/to/ee [--private-root DIR]
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
EXTRA = ['w9-render-math-a-draw-character-call-back', 'w9-render-math-a-draw-up-from']

def discover():
    """Every unit of this package that the shared plan does not select yet: the w12-wnd-a-* units that now carry a
    committed host check, the two w9-render-math-a drawing units, and the w16-wnd-a-* units."""
    plan = json.loads((EXP / 'build-plan.json').read_text())
    planned = {Path(i['source']).parent.name for i in plan['inputs']}
    names = [d.name for d in sorted((EXP / 'sources').glob('w12-wnd-a-*'))] + EXTRA + \
            [d.name for d in sorted((EXP / 'sources').glob('w16-wnd-a-*')) if d.name != 'w16-wnd-a-common']
    units = []
    for name in names:
        if name in planned:
            continue
        d = EXP / 'sources' / name
        files = [f for f in d.iterdir() if f.suffix == '.cpp' and f.name not in ('host_check.cpp', 'host_test.cpp')]
        assert len(files) == 1, (d, files)
        assert (d / 'host_check.cpp').exists(), d
        headers = []
        for line in files[0].read_text().splitlines():
            if line.startswith('#include "'):
                target = (d / line.split('"')[1]).resolve()
                headers.append(str(target.relative_to(EXP)))
        units.append((name, files[0].name, 'w16wa_' + name.replace('-', '_') + '.o', headers))
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
    profile = plan['profiles']['authored_cpp']
    prefix = plan['toolchain']['target']
    tc = Path(toolchain).resolve()
    flags = [f.replace('{toolchain}', str(tc)) for f in profile['flags']]
    out = []
    with tempfile.TemporaryDirectory(prefix='w16wa-plan-') as scratch:
        for name, source, object_name, headers in discover():
            rel = 'sources/%s/%s' % (name, source)
            obj = Path(scratch) / object_name
            subprocess.check_call([str(tc / 'bin' / (prefix + '-' + profile['driver'])), *flags, '-c', str(EXP / rel),
                                   '-o', str(obj)], cwd=scratch)
            inventory = driver.elf_inventory(obj)
            out.append({
                'header_closure': headers,
                'kind': 'independently_authored_source',
                'object_name': object_name,
                'profile': 'authored_cpp',
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
