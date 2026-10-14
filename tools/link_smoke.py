#!/usr/bin/env python3
"""Fresh, bounded GNU linkage of admitted EE state consumers and owners.

This integration check awards no reconstruction credit and produces no runnable
game. All generated objects, ELF files and logs must remain private.
"""
import argparse
from datetime import datetime, timezone
import json
import os
from pathlib import Path
import struct
import subprocess
import sys

import rebuild as r

GROUP = {
    'light_disable_volumetric_lighting': ('disable_volumetric_lighting_state', r.SCALAR_V2_MODE, 12, 4),
    'light_disable_local_lights': ('interface_mode_state', r.SCALAR_V2_MODE, 16, 4),
    'light_global_char_light_scale': ('global_char_light_scale_state', r.FLOAT_MODE, 8, 4),
    'world_set_half_frustum_mode': ('half_frustum_mode_state', r.BYTE_MODE, 8, 1),
}
FUNCTION_IDS = frozenset(GROUP)
SHIFT = 0x100


def select_group(ledger):
    functions = [i for i in ledger['functions'] if i['id'] in FUNCTION_IDS]
    r.require(len(functions) == len(FUNCTION_IDS), 'missing or duplicate smoke function')
    r.require(all(i.get('toolchain') == 'ee-gcc' and i.get('comparison_mode') in r.DATA_MODES
                  for i in functions), 'smoke group requires admitted EE data consumers')
    owner_ids = {i['data_binding']['owner_id'] for i in functions}
    owners = [i for i in ledger['data_objects'] if i['id'] in owner_ids]
    r.require(len(owners) == len(owner_ids) == 4, 'smoke group requires four distinct owners')
    r.require(len({i['symbol'] for i in functions + owners}) == 8, 'duplicate group symbol')
    by_id = {i['id']: i for i in owners}
    for item in functions:
        owner_id, mode, code_size, data_size = GROUP[item['id']]
        r.require(item['data_binding']['owner_id'] == owner_id and item['comparison_mode'] == mode
                  and item['size'] == code_size and by_id[owner_id]['size'] == data_size,
                  'smoke group contract changed; review tool scope explicitly')
    return functions, owners


def linker_script(functions, owners, shift=0):
    gp = r.address(r.GP_SPEC['gp_symbol']['address'])
    lines = ['OUTPUT_ARCH(mips:5900)', 'ENTRY(' + functions[0]['symbol'] + ')',
             'SECTIONS {', '  _gp = 0x%x;' % gp]
    for item in sorted(functions, key=lambda i: r.address(i['address'])):
        lines.append('  .text.%s 0x%x : { %s.o(.text) }' %
                     (item['id'], r.address(item['address']), item['id']))
    for item in sorted(owners, key=lambda i: r.address(i['address'])):
        location = r.address(item['address']) + shift
        r.require(0 <= location < 2**32 and -32768 <= location - gp <= 32767,
                  'shifted owner exceeds GP relocation range')
        r.require(location % item['size'] == 0, 'shifted owner is misaligned')
        lines.append('  .sdata.%s 0x%x : { %s.o(.sdata) }' %
                     (item['id'], location, item['id']))
    lines += ['  .MIPS.abiflags 0x00500000 : { *(.MIPS.abiflags) }',
              '  /DISCARD/ : { *(.comment) *(.note.GNU-stack) *(.gnu.attributes)',
              '    *(.data) *(.bss) *(.text) *(.sdata) }', '}']
    # Discarded input payload is independently rejected by rebuild's object
    # validators before ld runs. No unresolved symbol is supplied by this script.
    return '\n'.join(lines) + '\n'


def shifted_code(code, offsets, displacement):
    r.require(-32768 <= displacement <= 32767, 'GP relocation overflow')
    result = bytearray(code)
    for offset in offsets:
        r.require(type(offset) is int and offset % 4 == 0 and 0 <= offset <= len(code) - 4,
                  'invalid relocation offset')
        word, = struct.unpack_from('<I', result, offset)
        struct.pack_into('<I', result, offset, (word & 0xffff0000) | (displacement & 0xffff))
    return bytes(result)


def linked_owner(elf, owner, shift):
    symbols = [s for s in elf.symbols if s['name'] == owner['symbol']]
    r.require(len(symbols) == 1, 'missing or ambiguous linked owner')
    symbol = symbols[0]
    r.require(symbol['type'] == 1 and symbol['binding'] == 1 and symbol['other'] == 0
              and symbol['value'] == r.address(owner['address']) + shift
              and symbol['size'] == owner['size'], 'linked owner identity differs')
    r.require(0 < symbol['section'] < len(elf.sections), 'linked owner is not defined')
    section = elf.sections[symbol['section']]
    start = symbol['value'] - section[3]
    r.require(section[1] == 1 and section[2] & 7 == 3 and start >= 0
              and start + symbol['size'] <= section[5], 'linked owner storage differs')
    contents = elf.section_data(symbol['section'])[start:start + symbol['size']]
    r.require(contents == r.owner_initializer(owner), 'linked initializer differs')
    return contents


