#!/usr/bin/env python3
"""Print the build-plan entries for the w12-hud-loop-a package (and the earlier-wave units it relies on), and optionally
write a private plan/snapshot copy that contains them.

The shared experimental/build-plan.json and experimental/snapshot.json are not
edited by this package; the lead integrator merges the printed entries. For a
private check build, --private-root DIR writes DIR/snapshot.json, DIR/build-plan.json
and a DIR/sources copy of experimental/sources, so that

  python3 tools/build_partial.py --source-root DIR --plan DIR/build-plan.json \\
      --toolchain /path/to/reviewed/ee --output work/<new-dir>

compiles the shared selection plus these units. Nothing is written inside the
repository. Headers are found from the quoted includes of each source.

  python3 docs/agents/w12-hud-loop-a-plan-entries.py --toolchain /path/to/ee [--private-root DIR]
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
# (directory name, source file, object name). The first group is authored by this package; the second group are
# earlier-wave sources, already in the tree with committed host checks but never selected into the shared plan.
MINE = [
    ('font-string-size', 'font_string_size.cpp'),
    ('do-net-cam-toggle', 'do_net_cam_toggle.cpp'),
    ('drop-gold', 'drop_gold.cpp'),
    ('helpful-messages-draw', 'helpful_messages_draw.cpp'),
    ('draw-compass', 'draw_compass.cpp'),
    ('draw-gold', 'draw_gold.cpp'),
    ('get-data-for-all-objects', 'get_data_for_all_objects.cpp'),
    ('create-random-combineable-item', 'create_random_combineable_item.cpp'),
    ('create-named-random', 'create_named_random.cpp'),
    ('draw-bust', 'draw_bust.cpp'),
] + [(d, '%s.cpp' % d.replace('-', '_')) for d in [
    'data-arrow-left', 'data-arrow-right', 'data-beard-setting', 'data-bust-alpha', 'data-char-anim-test',
    'data-drew-helpful-message-help-bar', 'data-emergency-result', 'data-end-pos', 'data-expand-list',
    'data-free-list', 'data-game-flags', 'data-hair-color-setting', 'data-hair-style-setting', 'data-inv-header']]
EARLIER = [
    ('w10-front-a-mc-build-packed-player', 'mc_build_packed_player.cpp'),
    ('w9-frontend-a-mc-check-player-import-or-change', 'check_player_import_or_change.cpp'),
    ('w10-front-a-mc-convert-temp-player-to-real', 'mc_convert_temp_player_to_real.cpp'),
    ('w11-front-a-mc-load-level-entities', 'mc_load_level_entities.cpp'),
    ('w10-front-a-mc-net-pack', 'mc_net_pack.cpp'),
    ('w10-front-a-mc-net-un-pack', 'mc_net_un_pack.cpp'),
    ('w10-front-a-mc-set-ram-nm-players', 'mc_set_ram_nm_players.cpp'),
    ('w10-front-a-core-start-loading-screen', 'core_start_loading_screen.cpp'),
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
    with tempfile.TemporaryDirectory(prefix='w12hla-plan-') as scratch:
        listing = [('w12-hud-loop-a-' + d, f, 'w12_hla_' + d.replace('-', '_') + '.o') for d, f in MINE]
        listing += [(d, f, d.replace('-', '_') + '.o') for d, f in EARLIER]
        for suffix, source, object_name in listing:
            rel = 'sources/%s/%s' % (suffix, source)
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
