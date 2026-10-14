#!/usr/bin/env python3
"""Print the build-plan entries for the w9-animation-object-a sources, and optionally
write a private plan/snapshot copy that contains them.

The shared experimental/build-plan.json and experimental/snapshot.json are not
edited by this package; the lead integrator merges the printed entries. For a
private check build, --private-root DIR writes DIR/snapshot.json, DIR/build-plan.json
and a DIR/sources copy of experimental/sources, so that

  python3 tools/build_partial.py --source-root DIR --plan DIR/build-plan.json \\
      --toolchain /path/to/reviewed/ee --output work/<new-dir>

compiles the shared selection plus these units. Nothing is written inside the
repository. Headers are found from the quoted includes of each source.

  python3 docs/agents/w9-animation-object-a-plan-entries.py --toolchain /path/to/ee [--private-root DIR]
"""
import argparse
import hashlib
import importlib.util
import json
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
EXP = REPO / 'experimental'
P = 'w9-animation-object-a-'

# directory suffix, source file name
UNITS = [
    ('erand', 'erand.cpp'),
    ('b-exit-state-persue-idle', 'b_exit_state_persue_idle.cpp'),
    ('b-far-from-home', 'b_far_from_home.cpp'),
    ('anim-find-component-with-flags', 'anim_find_component_with_flags.cpp'),
    ('anim-add-to-end', 'anim_add_to_end.cpp'),
    ('anim-add-hairy', 'anim_add_hairy.cpp'),
    ('anim-add-one-shot', 'anim_add_one_shot.cpp'),
    ('anim-global-sync', 'anim_global_sync.cpp'),
    ('anim-advance-animation', 'anim_advance_animation.cpp'),
    ('game-line-init-path-state', 'game_line_init_path_state.cpp'),
    ('find-tag-float', 'find_tag_float.cpp'),
    ('get-smart-random-destination', 'get_smart_random_destination.cpp'),
    ('anim-map-animation-sounds', 'anim_map_animation_sounds.cpp'),
    ('anim-state-data', 'anim_state_data.cpp'),
    ('lump-data', 'lump_data.cpp'),
    ('game-line-data', 'game_line_data.cpp'),
    ('in-msg-data', 'in_msg_data.cpp'),
    ('has-light-or-particle-number', 'has_light_or_particle_number.cpp'),
    ('game-line-update-position', 'game_line_update_position.cpp'),
    ('convert-pointers-to-indicies', 'convert_pointers_to_indicies.cpp'),
    ('generic-move', 'generic_move.cpp'),
]


def header_closure(rel):
    """Quoted includes of the source, as plan-relative paths (one level deep is all these use)."""
    found = []
    base = (EXP / rel).parent
    for line in (EXP / rel).read_text().splitlines():
        m = re.match(r'\s*#\s*include\s+"([^"]+)"', line)
        if m:
            path = (base / m.group(1)).resolve().relative_to(EXP.resolve()).as_posix()
            if path not in found:
                found.append(path)
    return found


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
    with tempfile.TemporaryDirectory(prefix='w9ao-plan-') as scratch:
        for suffix, source in UNITS:
            rel = 'sources/%s%s/%s' % (P, suffix, source)
            object_name = 'w9ao_' + suffix.replace('-', '_') + '.o'
            headers = header_closure(rel)
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
