#!/usr/bin/env python3
"""Print the build-plan entries for the w15-startup-a package, and optionally write a private
plan/snapshot copy that contains them.

The shared experimental/build-plan.json and experimental/snapshot.json are not edited by
this package; the lead integrator merges the printed entries. With --private-root DIR the
script writes DIR/snapshot.json, DIR/build-plan.json and a DIR/sources copy of
experimental/sources, so that

  python3 tools/build_partial.py --source-root DIR --plan DIR/build-plan.json \\
      --toolchain /path/to/reviewed/ee --output work/<new-dir>

compiles the shared selection plus these units. Nothing is written inside the repository.
Based on docs/agents/w11-crt-c-plan-entries.py.
With --with-others the two earlier-wave sources that define the three archive vblank views and the
loader failure word (authored, not in the shared plan) are included after this package's units. Object names are the unit directory names.

  python3 docs/agents/w15-startup-a-plan-entries.py --toolchain /path/to/ee [--private-root DIR]
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
OWN = 'w15-startup-a-'
# Sources of earlier packages that define names this package's units import and that are not yet in the
# shared plan. Only included with --with-others (to measure the closure in a private build); the lead
# integrator takes them from their owners' notes.
# In-place replacement: the w11 receiveControlPacket unit invents norrath_net_flag_bytes_48681c, the w14 revision
# indexes kickState + 4 (kickState is defined by w10-net-a-frame-state-data, already planned). Applied with --swap-receive.
SWAP = ('w11-game-a-receive-control-packet', ('w14-startup-io-b-receive-control-packet', 'receive_control_packet.cpp', []))
OTHER = [
    ('sony-cd-vblank-views', 'views.cpp', []),
    ('w14-startup-io-b-loader-failure-word', 'failure_word.cpp', []),
]
NOT_INPUTS = ('host_check.cpp', 'host_contract.cpp', 'host_test.cpp')


def discover(with_others=False, swap=False):
    units = []
    for d in sorted((EXP / 'sources').glob(OWN + '*')):
        files = [f for f in d.iterdir() if f.suffix in ('.cpp', '.S') and f.name not in NOT_INPUTS]
        assert len(files) == 1, (d, files)
        text = (files[0]).read_text()
        headers = []
        for line in text.splitlines():
            if line.startswith('#include "'):
                header = (d / line.split('"')[1]).resolve().relative_to(EXP)
                headers.append(str(header))
        units.append((d.name, files[0].name, headers))
    extra = list(OTHER) if with_others else []
    if swap:
        extra.append(SWAP[1])
    return units + extra


def sha256(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def load_driver():
    spec = importlib.util.spec_from_file_location('build_partial', REPO / 'tools' / 'build_partial.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def entries(toolchain, with_others=False, swap=False):
    driver = load_driver()
    plan = json.loads((EXP / 'build-plan.json').read_text())
    prefix = plan['toolchain']['target']
    tc = Path(toolchain).resolve()
    out = []
    with tempfile.TemporaryDirectory(prefix='w15sa-plan-') as scratch:
        for directory, source, headers in discover(with_others, swap):
            profile_name = 'authored_entry_asm' if source.endswith('.S') else 'authored_cpp'
            profile = plan['profiles'][profile_name]
            flags = [f.replace('{toolchain}', str(tc)) for f in profile['flags']]
            rel = 'sources/%s/%s' % (directory, source)
            object_name = directory.replace('-', '_') + '.o'
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


def private_copy(root, new_entries, swap=False):
    root = Path(root)
    root.mkdir(parents=True, exist_ok=False)
    shutil.copytree(EXP / 'sources', root / 'sources')
    plan = json.loads((EXP / 'build-plan.json').read_text())
    snapshot = json.loads((EXP / 'snapshot.json').read_text())
    for entry in new_entries:
        if swap and entry['source'].startswith('sources/%s/' % SWAP[1][0]):
            old = next(i for i in plan['inputs'] if i['source'].startswith('sources/%s/' % SWAP[0]))
            order = old['input_order']
            plan['inputs'][plan['inputs'].index(old)] = dict(entry, input_order=order)
            snap = next(i for i in snapshot['inputs'] if i['input_order'] == order)
            snap.update({'component': Path(entry['source']).parent.name, 'object_name': entry['object_name'],
                         'emitted_functions': entry['emitted_functions'], 'source': entry['source']})
            for name in (old['source'], *old['header_closure']):
                plan['files'].pop(name, None)
                snapshot['files'].pop(name, None)
            for name in [entry['source'], *entry['header_closure']]:
                digest = sha256(EXP / name)
                plan['files'][name] = digest
                snapshot['files'][name] = digest
            continue
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
    plan['default_authored_inputs'] += len(new_entries) - (1 if swap else 0)
    (root / 'snapshot.json').write_text(json.dumps(snapshot, indent=2) + '\n')
    plan['snapshot_sha256'] = sha256(root / 'snapshot.json')
    (root / 'build-plan.json').write_text(json.dumps(plan, indent=2) + '\n')
    return root


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--toolchain', required=True)
    parser.add_argument('--private-root')
    parser.add_argument('--with-others', action='store_true')
    parser.add_argument('--swap-receive', action='store_true')
    args = parser.parse_args()
    new_entries = entries(args.toolchain, args.with_others, args.swap_receive)
    for entry in new_entries:
        print(json.dumps(entry, sort_keys=True))
    if args.private_root:
        private_copy(args.private_root, new_entries, args.swap_receive)
        print('private plan written to', args.private_root, file=sys.stderr)


if __name__ == '__main__':
    main()
