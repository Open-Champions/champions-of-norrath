#!/usr/bin/env python3
"""Print the build-plan entries for the w10-world-b sources, and optionally
write a private plan/snapshot copy that contains them.

The shared experimental/build-plan.json and experimental/snapshot.json are not
edited by this package; the lead integrator merges the printed entries. For a
private check build, --private-root DIR writes DIR/snapshot.json, DIR/build-plan.json
and a DIR/sources copy of experimental/sources, so that

  python3 tools/build_partial.py --source-root DIR --plan DIR/build-plan.json \\
      --toolchain /path/to/reviewed/ee --output work/<new-dir>

compiles the shared selection plus these units. Nothing is written inside the
repository. Headers are found from the quoted includes of each source.

  python3 docs/agents/w10-world-b-plan-entries.py --toolchain /path/to/ee [--private-root DIR]
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
P = 'w10-world-b-'

# directory suffix, source file name
UNITS = [
    ('map-init', 'map_init.cpp'),
    ('game-line-get-vert', 'game_line_get_vert.cpp'),
    ('game-line-get-next-point', 'game_line_get_next_point.cpp'),
    ('jump-gate-save-foot-print', 'jump_gate_save_foot_print.cpp'),
    ('jump-gate-load', 'jump_gate_load.cpp'),
    ('hide-hidden-objects', 'hide_hidden_objects.cpp'),
    ('demon-is-enemy', 'demon_is_enemy.cpp'),
    ('skeleton-is-enemy', 'skeleton_is_enemy.cpp'),
    ('get-closest-living-player', 'get_closest_living_player.cpp'),
    ('get-free-frame-state', 'get_free_frame_state.cpp'),
    ('game-line-init-position', 'game_line_init_position.cpp'),
    ('init-position-hermite', 'init_position_hermite.cpp'),
    ('is-boss', 'is_boss.cpp'),
    ('free-lump', 'free_lump.cpp'),
    ('init-jump-effect', 'init_jump_effect.cpp'),
    ('wraith-is-enemy', 'wraith_is_enemy.cpp'),
    ('jump-gate-data', 'jump_gate_data.cpp'),
    ('game-death-height-data', 'game_death_height_data.cpp'),
    ('ground-type-data', 'ground_type_data.cpp'),
    ('fs-list-back-data', 'fs_list_back_data.cpp'),
    ('frame-state-words-data', 'frame_state_words_data.cpp'),
    ('game-state-data', 'game_state_data.cpp'),
    ('game-line-ray-collision', 'game_line_ray_collision.cpp'),
    ('generate-sorted-list', 'generate_sorted_list.cpp'),
    ('game-line-fuse', 'game_line_fuse.cpp'),
    ('get-clearradius', 'get_clearradius.cpp'),
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
    with tempfile.TemporaryDirectory(prefix='w10wb-plan-') as scratch:
        for suffix, source in UNITS:
            rel = 'sources/%s%s/%s' % (P, suffix, source)
            object_name = 'w10wb_' + suffix.replace('-', '_') + '.o'
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