def check_consumer_object(elf, item, original, owner):
    r.require(elf.flags == r.BASELINE_FLAGS, 'consumer object flags differ')
    if item['comparison_mode'] in (r.FLOAT_MODE, r.BYTE_MODE):
        r.float_abi(elf)
    raw, _ = elf.rebuilt(item['symbol'], scalar_binding=item['data_binding'], scalar_mode=item['comparison_mode'])
    relocated, _ = r.relocate_scalar(raw, original, item, owner)
    r.require(relocated == original and len(relocated) == item['size'], 'fresh complete function mismatch')


def check_linked(elf, functions, owners, originals, shift=0):
    r.require(elf.kind == 2 and elf.flags == r.BASELINE_FLAGS, 'unexpected linked ELF type or flags')
    r.require(not any(s['name'] and s['section'] in (0, 0xfff2) for s in elf.symbols),
              'linked ELF contains undefined or COMMON symbols')
    r.require(not any(s[1] in (4, 9) and s[5] for s in elf.sections),
              'linked ELF contains unresolved relocations')
    gp = r.address(r.GP_SPEC['gp_symbol']['address'])
    gp_symbols = [s for s in elf.symbols if s['name'] == '_gp']
    r.require(len(gp_symbols) == 1 and gp_symbols[0]['value'] == gp
              and gp_symbols[0]['section'] == 0xfff1, 'linked GP differs')
    by_id = {i['id']: i for i in owners}
    records = []
    for item in functions:
        symbol, code = elf.function(item['symbol'])
        r.require(symbol['value'] == r.address(item['address']) and symbol['size'] == item['size']
                  and symbol['binding'] == 1 and symbol['other'] == 0, 'linked function identity differs')
        owner = by_id[item['data_binding']['owner_id']]
        offsets = r.scalar_offsets(item['data_binding']['operation'], item['comparison_mode'])
        displacement = r.address(owner['address']) + shift - gp
        expected = shifted_code(originals[item['id']], offsets, displacement)
        r.require(code == expected, 'complete linked function differs')
        r.require(r.scalar_body(code, item['data_binding']['operation'], mode=item['comparison_mode'])
                  == displacement, 'linked function does not address its source owner')
        r.require(not shift or code != originals[item['id']], 'relocation sensitivity control did not change code')
        records.append({'id': item['id'], 'size': len(code), 'sha256': r.digest(code),
                        'owner_id': owner['id'], 'fixups': len(offsets)})
    for owner in owners:
        linked_owner(elf, owner, shift)
    allowed = {'.text.' + i['id'] for i in functions} | {'.sdata.' + i['id'] for i in owners}
    for name, section in zip(elf.names, elf.sections):
        if section[2] & 2 and section[5] and name not in allowed:
            r.require(name == '.MIPS.abiflags' and section[1] == 0x7000002a and section[5] == 24,
                      'unexpected allocated linked payload')
    return records


