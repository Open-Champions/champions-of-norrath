#!/usr/bin/env python3
"""Print the build-plan entries for the w10-front-a sources, and optionally
write a private plan/snapshot copy that contains them.

The shared experimental/build-plan.json and experimental/snapshot.json are not
edited by this package; the lead integrator merges the printed entries. For a
private check build, --private-root DIR writes DIR/snapshot.json, DIR/build-plan.json
and a DIR/sources copy of experimental/sources, so that

  python3 tools/build_partial.py --source-root DIR --plan DIR/build-plan.json \\
      --toolchain /path/to/reviewed/ee --output work/<new-dir>

compiles the shared selection plus these units. Nothing is written inside the
repository. Headers are found from the quoted includes of each source.

  python3 docs/agents/w10-front-a-plan-entries.py --toolchain /path/to/ee [--private-root DIR]
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
P = 'w10-front-a-'

# directory suffix, source file name
UNITS = [
    ('mc-get-load-name', 'mc_get_load_name.cpp'),
    ('mc-get-ram-slot', 'mc_get_ram_slot.cpp'),
    ('mc-load-ram-slot', 'mc_load_ram_slot.cpp'),
    ('debug-start-menu-is-on', 'debug_start_menu_is_on.cpp'),
    ('mc-convert-temp-player-to-real', 'mc_convert_temp_player_to_real.cpp'),
    ('mc-build-packed-player', 'mc_build_packed_player.cpp'),
    ('mc-set-ram-nm-players', 'mc_set_ram_nm_players.cpp'),
    ('mc-reset-auto-check-point-save', 'mc_reset_auto_check_point_save.cpp'),
    ('get-save-table-entry', 'get_save_table_entry.cpp'),
    ('dir-entry-is-quick-load', 'dir_entry_is_quick_load.cpp'),
    ('get-slot-name', 'get_slot_name.cpp'),
    ('mc-replicate-first-players-important-things', 'mc_replicate_first_players_important_things.cpp'),
    ('eq-find', 'eq_find.cpp'),
    ('game-init-post-frontend', 'game_init_post_frontend.cpp'),
    ('mc-load-level-entities', 'mc_load_level_entities.cpp'),
    ('mc-net-pack', 'mc_net_pack.cpp'),
    ('mc-net-un-pack', 'mc_net_un_pack.cpp'),
    ('load-player-lumps', 'load_player_lumps.cpp'),
    ('core-force-small-characters', 'core_force_small_characters.cpp'),
    ('core-start-loading-screen', 'core_start_loading_screen.cpp'),
    ('browser-flags', 'browser_flags.cpp'),
    ('load-slot', 'load_slot.cpp'),
    ('mark-file-name', 'mark_file_name.cpp'),
    ('import-info-header', 'import_info_header.cpp'),
    ('hub-names', 'hub_names.cpp'),
    ('ascii-tables', 'ascii_tables.cpp'),
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
    with tempfile.TemporaryDirectory(prefix='w10fa-plan-') as scratch:
        for suffix, source in UNITS:
            rel = 'sources/%s%s/%s' % (P, suffix, source)
            object_name = 'w10fa_' + suffix.replace('-', '_') + '.o'
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
