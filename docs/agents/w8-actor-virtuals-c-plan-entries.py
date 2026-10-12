#!/usr/bin/env python3
"""Print the build-plan entries for the w8-actor-virtuals-c sources, and optionally
write a private plan/snapshot copy that contains them.

The shared experimental/build-plan.json and experimental/snapshot.json are not
edited by this package; the lead integrator merges the printed entries. For a
private check build, --private-root DIR writes DIR/snapshot.json, DIR/build-plan.json
and a DIR/sources copy of experimental/sources (the driver rejects symlinks that
leave the root), so that

  python3 tools/build_partial.py --source-root DIR --plan DIR/build-plan.json \\
      --toolchain /path/to/reviewed/ee --output work/<new-dir>

compiles the shared selection plus these units. Nothing is written inside the
repository.

  python3 docs/agents/w8-actor-virtuals-c-plan-entries.py --toolchain /path/to/ee [--private-root DIR]
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
P = 'w8-actor-virtuals-c-'
BYTES = 'sources/' + P + 'bytes/bytes.hpp'
HATE = 'sources/' + P + 'hate-run-common/hate_run.hpp'

# directory suffix, source file name, object name, extra headers
UNITS = [
    ('wake-monsters', 'wake_monsters.cpp', 'w8c_wake_monsters.o', [BYTES]),
    ('sleep-monsters', 'sleep_monsters.cpp', 'w8c_sleep_monsters.o', [BYTES]),
    ('save-activate', 'save_activate.cpp', 'w8c_save_activate.o', [BYTES]),
    ('teleporter-set-state', 'set_state.cpp', 'w8c_teleporter_set_state.o', []),
    ('teleporter-set-active-chunks', 'set_active_chunks.cpp', 'w8c_teleporter_set_active_chunks.o', []),
    ('goth-trap-run', 'run.cpp', 'w8c_goth_trap_run.o', []),
    ('hate-hole-run', 'run.cpp', 'w8c_hate_hole_run.o', []),
    ('shooter-save', 'save.cpp', 'w8c_shooter_save.o', [BYTES]),
    ('trigger-save', 'save.cpp', 'w8c_trigger_save.o', [BYTES]),
    ('auto-prop-save', 'save.cpp', 'w8c_auto_prop_save.o', [BYTES]),
    ('missile-trap-trigger', 'trigger.cpp', 'w8c_missile_trap_trigger.o', []),
    ('hack-door-passable', 'set_passable_state.cpp', 'w8c_hack_door_passable.o', [BYTES]),
    ('auto-prop-passable', 'set_passable_state.cpp', 'w8c_auto_prop_passable.o', [BYTES]),
    ('auto-prop-set-state', 'set_state.cpp', 'w8c_auto_prop_set_state.o', [BYTES]),
    ('auto-prop-trigger', 'trigger.cpp', 'w8c_auto_prop_trigger.o', [BYTES]),
    ('auto-prop-run', 'run.cpp', 'w8c_auto_prop_run.o', [BYTES]),
    ('goth-trap-passable', 'set_passable_state.cpp', 'w8c_goth_trap_passable.o', []),
    ('hate-bridge-passable', 'set_passable_state.cpp', 'w8c_hate_bridge_passable.o', []),
    ('hate-hole-passable', 'set_passable_state.cpp', 'w8c_hate_hole_passable.o', [BYTES]),
    ('container-save', 'save.cpp', 'w8c_container_save.o', [BYTES]),
    ('hate-into-run', 'run.cpp', 'w8c_hate_into_run.o', [HATE, BYTES]),
    ('hate-gate-run', 'run.cpp', 'w8c_hate_gate_run.o', [HATE, BYTES]),
    ('push-physics-prop-save', 'save.cpp', 'w8c_push_physics_prop_save.o', [BYTES]),
    ('trigger-run', 'run.cpp', 'w8c_trigger_run.o', [BYTES]),
    ('missile-trap-run', 'run.cpp', 'w8c_missile_trap_run.o', [BYTES]),
    ('fire-bomb-run', 'run.cpp', 'w8c_fire_bomb_run.o', [BYTES]),
    ('shooter-run', 'run.cpp', 'w8c_shooter_run.o', [BYTES]),
    ('container-run', 'run.cpp', 'w8c_container_run.o', [BYTES]),
    ('push-physics-prop-run', 'run.cpp', 'w8c_push_physics_prop_run.o', [BYTES]),
]


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
    with tempfile.TemporaryDirectory(prefix='w8c-plan-') as scratch:
        for suffix, source, object_name, headers in UNITS:
            rel = 'sources/%s%s/%s' % (P, suffix, source)
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