def run(game, compiler, linker, output_dir):
    ledger = r.load_ledger()
    functions, owners = select_group(ledger)
    baseline = r.baseline_elf(game)
    context = r.gp_context_evidence(baseline, r.ROOT)
    original_owners = {i['id']: r.scalar_owner_original(baseline, i) for i in owners}
    originals = {i['id']: r.original_graph_body(baseline, i, strict_intersections=True) for i in functions}
    for item in functions:
        r.require(baseline.original(item) == originals[item['id']], 'original extent differs')
    reviews = {i['id']: r.scalar_review(ledger, i, r.ROOT) for i in owners}
    for name in r.INJECTING_ENV:
        r.require(not os.environ.get(name), 'unset compiler environment variable ' + name)
    env = {'PATH': os.defpath, 'LANG': 'C', 'LC_ALL': 'C'}
    executable, identity = r.toolchain_identity('ee-gcc', compiler, env)
    linker = Path(linker).resolve()
    r.require(linker == (executable.parent / (r.EE_TARGET + '-ld')).resolve(),
              'linker must belong to the selected EE installation')
    linker_hash = r.digest(linker.read_bytes())
    version = r.compiler_output([str(linker), '--version'], env).decode('ascii').splitlines()[0]
    r.require(version.startswith('GNU ld '), 'GNU linker required')
    directory = Path(output_dir).resolve()
    r.require(directory.is_relative_to(r.ROOT / 'work'), 'private output must be inside ignored work/')
    directory.mkdir(parents=True, exist_ok=False)
    snapshots = {i['id']: r.source_bytes(r.local_path(r.ROOT, i['source'])) for i in functions + owners}
    objects = {}
    by_id = {i['id']: i for i in owners}
    for item in functions + owners:
        ident = item['id']
        snapshot, obj = directory / (ident + '.cpp'), directory / (ident + '.o')
        snapshot.write_bytes(snapshots[ident])
        r.ee_data_model(executable, item['compiler_args'], env)
        if item.get('comparison_mode') == r.FLOAT_MODE or r.float_owner(item):
            r.ee_data_model(executable, item['compiler_args'], env, r.FLOAT_MODEL)
        if item.get('comparison_mode') == r.BYTE_MODE or r.byte_owner(item):
            r.ee_data_model(executable, item['compiler_args'], env, r.BYTE_MODEL)
        command = [str(executable)] + item['compiler_args'] + r.FIXED_FLAGS + ['-c', snapshot.name, '-o', obj.name]
        process = subprocess.run(command, cwd=directory, env=env, capture_output=True, timeout=120)
        (directory / (ident + '.log')).write_bytes(process.stdout + process.stderr)
        r.require(process.returncode == 0, 'fresh compilation failed; inspect private log')
        elf = r.Elf32(obj.read_bytes())
        if ident in by_id:
            r.scalar_owner_rebuilt(elf, item)
        else:
            check_consumer_object(elf, item, originals[ident], by_id[item['data_binding']['owner_id']])
        objects[ident] = {'source_sha256': r.digest(snapshots[ident]), 'object_sha256': r.digest(obj.read_bytes()),
                          'compiler_args': r.effective_args(item)}
    links = []
    for name, shift in (('original', 0), ('shifted', SHIFT)):
        script = directory / (name + '.ld')
        script.write_text(linker_script(functions, owners, shift))
        args = ['-m', 'elf32lr5900', '--fatal-warnings', '--no-undefined', '-T', script.name, '-o', name + '.elf']
        command = [str(linker)] + args + [i['id'] + '.o' for i in functions + owners]
        process = subprocess.run(command, cwd=directory, env=env, capture_output=True, timeout=120)
        (directory / (name + '.log')).write_bytes(process.stdout + process.stderr)
        r.require(process.returncode == 0, 'GNU link failed; inspect private log')
        contents = (directory / (name + '.elf')).read_bytes()
        records = check_linked(r.Elf32(contents), functions, owners, originals, shift)
        links.append({'name': name, 'data_shift': shift, 'elf_sha256': r.digest(contents),
                      'script_sha256': r.digest(script.read_bytes()), 'linker_args': args,
                      'functions': records, 'undefined_symbols': 0, 'remaining_relocations': 0})
    r.require(r.toolchain_identity('ee-gcc', executable, env)[1] == identity
              and r.digest(linker.read_bytes()) == linker_hash, 'build tools changed during run')
    r.require(all(r.source_bytes(r.local_path(r.ROOT, i['source'])) == snapshots[i['id']]
                  and (directory / (i['id'] + '.cpp')).read_bytes() == snapshots[i['id']]
                  for i in functions + owners), 'sources changed during run')
    r.require(r.load_ledger() == ledger and r.gp_descriptor(r.ROOT) == context['context_sha256']
              and all(r.scalar_review(ledger, i, r.ROOT) == reviews[i['id']] for i in owners),
              'admission records changed during run')
    report = {'schema_version': 1, 'status': 'passed', 'scope': 'four-admitted-ee-state-consumers-link-smoke',
              'recorded_at': datetime.now(timezone.utc).isoformat(), 'target': r.TARGET,
              'tool_sha256': r.digest(Path(__file__).read_bytes()), 'verifier_sha256': r.digest(Path(r.__file__).read_bytes()),
              'ledger_sha256': r.digest(r.canonical(ledger)), 'compiler': identity,
              'linker': {'version': version, 'sha256': linker_hash}, 'gp_context': context,
              'owner_reviews': reviews, 'original_owners': original_owners, 'objects': objects, 'links': links,
              'existing_code_bytes': sum(i['size'] for i in functions),
              'existing_data_bytes': sum(i['size'] for i in owners), 'new_reconstruction_credit': 0,
              'limits': ['No target execution or startup.', 'No whole-game ABI or linkage proof.',
                         'Original local data use explicit external comparison adapters.',
                         'Shifted placement is a relocation control, not a retail byte match.']}
    (directory / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
    return report


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game', required=True)
    parser.add_argument('--ee-compiler', required=True)
    parser.add_argument('--linker', required=True)
    parser.add_argument('--output-dir', required=True, help='new directory under ignored work/')
    args = parser.parse_args(argv)
    try:
        report = run(args.game, args.ee_compiler, args.linker, args.output_dir)
    except (OSError, ValueError, subprocess.TimeoutExpired) as exc:
        print('Link smoke failed: ' + str(exc), file=sys.stderr)
        return 1
    print('Passed: 4 functions, %d existing code bytes, 4 owners, %d existing data bytes; '
          '5 fixups in each of 2 GNU links; no new reconstruction credit.' %
          (report['existing_code_bytes'], report['existing_data_bytes']))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
