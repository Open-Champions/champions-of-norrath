#!/usr/bin/env python3
"""Print the build-plan entries for the w10-startup-a sources, and optionally write a private
plan/snapshot copy that contains them.

The shared experimental/build-plan.json and experimental/snapshot.json are not edited by this
package; the lead integrator merges the printed entries. For a private check build,
--private-root DIR writes DIR/snapshot.json, DIR/build-plan.json and a DIR/sources copy of
experimental/sources (the driver rejects symlinks that leave the root), so that

  python3 tools/build_partial.py --source-root DIR --plan DIR/build-plan.json \\
      --toolchain /path/to/reviewed/ee --output work/<new-dir>

compiles the shared selection plus these units. Nothing is written inside the repository.
The unit marked "other package" (w8-startup-sce-a-ctype-table) is an existing authored source
that defines _ctype_ but is absent from the shared plan; it is listed so the integrator can see
what closing that import needs.

  python3 docs/agents/w10-startup-a-plan-entries.py --toolchain /path/to/ee [--private-root DIR]
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
DIV = 'sources/w9-startup-crt-c-libgcc-div-common/udivmod.hpp'

# directory, source file name, object name, extra headers, profile
UNITS = [
    ('w10-startup-a-unpack-f', 'unpack_f.cpp', 'w10sa_unpack_f.o', [], 'authored_cpp'),
    ('w10-startup-a-unpack-d', 'unpack_d.cpp', 'w10sa_unpack_d.o', [], 'authored_cpp'),
    ('w10-startup-a-pack-d', 'pack_d.cpp', 'w10sa_pack_d.o', [], 'authored_cpp'),
    ('w10-startup-a-make-dp', 'make_dp.cpp', 'w10sa_make_dp.o', [], 'authored_cpp'),
    ('w10-startup-a-make-fp', 'make_fp.cpp', 'w10sa_make_fp.o', [], 'authored_cpp'),
    ('w10-startup-a-fpcmp-parts-d', 'fpcmp_parts_d.cpp', 'w10sa_fpcmp_parts_d.o', [], 'authored_cpp'),
    ('w10-startup-a-sleep-thread', 'sleep_thread.S', 'w10sa_sleep_thread.o', [], 'authored_entry_asm'),
    ('w10-startup-a-deci2-call', 'deci2_call.S', 'w10sa_deci2_call.o', [], 'authored_entry_asm'),
    ('w10-startup-a-get-memory-size', 'get_memory_size.S', 'w10sa_get_memory_size.o', [], 'authored_entry_asm'),
    ('w10-startup-a-init-tlb-call', 'init_tlb_call.S', 'w10sa_init_tlb_call.o', [], 'authored_entry_asm'),
    ('w10-startup-a-rtti-user', 'rtti_user.cpp', 'w10sa_rtti_user.o', [], 'authored_cpp'),
    ('w10-startup-a-ieee754-sqrtf', 'ieee754_sqrtf.cpp', 'w10sa_ieee754_sqrtf.o', [], 'authored_cpp'),
    ('w10-startup-a-fixunsdfdi', 'fixunsdfdi.cpp', 'w10sa_fixunsdfdi.o', [], 'authored_cpp'),
    ('w10-startup-a-divdi3', 'divdi3.cpp', 'w10sa_divdi3.o', [DIV], 'authored_cpp'),
    ('w10-startup-a-moddi3', 'moddi3.cpp', 'w10sa_moddi3.o', [DIV], 'authored_cpp'),
    ('w10-startup-a-exit', 'exit.cpp', 'w10sa_exit.o', [], 'authored_cpp'),
    ('w10-startup-a-alarm-handler', 'alarm_handler.cpp', 'w10sa_alarm_handler.o', [], 'authored_cpp'),
    ('w10-startup-a-create-reader-thread', 'create_reader_thread.cpp', 'w10sa_create_reader_thread.o', [], 'authored_cpp'),
    ('w10-startup-a-init-tlb-32mb', 'init_tlb_32mb.cpp', 'w10sa_init_tlb_32mb.o', [], 'authored_cpp'),
    ('w10-startup-a-eh-alloc', 'eh_alloc.cpp', 'w10sa_eh_alloc.o', [], 'authored_cpp'),
    ('w10-startup-a-cp-push-exception', 'cp_push_exception.cpp', 'w10sa_cp_push_exception.o', [], 'authored_cpp'),
    ('w10-startup-a-default-terminate', 'default_terminate.cpp', 'w10sa_default_terminate.o', [], 'authored_cpp'),
    ('w10-startup-a-new-handler', 'new_handler.cpp', 'w10sa_new_handler.o', [], 'authored_cpp'),
    ('w10-startup-a-terminate-func', 'terminate_func.cpp', 'w10sa_terminate_func.o', [], 'authored_cpp'),
    ('w10-startup-a-kernel-rem-pio2f', 'kernel_rem_pio2f.cpp', 'w10sa_kernel_rem_pio2f.o', [], 'authored_cpp'),
    ('w10-startup-a-ps2-klibinfo', 'klibinfo.cpp', 'w10sa_ps2_klibinfo.o', [], 'authored_cpp'),
    ('w10-startup-a-throw', 'throw.cpp', 'w10sa_throw.o', [], 'authored_cpp'),
    # other package: defines _ctype_, absent from the shared plan
    ('w8-startup-sce-a-ctype-table', 'ctype_table.cpp', 'w10sa_other_ctype_table.o', [], 'authored_cpp'),
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
    prefix = plan['toolchain']['target']
    tc = Path(toolchain).resolve()
    out = []
    with tempfile.TemporaryDirectory(prefix='w10sa-plan-') as scratch:
        for directory, source, object_name, headers, profile_name in UNITS:
            profile = plan['profiles'][profile_name]
            flags = [f.replace('{toolchain}', str(tc)) for f in profile['flags']]
            rel = 'sources/%s/%s' % (directory, source)
            obj = Path(scratch) / object_name
            subprocess.check_call([str(tc / 'bin' / (prefix + '-' + profile['driver'])), *flags, '-c', str(EXP / rel),
                                   '-o', str(obj)], cwd=scratch)
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
