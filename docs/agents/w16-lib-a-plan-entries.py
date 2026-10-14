#!/usr/bin/env python3
"""Print the build-plan entries for the w16-lib-a sources plus seven w15-libs-eh-a units that are not yet in the plan, and optionally write
a private plan/snapshot copy that contains them.

The shared experimental/build-plan.json and experimental/snapshot.json are not edited by
this package; the lead integrator merges the printed entries. For a private check build,
--private-root DIR writes DIR/snapshot.json, DIR/build-plan.json and a DIR/sources copy of
experimental/sources, so that

  python3 tools/build_partial.py --source-root DIR --plan DIR/build-plan.json \\
      --toolchain /path/to/reviewed/ee --output work/<new-dir>

compiles the shared selection plus these units. Nothing is written inside the repository.
Units are discovered from the directories named w16-lib-a-*, plus the seven w15-libs-eh-a-* units listed in
EXTRA below (authored by an earlier package, selected for no plan entry yet); the header closure is found by
following the quoted includes.

  python3 docs/agents/w16-lib-a-plan-entries.py --toolchain /path/to/ee [--private-root DIR]
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
P = 'w16-lib-a-'
EXTRA = [
    ('w15-libs-eh-a-gsm-long-term-predictor', 'long_term_predictor.cpp', 'w15eha_gsm_long_term_predictor.o'),
    ('w15-libs-eh-a-gsm-lpc-analysis', 'lpc_analysis.cpp', 'w15eha_gsm_lpc_analysis.o'),
    ('w15-libs-eh-a-gsm-rpe-encoding', 'rpe_encoding.cpp', 'w15eha_gsm_rpe_encoding.o'),
    ('w15-libs-eh-a-inflate-blocks', 'inflate_blocks.cpp', 'w15eha_inflate_blocks.o'),
    ('w15-libs-eh-a-inflate-blocks-free', 'inflate_blocks_free.cpp', 'w15eha_inflate_blocks_free.o'),
    ('w15-libs-eh-a-inflate-blocks-reset', 'inflate_blocks_reset.cpp', 'w15eha_inflate_blocks_reset.o'),
    ('w15-libs-eh-a-inflate-init2', 'inflate_init2.cpp', 'w15eha_inflate_init2.o'),
]
def own_units():
    units = []
    for directory in sorted((EXP / 'sources').iterdir()):
        if not directory.name.startswith(P) or directory.name.endswith('-common'):
            continue
        cpp = [f.name for f in directory.glob('*.cpp') if not f.name.endswith('_host_test.cpp') and f.name != 'host_check.cpp']
        if not cpp:
            continue
        assert len(cpp) == 1, directory
        suffix = directory.name[len(P):].replace('-', '_')
        units.append((directory.name, cpp[0], 'w16la_%s.o' % suffix, None))
    return units + [(d, c, o, None) for d, c, o in EXTRA]


def closure(rel):
    """Quoted includes of a source, followed transitively, as sources/... paths in first-seen order."""
    seen = []
    todo = [EXP / rel]
    while todo:
        path = todo.pop(0)
        for line in path.read_text().splitlines():
            match = re.match(r'\s*#\s*include\s+"([^"]+)"', line)
            if not match:
                continue
            target = (path.parent / match.group(1)).resolve()
            name = 'sources/' + str(target.relative_to((EXP / 'sources').resolve()))
            if name not in seen:
                seen.append(name)
                todo.append(target)
    return seen


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
    with tempfile.TemporaryDirectory(prefix='w16la-plan-') as scratch:
        units = own_units()
        for directory, source, object_name, headers in units:
            rel = 'sources/%s/%s' % (directory, source)
            headers = closure(rel) if headers is None else headers
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
