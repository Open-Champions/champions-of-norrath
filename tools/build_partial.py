#!/usr/bin/env python3
"""Build the selected authored snapshot into an unfinished EE relocatable object.

No downloading, original game files, fallback objects, stubs or executable link.
Use --check-plan for file-only validation; a build requires an explicit toolchain.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import struct
import subprocess
import sys
import memory_binding


def digest(path):
    h = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            h.update(block)
    return h.hexdigest()


def checked_path(root, relative):
    p = Path(relative)
    if p.is_absolute() or '..' in p.parts:
        raise ValueError('Expected a contained relative path: ' + relative)
    result = (root / p).resolve()
    result.relative_to(root)
    return result


def verify(root, hashes):
    for relative, expected in hashes.items():
        path = checked_path(root, relative)
        if not path.is_file() or digest(path) != expected:
            raise ValueError('File identity mismatch: ' + relative)


def save(path, data):
    path.write_text(json.dumps(data, indent=2, sort_keys=True) + '\n')


def validate_plan(root, plan):
    if plan['schema'] != 1:
        raise ValueError('Unsupported plan schema')
    snapshot_path = root / 'snapshot.json'
    if digest(snapshot_path) != plan['snapshot_sha256']:
        raise ValueError('Snapshot changed; derive and review a successor plan')
    snapshot = json.loads(snapshot_path.read_text())
    if len(plan['inputs']) != len(snapshot['inputs']):
        raise ValueError('Input coverage mismatch')
    expected_files = set()
    authored = 0
    for position, (item, original) in enumerate(zip(plan['inputs'], snapshot['inputs'])):
        if item['input_order'] != position or original['input_order'] != position:
            raise ValueError('Input order mismatch')
        for key in ('kind', 'object_name'):
            if item[key] != original[key]:
                raise ValueError('Snapshot input mismatch: ' + key)
        if item['kind'] == 'independently_authored_source':
            authored += 1
            if item['source'] != original['source'] or item['sha256'] != snapshot['files'][item['source']]:
                raise ValueError('Authored source mismatch')
            stack, reached = [item['source']], set()
            while stack:
                name = stack.pop()
                if name in reached:
                    continue
                reached.add(name)
                path = checked_path(root, name)
                text = path.read_text()
                if re.search(r'^\s*#\s*include\s*<', text, re.M):
                    raise ValueError('Unreviewed system include: ' + name)
                for include in re.findall(r'^\s*#\s*include\s*"([^"]+)"', text, re.M):
                    target = (path.parent / include).resolve()
                    stack.append(target.relative_to(root).as_posix())
            if reached != {item['source'], *item['header_closure']}:
                raise ValueError('Quoted include closure mismatch: ' + item['source'])
            expected_files.update(reached)
        elif item['kind'] == 'external_upstream_source':
            if item['source'] != original['upstream_file'] or item['sha256'] != original['source_sha256']:
                raise ValueError('External source mismatch')
        else:
            raise ValueError('Unreviewed input kind')
    if authored != plan['default_authored_inputs'] or len(plan['inputs']) - authored != plan['optional_external_inputs']:
        raise ValueError('Input totals mismatch')
    if expected_files != set(plan['files']):
        raise ValueError('Source closure contains omitted or additional files')
    for name, sha in plan['files'].items():
        if snapshot['files'].get(name) != sha:
            raise ValueError('Source closure hash mismatch: ' + name)
    verify(root, plan['files'])
    if plan['symbol_aliases'] or plan['link_flags'] != ['-m', 'elf32lr5900', '-r', '--warn-common']:
        raise ValueError('This recipe permits no symbol aliases, absolute address imports or executable link')
    return snapshot


def elf_inventory(path):
    """Read actual ELF32 symbol/section inventories without host binutils."""
    data = path.read_bytes()
    if data[:7] != b'\x7fELF\x01\x01\x01' or len(data) < 52:
        raise ValueError('Expected little-endian ELF32: ' + str(path))
    hdr = struct.unpack_from('<HHIIIIIHHHHHH', data, 16)
    if hdr[0] != 1 or hdr[1] != 8 or hdr[10] != 40 or not hdr[11]:
        raise ValueError('Expected non-extended ET_REL EM_MIPS section table')
    def take(offset, size):
        if offset < 0 or size < 0 or offset + size > len(data):
            raise ValueError('ELF file extent out of bounds')
        return data[offset:offset + size]
    sections = [struct.unpack('<10I', take(hdr[5] + i * 40, 40)) for i in range(hdr[11])]
    names = take(sections[hdr[12]][4], sections[hdr[12]][5])
    def string(table, offset):
        if offset >= len(table):
            raise ValueError('ELF string offset out of bounds')
        end = table.find(b'\0', offset)
        if end < 0:
            raise ValueError('Unterminated ELF string')
        return table[offset:end].decode('utf-8', 'strict')
    allocated, defined, undefined, fields = [], [], set(), 0
    abi = None
    for section in sections:
        name = string(names, section[0])
        if section[1] != 8:
            payload = take(section[4], section[5])
        if section[2] & 2:
            allocated.append({'name': name, 'type': section[1], 'size': section[5], 'flags': section[2], 'alignment': section[8]})
        if name == '.MIPS.abiflags':
            if len(payload) != 24:
                raise ValueError('Unexpected MIPS ABI record')
            abi = dict(zip(('version', 'isa_level', 'isa_rev', 'gpr_size', 'cpr1_size', 'cpr2_size', 'fp_abi', 'isa_ext', 'ases', 'flags1', 'flags2'), struct.unpack('<H6B4I', payload)))
        if section[1] == 9:
            if section[5] % 8:
                raise ValueError('Malformed relocation table')
            fields += section[5] // 8
        if section[1] == 2:
            strings = sections[section[6]]
            strings = take(strings[4], strings[5])
            if section[9] != 16 or section[5] % 16:
                raise ValueError('Unexpected ELF symbol table shape')
            for offset in range(0, len(payload), 16):
                nameoff, value, size, info, other, shndx = struct.unpack_from('<IIIBBH', payload, offset)
                symbol = string(strings, nameoff)
                if shndx == 0 and symbol and info >> 4 in (1, 2):
                    undefined.add(symbol)
                if shndx and info & 15 == 2:
                    defined.append({'name': symbol, 'compiled_body_bytes': size,
                                    'section_index': shndx, 'value': value, 'binding': info >> 4})
    if not abi or (abi['gpr_size'], abi['cpr1_size'], abi['fp_abi']) != (2, 1, 2):
        raise ValueError('Unexpected GPR64/FP32/single-float ABI')
    return {'sha256': digest(path), 'functions': defined, 'function_count': sum(x['compiled_body_bytes'] > 0 for x in defined), 'function_symbol_count': len(defined), 'function_body_bytes': sum(x['compiled_body_bytes'] for x in defined), 'undefined_symbols': sorted(undefined), 'relocation_fields': fields, 'allocated_sections': allocated, 'abi': abi}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', type=Path, required=True, help='Published experimental directory')
    parser.add_argument('--plan', type=Path, required=True, help='Reviewed build-plan.json')
    parser.add_argument('--check-plan', action='store_true', help='Check source hashes/includes only; launch no target tools')
    parser.add_argument('--toolchain', type=Path, help='Explicit reviewed EE installation prefix')
    parser.add_argument('--newlib-root', type=Path, help='Optional supplied newlib-1.10.0 root; never fetched')
    parser.add_argument('--bind-memset', action='store_true',
                        help='Bind only the three reviewed compiler memset calls to the authored provider')
    parser.add_argument('--output', type=Path, help='New output directory; existing directories are rejected')
    args = parser.parse_args()
    root = args.source_root.resolve()
    plan = json.loads(args.plan.read_text())
    snapshot = validate_plan(root, plan)
    newlib = args.newlib_root.resolve() if args.newlib_root else None
    if newlib:
        verify(newlib, plan['external_files'])
    selected = [x for x in plan['inputs'] if newlib or x['kind'] == 'independently_authored_source']
    if args.check_plan:
        if args.bind_memset:
            parser.error('--bind-memset requires an actual build to verify emitted provider and consumers')
        print(json.dumps({'status': 'source_plan_valid', 'selected_inputs': len(selected), 'source_and_header_files': len(plan['files']), 'target_tools_executed': 0, 'build_verified': False}))
        return
    if not args.toolchain or not args.output:
        parser.error('Build requires --toolchain and --output; use --check-plan for file-only checks')
    tc = args.toolchain.resolve()
    identity = plan['toolchain']
    verify(tc, identity['binary_hashes'])
    output = args.output.resolve()
    if output == root or root in output.parents:
        raise ValueError('Place generated outputs outside the published experimental sources')
    output.mkdir(parents=True, exist_ok=False)
    env = {'PATH': str(tc / 'bin') + os.pathsep + '/usr/bin:/bin', 'LANG': 'C', 'LC_ALL': 'C'}
    commands = []
    status = {'status': 'in_progress', 'source_cc_count': 0, 'implicit_assembler_count': 0, 'link_count': 0, 'metadata_query_count': 0, 'snapshot_sha256': plan['snapshot_sha256'], 'plan_sha256': digest(args.plan), 'selected_inputs': len(selected), 'original_files_used': False, 'runtime_verified': False, 'game_executable': False}
    def run(argv, kind, cwd=output, stdin=None):
        index = len(commands)
        row = {'argv': list(map(str, argv)), 'cwd': str(cwd), 'kind': kind, 'returncode': None}
        commands.append(row)
        if kind == 'source_compile':
            status['source_cc_count'] += 1
        elif kind == 'relocatable_link':
            status['link_count'] += 1
        else:
            status['metadata_query_count'] += 1
        save(output / 'commands.json', commands)
        save(output / 'build-status.json', status)
        proc = subprocess.run(row['argv'], cwd=cwd, env=env, input=stdin, capture_output=True)
        (output / ('command-%03d.stdout' % index)).write_bytes(proc.stdout)
        (output / ('command-%03d.stderr' % index)).write_bytes(proc.stderr)
        row['returncode'] = proc.returncode
        save(output / 'commands.json', commands)
        if proc.returncode == 0 and kind == 'source_compile':
            status['implicit_assembler_count'] += 1
        if proc.returncode:
            raise RuntimeError('Command failed; output preserved and no retry made: %d' % index)
        return proc.stdout
    save(output / 'build-plan.json', plan)
    save(output / 'snapshot.json', snapshot)
    try:
        prefix = identity['target']
        cpp = tc / 'bin' / (prefix + '-g++')
        for name, expected in [('cc1plus', identity['binary_hashes']['libexec/gcc/' + prefix + '/15.2.0/cc1plus']), ('as', identity['binary_hashes']['bin/' + prefix + '-as'])]:
            path = Path(run([cpp, '-print-prog-name=' + name], 'tool_identity').decode().strip()).resolve()
            path.relative_to(tc)
            if digest(path) != expected:
                raise ValueError('Compiler resolves an unreviewed subtool: ' + name)
        if hashlib.sha256(run([cpp, '-dumpspecs'], 'tool_identity')).hexdigest() != identity['specs_sha256']:
            raise ValueError('Compiler specs changed')
        flags = plan['profiles']['authored_cpp']['flags']
        macros_text = run([cpp, *flags, '-dM', '-E', '-'], 'macro_identity', stdin=b'').decode()
        macros = dict(re.findall(r'^#define (\S+) (.*)$', macros_text, re.M))
        for key, expected in identity['float_model'].items():
            if macros.get(key) != expected:
                raise ValueError('Floating-point model mismatch: ' + key)
        if newlib:
            include = tc / 'lib/gcc' / prefix / '15.2.0/include'
            verify(include, identity['builtin_headers'])
            c = tc / 'bin' / (prefix + '-gcc')
            cc1 = Path(run([c, '-print-prog-name=cc1'], 'tool_identity').decode().strip()).resolve()
            cc1.relative_to(tc)
            if digest(cc1) != identity['binary_hashes']['libexec/gcc/' + prefix + '/15.2.0/cc1']:
                raise ValueError('Unreviewed C frontend')
        objects, inventories = [], []
        for item in selected:
            order = item['input_order']
            unit = output / ('%03d' % order)
            unit.mkdir()
            external = item['kind'] == 'external_upstream_source'
            source_base = newlib if external else root
            if external:
                verify(newlib, plan['external_files'])
            else:
                verify(root, {k: plan['files'][k] for k in [item['source'], *item['header_closure']]})
            source = checked_path(source_base, item['source'])
            obj = unit / item['object_name']
            profile = plan['profiles'][item['profile']]
            flags = [x.replace('{toolchain}', str(tc)).replace('{newlib}', str(newlib)) for x in profile['flags']]
            driver = tc / 'bin' / (prefix + '-' + profile['driver'])
            run([driver, *flags, '-save-temps=obj', '-c', source, '-o', obj], 'source_compile', cwd=unit)
            inventory = elf_inventory(obj)
            expected = snapshot['inputs'][order]['emitted_functions']
            key = lambda x: (x['name'], x['compiled_body_bytes'])
            if sorted(map(key, inventory['functions'])) != sorted(map(key, expected)):
                save(unit / 'inventory.json', inventory)
                raise ValueError('Generated function inventory differs; review required: ' + item['source'])
            if external:
                preprocessed = obj.with_suffix('.i')
                for name in re.findall(r'^#\s+\d+\s+"([^"]+)"', preprocessed.read_text(), re.M):
                    if name.startswith('<'):
                        continue
                    p = Path(name).resolve()
                    try:
                        relative = p.relative_to(newlib).as_posix()
                        hashes = plan['external_files']
                    except ValueError:
                        relative = p.relative_to(tc / 'lib/gcc' / prefix / '15.2.0/include').as_posix()
                        hashes = identity['builtin_headers']
                    if hashes.get(relative) != digest(p):
                        raise ValueError('Unreviewed preprocessor dependency: ' + name)
            inventory['input_order'] = order
            save(unit / 'inventory.json', inventory)
            inventories.append(inventory)
            objects.append(obj)
        verify(root, plan['files'])
        if newlib:
            verify(newlib, plan['external_files'])
        result = output / 'game-components-partial.o'
        binding_flags = []
        if args.bind_memset:
            binding = memory_binding.validate_inputs(inventories)
            save(output / 'memory-binding.json', binding)
            binding_flags = [binding['link_flag']]
        run([tc / 'bin' / (prefix + '-ld'), *plan['link_flags'], *binding_flags, '-Map=' + str(output / 'game-components.map'), '-o', result, *objects], 'relocatable_link')
        linked = elf_inventory(result)
        if args.bind_memset:
            memory_binding.validate_output(linked)
        if linked['function_body_bytes'] != sum(x['function_body_bytes'] for x in inventories):
            raise ValueError('Partial link changed total function extents; review required')
        verify(tc, identity['binary_hashes'])
        save(output / 'object-inventories.json', inventories)
        save(output / 'partial-inventory.json', linked)
        save(output / 'unresolved-imports.json', linked['undefined_symbols'])
        status.update(status='partial_build_complete', function_count=linked['function_count'], unresolved_symbol_count=len(linked['undefined_symbols']), symbol_aliases_applied=args.bind_memset)
        print(json.dumps(status))
    except BaseException as exc:
        status.update(status='failed_preserved_no_retry', error=str(exc))
        raise
    finally:
        save(output / 'build-status.json', status)
        save(output / 'artifact-hashes.json', {p.relative_to(output).as_posix(): digest(p) for p in sorted(output.rglob('*')) if p.is_file() and p.name != 'artifact-hashes.json'})


if __name__ == '__main__':
    main()
