"""Synthetic, game-free tests of Norrath's intentionally narrow verification policy."""
import contextlib
import copy
import json
import os
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

import rebuild

CODE = b'\x01\x02\x03\x04' * 2
BASE = 0x100000
CALLER = struct.pack('<8I', 0x27bdfff8, 0xffbf0000, 0x0c000000, 0, 0xdfbf0000, 0x24420001, 0x03e00008, 0x27bd0008)
CALLEE = struct.pack('<2I', 0x03e00008, 0x2402000c)
LINKED_CALLER = CALLER[:8] + struct.pack('<I', 0x0c040008) + CALLER[12:]


def call_elf_fixture(kind=1, code=CALLER, functions=None, relocations=((8, 0x204),)):
    """Independent synthetic ELF with load mapping, undefined import and both relocation tables."""
    if functions is None:
        functions = [('caller', 0, len(code), 2, 1, 0, 1), ('dep', 0, 0, 0, 1, 0, 0)]
    strings, syms = bytearray(b'\0'), bytearray(16)
    for name, value, size, typ, binding, other, section in functions:
        at = len(strings); strings.extend(name.encode() + b'\0')
        syms.extend(struct.pack('<IIIBBH', at, value, size, binding << 4 | typ, other, section))
    rows = [('', b'', 0, 0, 0, 0, 0, 0),
            ('.text', code, 1, 6, BASE if kind == 2 else 0, 0, 0, 0),
            ('.symtab', syms, 2, 0, 0, 3, 1, 16),
            ('.strtab', strings, 3, 0, 0, 0, 0, 0),
            ('.pdr', bytes(32), 1, 0, 0, 0, 0, 0),
            ('.rel.pdr', struct.pack('<II', 0, 0x102) if kind == 1 else b'', 9, 0x40, 0, 2, 4, 8),
            ('.rel.text', b''.join(struct.pack('<II', *r) for r in relocations) if kind == 1 else b'', 9, 0x40, 0, 2, 1, 8),
            ('.data', b'', 1, 3, 0, 0, 0, 0)]
    names = b'\0' + b''.join(row[0].encode() + b'\0' for row in rows[1:]) + b'.shstrtab\0'
    rows.append(('.shstrtab', names, 3, 0, 0, 0, 0, 0))
    data, sections = bytearray(84 if kind == 2 else 52), []
    for name, blob, typ, flags, addr, link, info, entry in rows:
        data.extend(bytes(-len(data) % 4)); offset = len(data); data.extend(blob)
        sections.append((names.index(name.encode()+b'\0'), typ, flags, addr, offset, len(blob), link, info, 4, entry))
    data.extend(bytes(-len(data) % 4)); shoff = len(data)
    for row in sections: data.extend(struct.pack('<10I', *row))
    data[:16] = b'\x7fELF\x01\x01\x01' + bytes(9)
    struct.pack_into('<HHIIIIIHHHHHH', data, 16, kind, 8, 1, rebuild.BASELINE_ENTRY,
                     52 if kind == 2 else 0, shoff, rebuild.BASELINE_FLAGS, 52, 32 if kind == 2 else 0,
                     1 if kind == 2 else 0, 40, len(sections), 8)
    if kind == 2: struct.pack_into('<8I', data, 52, 1, sections[1][4], BASE, BASE, len(code), len(code), 5, 4)
    return bytes(data)


def change_symbol(data, name, **changes):
    elf = rebuild.Elf32(data)
    symbol, = [s for s in elf.symbols if s['name'] == name]
    symbol.update(changes)
    offset = elf.sections[symbol['table']][4] + symbol['index'] * 16
    data = bytearray(data)
    struct.pack_into('<IIBBH', data, offset + 4, symbol['value'], symbol['size'],
                     symbol['binding'] << 4 | symbol['type'], symbol['other'], symbol['section'])
    return bytes(data)


def elf_fixture(kind=1, code=CODE, symbol_size=None, symbol_type=2, extra_function=False,
                relocation=False, pdr=False, allocated_data=False, undefined=False,
                symbol=None, extra_value=None, extra_size=4):
    """ELF data assembled independently from the parser; never uses original bytes."""
    name = symbol or ('original' if kind == 2 else 'candidate')
    strings = b'\0' + name.encode() + b'\0other\0'
    syms = b'\0' * 16 + struct.pack('<IIIBBH', 1, BASE if kind == 2 else 0,
                                   len(code) if symbol_size is None else symbol_size,
                                   0x10 | symbol_type, 0, 1)
    if extra_function or undefined:
        syms += struct.pack('<IIIBBH', len(name) + 2, (BASE if kind == 2 else 0) if extra_value is None else extra_value, extra_size, 0x12, 0, 0 if undefined else 1)
    rows = [('', b'', 0, 0, 0, 0, 0, 0),
            ('.text', code, 1, 6, BASE if kind == 2 else 0, 0, 0, 0),
            ('.symtab', syms, 2, 0, 0, 3, 1, 16),
            ('.strtab', strings, 3, 0, 0, 0, 0, 0),
            ('.pdr', b'\0' * 32 if pdr else b'', 1, 0, 0, 0, 0, 0),
            ('.rel.pdr' if pdr else '.rel.text', struct.pack('<II', 0, 0x102) if pdr or relocation else b'', 9, 0, 0, 2, 4 if pdr else 1, 8),
            ('.data', b'\0' * 4 if allocated_data else b'', 1, 3, 0, 0, 0, 0)]
    names = b'\0' + b''.join(row[0].encode() + b'\0' for row in rows[1:]) + b'.shstrtab\0'
    rows.append(('.shstrtab', names, 3, 0, 0, 0, 0, 0))
    data = bytearray(52)
    sections = []
    for name, blob, typ, flags, addr, link, info, entry_size in rows:
        data.extend(b'\0' * (-len(data) % 4))
        offset = len(data)
        data.extend(blob)
        sections.append((names.index(name.encode() + b'\0'), typ, flags, addr, offset, len(blob), link, info, 4, entry_size))
    data.extend(b'\0' * (-len(data) % 4))
    shoff = len(data)
    for row in sections:
        data.extend(struct.pack('<10I', *row))
    data[:16] = b'\x7fELF\x01\x01\x01' + b'\0' * 9
    struct.pack_into('<HHIIIIIHHHHHH', data, 16, kind, 8, 1, rebuild.BASELINE_ENTRY,
                     0, shoff, rebuild.BASELINE_FLAGS, 52, 0, 0, 40, len(sections), 7)
    return bytes(data)


def change_section(data, name, field, value):
    elf = rebuild.Elf32(data)
    index, _ = elf.named_section(name)
    data = bytearray(data)
    shoff = struct.unpack_from('<I', data, 32)[0]
    struct.pack_into('<I', data, shoff + index * 40 + field * 4, value)
    return bytes(data)


class RebuildTests(unittest.TestCase):
    def setUp(self):
        # Synthetic compiler tests own their environment. Hosted Python may set
        # LD_LIBRARY_PATH; injection cases explicitly override this clean baseline.
        clean_environment = patch.dict(os.environ, {name: '' for name in rebuild.INJECTING_ENV})
        clean_environment.start()
        self.addCleanup(clean_environment.stop)
        self.temp = tempfile.TemporaryDirectory(prefix='norrath verifier ')
        self.root = Path(self.temp.name)
        self.source = self.root / 'candidate.cpp'
        self.source.write_text('extern "C" int candidate(void*, int) { return 12; }\n')
        self.game = self.root / 'game.elf'
        self.game.write_bytes(elf_fixture(kind=2))
        self.compiler = self.root / 'clang-test'
        self.compiler.write_bytes(b'synthetic compiler')
        self.target = copy.deepcopy(rebuild.TARGET)
        self.target['baseline_sha256'] = rebuild.digest(self.game.read_bytes())
        self.target['text']['size'] = 8
        self.target_patch = patch.object(rebuild, 'TARGET', self.target)
        self.size_patch = patch.object(rebuild, 'BASELINE_SIZE', self.game.stat().st_size)
        self.target_patch.start()
        self.size_patch.start()
        self.addCleanup(self.target_patch.stop)
        self.addCleanup(self.size_patch.stop)
        self.addCleanup(self.temp.cleanup)
        self.ledger = {'schema_version': 1, 'target': copy.deepcopy(self.target), 'functions': [{
            'id': 'fixture', 'address': '0x00100000', 'size': 8, 'original_symbol': 'original',
            'source': 'candidate.cpp', 'symbol': 'candidate', 'language': 'c++',
            'compiler_args': sorted(rebuild.REQUIRED_FLAGS) + ['-O2']}]}
        self.ledger_path = self.root / 'rebuild.json'
        self.ledger_path.write_text(json.dumps(self.ledger))
        self.output = self.root / 'private output'

    @contextlib.contextmanager
    def compiler_fixture(self, object_data=None, exit_code=0, mutation=None, no_object=False):
        calls = []
        def run(command, **kwargs):
            calls.append(command)
            self.assertNotIn('shell', kwargs)
            self.assertEqual(kwargs['env'], {'PATH': os.defpath, 'LANG': 'C', 'LC_ALL': 'C'})
            if command[-1] == '--version':
                return subprocess.CompletedProcess(command, 0, b'Homebrew clang version 20.1.8\nInstalledDir: /private/path\n', b'')
            snapshot = Path(command[-3])
            self.assertNotEqual(snapshot, self.source)
            self.assertEqual(snapshot.read_bytes(), self.source.read_bytes())
            if exit_code == 0 and not no_object:
                Path(command[-1]).write_bytes(elf_fixture() if object_data is None else object_data)
            if mutation:
                mutation()
            return subprocess.CompletedProcess(command, exit_code, b'', b'private diagnostic')
        with patch.object(rebuild.shutil, 'which', return_value=str(self.compiler)), patch.object(rebuild.subprocess, 'run', side_effect=run):
            yield calls

    def verify(self, **kwargs):
        with self.compiler_fixture(**kwargs):
            return rebuild.verify(self.ledger, self.game, self.compiler, self.output, self.root)

    def test_fresh_match_and_record_check(self):
        report = self.verify()
        self.assertEqual(report['functions'][0]['status'], 'matched')
        self.assertEqual(len(rebuild.recorded_matches(report, self.ledger, self.root)), 1)
        self.assertNotIn(str(self.root), json.dumps(report))
        self.assertNotIn('InstalledDir', json.dumps(report))
        self.assertIn('-nostdinc', report['functions'][0]['compiler_args'])

    def test_wrong_baseline_never_invokes_compiler(self):
        self.game.write_bytes(b'wrong')
        with self.compiler_fixture() as calls, self.assertRaisesRegex(ValueError, 'SHA-256'):
            rebuild.verify(self.ledger, self.game, self.compiler, self.output, self.root)
        self.assertEqual(calls, [])

    def test_cannot_change_target_hash_to_fit_another_baseline(self):
        self.ledger['target']['baseline_sha256'] = '0' * 64
        with self.assertRaisesRegex(ValueError, 'pin'):
            rebuild.validate_ledger(self.ledger, self.root)

    def test_partial_original_function_never_invokes_compiler(self):
        self.ledger['functions'][0]['size'] = 4
        with self.compiler_fixture() as calls, self.assertRaisesRegex(ValueError, 'complete original'):
            rebuild.verify(self.ledger, self.game, self.compiler, self.output, self.root)
        self.assertEqual(calls, [])

    def test_original_alias_or_overlap_rejected(self):
        elf = rebuild.Elf32(elf_fixture(kind=2, extra_function=True))
        with self.assertRaisesRegex(ValueError, 'overlaps or aliases'):
            elf.original(self.ledger['functions'][0])

    def test_elf_bounds_type_and_machine_rejected(self):
        wrong_machine = bytearray(elf_fixture())
        struct.pack_into('<H', wrong_machine, 18, 62)
        for data in (b'', elf_fixture()[:90], bytes(wrong_machine)):
            with self.subTest(length=len(data)), self.assertRaises(ValueError):
                rebuild.Elf32(data)
        for data in (elf_fixture(symbol_size=80), elf_fixture(symbol_type=1)):
            with self.assertRaises(ValueError):
                rebuild.Elf32(data).rebuilt('candidate')

    def test_code_relocations_rejected(self):
        with self.assertRaisesRegex(ValueError, 'relocations'):
            rebuild.Elf32(elf_fixture(relocation=True)).rebuilt('candidate')

    def test_exact_nonallocated_pdr_metadata_exception(self):
        self.assertEqual(rebuild.Elf32(elf_fixture(pdr=True)).rebuilt('candidate'), CODE)
        for field, value in ((1, 4), (5, 16), (6, 3), (7, 1), (9, 16)):
            with self.subTest(field=field), self.assertRaises(ValueError):
                rebuild.Elf32(change_section(elf_fixture(pdr=True), '.rel.pdr', field, value)).rebuilt('candidate')
        for field, value in ((2, 2), (5, 28), (1, 8)):
            with self.subTest(field=field), self.assertRaises(ValueError):
                rebuild.Elf32(change_section(elf_fixture(pdr=True), '.pdr', field, value)).rebuilt('candidate')

    def test_pdr_relocation_wrong_symbol_type_or_offset(self):
        data = elf_fixture(pdr=True)
        _, section = rebuild.Elf32(data).named_section('.rel.pdr')
        for offset, info in ((4, 0x102), (0, 0x104), (0, 0x202)):
            bad = bytearray(data)
            struct.pack_into('<II', bad, section[4], offset, info)
            with self.assertRaisesRegex(ValueError, 'metadata relocation'):
                rebuild.Elf32(bytes(bad)).rebuilt('candidate')

    def test_extra_functions_data_undefined_symbols_rejected(self):
        for option in ('extra_function', 'allocated_data', 'undefined'):
            with self.subTest(option=option), self.assertRaises(ValueError):
                rebuild.Elf32(elf_fixture(**{option: True})).rebuilt('candidate')

    def test_nonzero_or_excess_trailing_code_rejected(self):
        for code in (CODE + b'\0' * 4, CODE + b'\1' * 4):
            with self.assertRaisesRegex(ValueError, 'extra object code'):
                rebuild.Elf32(elf_fixture(code=code, symbol_size=8)).rebuilt('candidate')

    def test_wrong_function_length_and_mismatch_receive_no_credit(self):
        for data, expected in ((elf_fixture(code=b'\0' * 8), 'mismatched'), (elf_fixture(code=CODE * 2), 'failed')):
            report = self.verify(object_data=data)
            self.assertEqual(report['functions'][0]['status'], expected)
            self.assertEqual(rebuild.recorded_matches(report, self.ledger, self.root), [])

    def test_source_snapshot_and_compiler_races_fail(self):
        for path in (self.source, self.compiler):
            original = path.read_bytes()
            report = self.verify(mutation=lambda: path.write_bytes(original + b' changed'))
            self.assertEqual(report['functions'][0]['status'], 'failed')
            self.assertIn('changed during compilation', report['functions'][0]['reason'])
            path.write_bytes(original)

    def test_environment_injection_never_invokes_compiler(self):
        for name in rebuild.INJECTING_ENV:
            with self.subTest(name=name), patch.dict(os.environ, {name: 'secret-option'}):
                with self.compiler_fixture() as calls, self.assertRaises(ValueError) as failure:
                    rebuild.verify(self.ledger, self.game, self.compiler, self.output, self.root)
                self.assertEqual(calls, [])
                self.assertNotIn('secret-option', str(failure.exception))

    def test_failed_compile_and_success_without_object_cannot_reuse_stale_object(self):
        self.output.mkdir()
        (self.output / 'fixture.o').write_bytes(elf_fixture())
        for kwargs in ({'exit_code': 1}, {'no_object': True}):
            report = self.verify(**kwargs)
            self.assertEqual(report['functions'][0]['status'], 'failed')
            self.assertEqual(rebuild.recorded_matches(report, self.ledger, self.root), [])

    def test_sources_and_flags_reject_external_input_and_assembly(self):
        for contents in ('#include "x"', '%:include "x"', 'asm("nop");', '__asm__("nop");',
                         'import x;', '__attribute__((naked)) void x() {}', '??=include "x"', '#inc\\\nlude "x"'):
            self.source.write_text(contents)
            with self.subTest(contents=contents), self.assertRaises(ValueError):
                rebuild.validate_ledger(self.ledger, self.root)
        self.source.write_text('extern "C" int candidate(){return 12;}')
        for flag in ('@args', '-include=x', '--config=x', '-S', '-DVALUE=1', '-Xclang', '-ooutput'):
            ledger = copy.deepcopy(self.ledger)
            ledger['functions'][0]['compiler_args'].append(flag)
            with self.subTest(flag=flag), self.assertRaises(ValueError):
                rebuild.validate_ledger(ledger, self.root)

    def test_duplicate_and_escaping_source_rejected(self):
        self.ledger['functions'].append(copy.deepcopy(self.ledger['functions'][0]))
        with self.assertRaises(ValueError):
            rebuild.validate_ledger(self.ledger, self.root)
        self.ledger['functions'].pop()
        self.ledger['functions'][0]['source'] = '../outside.cpp'
        with self.assertRaisesRegex(ValueError, 'escapes'):
            rebuild.validate_ledger(self.ledger, self.root)

    def test_receipts_reject_stale_source_ledger_verifier_and_flags(self):
        report = self.verify()
        for kind in ('source', 'ledger', 'verifier', 'flags', 'duplicate', 'compiler', 'size'):
            bad = copy.deepcopy(report)
            if kind == 'source': bad['functions'][0]['source_sha256'] = '0' * 64
            elif kind == 'ledger': bad['ledger_sha256'] = '0' * 64
            elif kind == 'verifier': bad['verifier_sha256'] = '0' * 64
            elif kind == 'flags': bad['functions'][0]['compiler_args'].append('-S')
            elif kind == 'duplicate': bad['functions'].append(copy.deepcopy(bad['functions'][0]))
            elif kind == 'compiler': bad['toolchains']['clang']['sha256'] = 'bad'
            else: bad['functions'][0]['size'] = 4
            with self.subTest(kind=kind), self.assertRaises(ValueError):
                rebuild.recorded_matches(bad, self.ledger, self.root)

    def test_report_invalidation_preserves_all_inputs_and_unrelated_files(self):
        unrelated = self.root / 'unrelated.json'
        unrelated.write_text('{"important": true}')
        for destination in (self.ledger_path, self.game, self.source, self.compiler, unrelated):
            before = destination.read_bytes()
            with self.subTest(destination=destination.name), self.assertRaises(ValueError):
                rebuild.invalidate_report(destination, self.ledger_path, self.ledger, self.game, self.compiler, self.root)
            self.assertEqual(destination.read_bytes(), before)
        receipt = self.root / 'receipt.json'
        receipt.write_text(json.dumps(self.verify()))
        rebuild.invalidate_report(receipt, self.ledger_path, self.ledger, self.game, self.compiler, self.root)
        self.assertFalse(receipt.exists())

    def test_receipt_symlink_is_rejected(self):
        symlink = self.root / 'alias.json'
        symlink.symlink_to(self.ledger_path)
        with self.assertRaises(ValueError):
            rebuild.invalidate_report(symlink, self.ledger_path, self.ledger, self.game, self.compiler, self.root)
        self.assertTrue(self.ledger_path.exists())

    def test_comment_looking_strings_cannot_hide_external_source(self):
        for opening, closing in (('"/*"', '"*/"'), ('R"(/*)"', 'R"(*/)"')):
            self.source.write_text('constexpr const char* begin = ' + opening + ';\n'
                                   '#include "/tmp/unhashed.hpp"\n'
                                   'constexpr const char* end = ' + closing + ';\n')
            with self.assertRaisesRegex(ValueError, 'preprocessor'):
                rebuild.source_bytes(self.source)
        self.source.write_text('/**/#include "/tmp/unhashed.hpp"\n')
        with self.assertRaisesRegex(ValueError, 'preprocessor'):
            rebuild.source_bytes(self.source)

    def test_cli_required_match_mode_rejects_failed_or_missing_receipts(self):
        original_recorded = rebuild.recorded_matches
        receipt = self.root / 'receipt.json'
        for data in (elf_fixture(code=b'\0' * 8), elf_fixture(code=CODE * 2)):
            receipt.write_text(json.dumps(self.verify(object_data=data)))
            with patch.object(rebuild, 'load_ledger', return_value=self.ledger), \
                 patch.object(rebuild, 'recorded_matches', side_effect=lambda r, l: original_recorded(r, l, self.root)), \
                 contextlib.redirect_stderr(__import__('io').StringIO()):
                self.assertEqual(rebuild.main(['check', '--report', str(receipt), '--require-matches']), 1)
        with patch.object(rebuild, 'load_ledger', return_value=self.ledger), contextlib.redirect_stderr(__import__('io').StringIO()):
            self.assertEqual(rebuild.main(['check', '--require-matches']), 1)

    def prepare_ee(self):
        self.ee_prefix = self.root / 'ee'
        self.ee_driver = self.ee_prefix / ('bin/' + rebuild.EE_TARGET + '-g++')
        self.ee_cc1plus = self.ee_prefix / ('libexec/gcc/' + rebuild.EE_TARGET + '/' + rebuild.EE_VERSION + '/cc1plus')
        self.ee_as = self.ee_prefix / (rebuild.EE_TARGET + '/bin/as')
        for path in (self.ee_driver, self.ee_cc1plus, self.ee_as):
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(('synthetic ' + path.name).encode())
        self.ee_specs = b'synthetic builtin specs'
        self.ledger['functions'][0].update(toolchain='ee-gcc', compiler_args=sorted(rebuild.EE_FLAGS))

    @contextlib.contextmanager
    def ee_fixture(self, mutation=None, overrides=None, object_data=None, source=None):
        overrides = overrides or {}
        calls = []
        def run(command, **kwargs):
            calls.append(command)
            self.assertEqual(kwargs['env'], {'PATH': os.defpath, 'LANG': 'C', 'LC_ALL': 'C'})
            self.assertNotIn('shell', kwargs)
            option = command[-1]
            response = None
            if option == '--version': response = (rebuild.EE_TARGET + '-g++ (GCC) ' + rebuild.EE_VERSION + '\n').encode()
            elif option == '-dumpmachine': response = rebuild.EE_TARGET.encode()
            elif option == '-print-file-name=specs': response = b'specs'
            elif option == '-print-prog-name=cc1plus': response = str(self.ee_cc1plus).encode()
            elif option == '-print-prog-name=as': response = str(self.ee_as).encode()
            elif option == '-dumpspecs': response = self.ee_specs
            elif '-dM' in command:
                response = '\n'.join('#define ' + k + ' ' + v for k, v in rebuild.EE_MODEL.items()).encode()
                option = 'model'
            if response is not None:
                return subprocess.CompletedProcess(command, 0, overrides.get(option, response), b'')
            self.assertEqual(command[-2], '-o')
            snapshot = Path(command[-3])
            if isinstance(source, tuple):
                self.assertIn(snapshot.read_bytes(), [p.read_bytes() for p in source])
            else:
                self.assertEqual(snapshot.read_bytes(), (source or self.source).read_bytes())
            payload = object_data(snapshot.read_bytes()) if callable(object_data) else object_data
            Path(command[-1]).write_bytes(elf_fixture(pdr=True) if payload is None else payload)
            if mutation: mutation()
            return subprocess.CompletedProcess(command, 0, b'', b'')
        with patch.object(rebuild.shutil, 'which', return_value=str(self.ee_driver)), patch.object(rebuild.subprocess, 'run', side_effect=run):
            yield calls

    def verify_ee(self, **kwargs):
        with self.ee_fixture(**kwargs):
            return rebuild.verify(self.ledger, self.game, None, self.output, self.root, ee_compiler=self.ee_driver)

    def test_ee_match_records_components_specs_model_and_effective_flags(self):
        self.prepare_ee()
        report = self.verify_ee()
        self.assertEqual(report['functions'][0]['status'], 'matched')
        identity = report['toolchains']['ee-gcc']
        self.assertEqual(set(identity['components']), {'cc1plus', 'as'})
        self.assertEqual(identity['components']['as']['sha256'], rebuild.digest(self.ee_as.read_bytes()))
        self.assertEqual(identity['specs_sha256'], rebuild.digest(self.ee_specs))
        self.assertEqual(report['functions'][0]['data_model'], rebuild.EE_MODEL)
        self.assertEqual(len(rebuild.recorded_matches(report, self.ledger, self.root)), 1)
        self.assertNotIn(str(self.root), json.dumps(report))

    def test_ee_component_and_specs_changes_during_compile_fail(self):
        self.prepare_ee()
        for component in (self.ee_driver, self.ee_cc1plus, self.ee_as):
            before = component.read_bytes()
            report = self.verify_ee(mutation=lambda: component.write_bytes(before + b' changed'))
            self.assertEqual(report['functions'][0]['status'], 'failed')
            self.assertEqual(rebuild.recorded_matches(report, self.ledger, self.root), [])
            component.write_bytes(before)
        def change_specs(): self.ee_specs = b'changed specs'
        report = self.verify_ee(mutation=change_specs)
        self.assertEqual(report['functions'][0]['status'], 'failed')
        self.assertIn('specs changed', report['functions'][0]['reason'])

    def test_ee_external_specs_and_component_escape_rejected_before_compile(self):
        self.prepare_ee()
        outside = self.root / 'outside-cc1plus'
        outside.write_bytes(b'unrelated executable')
        for override in ({'-print-file-name=specs': b'/unrecorded/specs'},
                         {'-print-prog-name=cc1plus': str(outside).encode()},
                         {'-print-prog-name=as': b'as'},
                         {'-dumpmachine': b'x86_64-linux-gnu'},
                         {'--version': b'GCC unknown'}):
            with self.subTest(override=override), self.ee_fixture(overrides=override) as calls, self.assertRaises(ValueError):
                rebuild.verify(self.ledger, self.game, None, self.output, self.root, ee_compiler=self.ee_driver)
            self.assertFalse(any('-c' in call for call in calls))

    def test_ee_component_symlink_outside_prefix_rejected(self):
        self.prepare_ee()
        outside = self.root / 'outside'
        outside.write_bytes(b'outside')
        self.ee_cc1plus.unlink()
        self.ee_cc1plus.symlink_to(outside)
        with self.ee_fixture(), self.assertRaisesRegex(ValueError, 'escapes'):
            rebuild.verify(self.ledger, self.game, None, self.output, self.root, ee_compiler=self.ee_driver)

    def test_ee_environment_injection_rejected_before_probes(self):
        self.prepare_ee()
        for name in ('GCC_EXEC_PREFIX', 'COMPILER_PATH', 'GCC_SPECS', 'GCC_COMPARE_DEBUG', 'LD_LIBRARY_PATH'):
            with patch.dict(os.environ, {name: 'private-value'}), self.ee_fixture() as calls, self.assertRaises(ValueError):
                rebuild.verify(self.ledger, self.game, None, self.output, self.root, ee_compiler=self.ee_driver)
            self.assertEqual(calls, [])

    def test_ee_fixed_flags_forbid_plugins_specs_paths_and_other_abis(self):
        self.prepare_ee()
        for flag in ('-fplugin=plugin.so', '-specs=custom.specs', '-B/tmp/tools', '-I/tmp/include',
                     '-mabi=n32', '-mlong64', '@response', '-O3'):
            ledger = copy.deepcopy(self.ledger)
            ledger['functions'][0]['compiler_args'].append(flag)
            with self.subTest(flag=flag), self.assertRaises(ValueError):
                rebuild.validate_ledger(ledger, self.root)
        self.ledger['functions'][0]['toolchain'] = 'unreviewed-gcc'
        with self.assertRaisesRegex(ValueError, 'flavor'):
            rebuild.validate_ledger(self.ledger, self.root)

    def test_ee_wrong_model_or_object_flags_receive_no_credit(self):
        self.prepare_ee()
        report = self.verify_ee(overrides={'model': b'#define __SIZEOF_POINTER__ 8'})
        self.assertEqual(report['functions'][0]['status'], 'failed')
        bad = bytearray(elf_fixture())
        struct.pack_into('<I', bad, 36, 0)
        report = self.verify_ee(object_data=bytes(bad))
        self.assertEqual(report['functions'][0]['status'], 'failed')
        self.assertIn('object flags', report['functions'][0]['reason'])

    def test_ee_receipt_component_tampering_requires_new_function_binding(self):
        self.prepare_ee()
        report = self.verify_ee()
        for kind in ('driver', 'cc1plus', 'as', 'specs', 'model', 'flavor', 'binding', 'missing'):
            bad = copy.deepcopy(report)
            identity = bad['toolchains']['ee-gcc']
            if kind == 'driver': identity['sha256'] = '0' * 64
            elif kind in ('cc1plus', 'as'): identity['components'][kind]['sha256'] = '0' * 64
            elif kind == 'specs': identity['specs_sha256'] = '0' * 64
            elif kind == 'model': bad['functions'][0]['data_model']['__SIZEOF_POINTER__'] = '8'
            elif kind == 'flavor': bad['functions'][0]['toolchain'] = 'clang'
            elif kind == 'binding': bad['functions'][0]['toolchain_sha256'] = '0' * 64
            else: del identity['components']['as']
            with self.subTest(kind=kind), self.assertRaises(ValueError):
                rebuild.recorded_matches(bad, self.ledger, self.root)

    def test_legacy_receipts_are_clang_only_and_remain_hash_bound(self):
        report = self.verify()
        legacy = copy.deepcopy(report)
        legacy['schema_version'] = 1
        legacy['compiler'] = legacy.pop('toolchains')['clang']
        del legacy['compiler']['flavor']
        for key in ('toolchain', 'toolchain_sha256'):
            del legacy['functions'][0][key]
        self.assertEqual(len(rebuild.recorded_matches(legacy, self.ledger, self.root)), 1)
        legacy['verifier_sha256'] = '0' * 64
        with self.assertRaisesRegex(ValueError, 'stale receipt'):
            rebuild.recorded_matches(legacy, self.ledger, self.root)
        self.prepare_ee()
        report = self.verify_ee()
        report['schema_version'] = 1
        with self.assertRaisesRegex(ValueError, 'legacy receipts'):
            rebuild.recorded_matches(report, self.ledger, self.root)

    def test_mixed_dispatch_uses_distinct_sources_originals_and_compiler_identities(self):
        self.prepare_ee()
        second_code = b'\x05\x06\x07\x08' * 2
        second_source = self.root / 'second.cpp'
        second_source.write_text('extern "C" int second(void*, int) { return 24; }\n')
        second = copy.deepcopy(self.ledger['functions'][0])
        second.update(id='second', address='0x00100008', original_symbol='other', source='second.cpp', symbol='second')
        first = self.ledger['functions'][0]
        del first['toolchain']
        first['compiler_args'] = sorted(rebuild.REQUIRED_FLAGS) + ['-O2']
        self.ledger['functions'].append(second)
        self.game.write_bytes(elf_fixture(kind=2, code=CODE + second_code, symbol_size=8,
                                         extra_function=True, extra_value=BASE + 8, extra_size=8))
        self.target['text']['size'] = 16
        self.target['baseline_sha256'] = rebuild.digest(self.game.read_bytes())
        self.ledger['target'] = copy.deepcopy(self.target)
        with patch.object(rebuild, 'BASELINE_SIZE', self.game.stat().st_size):
            with self.ee_fixture(object_data=elf_fixture(code=second_code, symbol='second'), source=second_source) as ee_calls:
                ee_run = rebuild.subprocess.run
                with self.compiler_fixture() as clang_calls:
                    clang_run = rebuild.subprocess.run
                    def dispatch(command, **kwargs):
                        return (ee_run if Path(command[0]).resolve() == self.ee_driver.resolve() else clang_run)(command, **kwargs)
                    def which(selected):
                        return str(self.ee_driver if Path(selected).resolve() == self.ee_driver.resolve() else self.compiler)
                    with patch.object(rebuild.subprocess, 'run', side_effect=dispatch), patch.object(rebuild.shutil, 'which', side_effect=which):
                        report = rebuild.verify(self.ledger, self.game, self.compiler, self.output, self.root, ee_compiler=self.ee_driver)
                        self.assertEqual([r['status'] for r in report['functions']], ['matched', 'matched'])
                        self.assertEqual([r['toolchain'] for r in report['functions']], ['clang', 'ee-gcc'])
                        self.assertEqual(len(rebuild.recorded_matches(report, self.ledger, self.root)), 2)
                        self.assertEqual(sum('-c' in call for call in clang_calls), 1)
                        self.assertEqual(sum('-c' in call for call in ee_calls), 1)
                        bad = copy.deepcopy(report)
                        bad['functions'][0]['toolchain'] = 'ee-gcc'
                        bad['functions'][0]['toolchain_sha256'] = rebuild.digest(rebuild.canonical(bad['toolchains']['ee-gcc']))
                        with self.assertRaisesRegex(ValueError, 'toolchain identity mismatch'):
                            rebuild.recorded_matches(bad, self.ledger, self.root)
                        for clang, ee in ((None, self.ee_driver), (self.compiler, None)):
                            before = (sum('-c' in call for call in clang_calls), sum('-c' in call for call in ee_calls))
                            with self.assertRaisesRegex(ValueError, 'missing compiler'):
                                rebuild.verify(self.ledger, self.game, clang, self.output, self.root, ee_compiler=ee)
                            self.assertEqual(before, (sum('-c' in call for call in clang_calls), sum('-c' in call for call in ee_calls)))

    def prepare_call_graph(self):
        self.prepare_ee()
        self.source.write_text('extern "C" int dep(int); extern "C" int caller(int x) { return dep(x) + 1; }\n')
        self.dep_source = self.root / 'dep.cpp'
        self.dep_source.write_text('extern "C" int dep(int) { return 12; }\n')
        caller = self.ledger['functions'][0]
        caller.update(symbol='caller', size=32, comparison_mode=rebuild.DIRECT_MODE, call_bindings=[{
            'callee_id': 'dep', 'object_symbol': 'dep', 'original_symbol': 'original_dep',
            'address': '0x00100020', 'size': 8, 'body_sha256': rebuild.digest(CALLEE), 'call_offsets': [8]}])
        dep = {key: copy.deepcopy(value) for key, value in caller.items() if key not in ('comparison_mode', 'call_bindings')}
        dep.update(id='dep', symbol='dep', original_symbol='original_dep', address='0x00100020', size=8, source='dep.cpp')
        self.ledger['functions'].append(dep)
        self.game.write_bytes(call_elf_fixture(kind=2, code=LINKED_CALLER + CALLEE, functions=[
            ('original', BASE, 32, 2, 1, 0, 1), ('original_dep', BASE + 32, 8, 2, 1, 0, 1)]))
        self.target['text']['size'] = 40
        self.target['baseline_sha256'] = rebuild.digest(self.game.read_bytes())
        self.ledger['target'] = copy.deepcopy(self.target)
        self.graph_objects = {self.source.read_bytes(): call_elf_fixture(),
                              self.dep_source.read_bytes(): call_elf_fixture(code=CALLEE, functions=[('dep', 0, 8, 2, 1, 0, 1)], relocations=())}

    def verify_graph(self, mutation=None):
        with patch.object(rebuild, 'BASELINE_SIZE', self.game.stat().st_size), \
             self.ee_fixture(object_data=lambda contents: self.graph_objects[contents],
                             source=tuple(self.root / item['source'] for item in self.ledger['functions']), mutation=mutation) as calls:
            report = rebuild.verify(self.ledger, self.game, None, self.output, self.root, ee_compiler=self.ee_driver)
            self.graph_calls = calls
            return report

    def test_direct_graph_compiles_callee_first_and_binds_complete_results(self):
        self.prepare_call_graph()
        report = self.verify_graph()
        self.assertEqual(report['schema_version'], 3)
        self.assertEqual([r['id'] for r in report['functions']], ['fixture', 'dep'])  # ledger order stays stable
        self.assertEqual([r['status'] for r in report['functions']], ['matched', 'matched'])
        calls = [c for c in self.graph_calls if '-c' in c]
        self.assertEqual(len(calls), 2)
        self.assertIn('dep-', calls[0][-3]); self.assertIn('fixture-', calls[1][-3])
        caller, dep = report['functions']
        self.assertNotEqual(caller['raw_rebuilt_sha256'], caller['original_sha256'])
        self.assertEqual(caller['rebuilt_sha256'], rebuild.digest(LINKED_CALLER))
        self.assertEqual(caller['dependencies'], {'dep': rebuild.digest(rebuild.canonical(dep))})
        self.assertEqual(caller['relocations'][0]['relocated_word'], '0x0c040008')
        self.assertEqual(len(rebuild.recorded_matches(report, self.ledger, self.root)), 2)
        self.assertNotIn(str(self.root), json.dumps(report))

    def test_direct_graph_missing_mixed_stale_and_recursive_bindings_fail(self):
        self.prepare_call_graph()
        original = copy.deepcopy(self.ledger)
        mutations = [lambda l: l['functions'].pop(),
            lambda l: l['functions'][1].update(toolchain='clang', compiler_args=sorted(rebuild.REQUIRED_FLAGS)+['-O2']),
            lambda l: l['functions'][1]['compiler_args'].reverse(),
            lambda l: l['functions'][0]['call_bindings'][0].update(callee_id='fixture'),
            lambda l: l['functions'][0]['call_bindings'][0].update(address='0x00100024'),
            lambda l: l['functions'][0]['call_bindings'][0].update(object_symbol='unrelated'),
            lambda l: l['functions'][0]['call_bindings'][0].update(original_symbol='wrong'),
            lambda l: l['functions'][0]['call_bindings'][0].update(size=True),
            lambda l: l['functions'][0]['call_bindings'][0].update(body_sha256='bad'),
            lambda l: l['functions'][0]['call_bindings'][0].update(call_offsets=[8,8]),
            lambda l: l['functions'][0]['call_bindings'][0].update(call_offsets=[True]),
            lambda l: l['functions'][0]['call_bindings'][0].update(call_offsets=[28]),
            lambda l: l['functions'][0]['call_bindings'][0].update(unresolved=True),
            lambda l: l['functions'][0].update(comparison_mode='retail-address-link')]
        for change in mutations:
            bad = copy.deepcopy(original); change(bad)
            with self.subTest(ledger=bad), self.assertRaises((ValueError, KeyError)):
                rebuild.validate_ledger(bad, self.root)
        # A two-node cycle passes local edge identity but must fail DAG validation.
        caller, dep = self.ledger['functions']
        dep.update(comparison_mode=rebuild.DIRECT_MODE, call_bindings=[dict(callee_id='fixture', object_symbol='caller',
            original_symbol='original', address=caller['address'], size=32, body_sha256=rebuild.digest(LINKED_CALLER), call_offsets=[0])])
        with self.assertRaisesRegex(ValueError, 'cycle'): rebuild.validate_ledger(self.ledger, self.root)

    def test_direct_graph_failed_callee_never_compiles_or_credits_caller(self):
        self.prepare_call_graph()
        bad_code = CALLEE[:4] + struct.pack('<I', 0x2402000d)
        self.graph_objects[self.dep_source.read_bytes()] = call_elf_fixture(code=bad_code, functions=[('dep',0,8,2,1,0,1)],relocations=())
        report = self.verify_graph()
        self.assertEqual([r['status'] for r in report['functions']], ['failed','mismatched'])
        self.assertIn('dependency did not freshly match', report['functions'][0]['reason'])
        self.assertEqual(sum('-c' in c for c in self.graph_calls), 1)
        self.assertEqual(rebuild.recorded_matches(report,self.ledger,self.root), [])
        # A saved success does not enter verify(), and cannot rescue a new failure.
        (self.output/'verification.json').write_text('{"status":"matched"}')
        self.assertEqual(self.verify_graph()['functions'][0]['status'], 'failed')

    def test_direct_graph_transitive_source_closure_is_bound_and_fresh(self):
        self.prepare_call_graph()
        caller, leaf = self.ledger['functions']
        middle = copy.deepcopy(caller)
        middle.update(id='mid', symbol='mid', original_symbol='original_mid', address='0x00100020', source='mid.cpp')
        middle_source = self.root / 'mid.cpp'
        middle_source.write_text('extern "C" int dep(int); extern "C" int mid(int x) { return dep(x) + 1; }\n')
        leaf['address'] = '0x00100040'
        middle['call_bindings'][0]['address'] = leaf['address']
        linked_middle = CALLER[:8] + struct.pack('<I', 0x0c040010) + CALLER[12:]
        caller['call_bindings'][0].update(callee_id='mid', object_symbol='mid', original_symbol='original_mid',
                                         size=32, body_sha256=rebuild.digest(linked_middle))
        self.ledger['functions'].append(middle)
        self.source.write_text('extern "C" int mid(int); extern "C" int caller(int x) { return mid(x) + 1; }\n')
        self.graph_objects[self.source.read_bytes()] = call_elf_fixture(functions=[('caller',0,32,2,1,0,1),('mid',0,0,0,1,0,0)])
        self.graph_objects[middle_source.read_bytes()] = call_elf_fixture(functions=[('mid',0,32,2,1,0,1),('dep',0,0,0,1,0,0)])
        self.game.write_bytes(call_elf_fixture(kind=2, code=LINKED_CALLER + linked_middle + CALLEE, functions=[
            ('original',BASE,32,2,1,0,1),('original_mid',BASE+32,32,2,1,0,1),('original_dep',BASE+64,8,2,1,0,1)]))
        self.target['text']['size']=72;self.target['baseline_sha256']=rebuild.digest(self.game.read_bytes())
        self.ledger['target']=copy.deepcopy(self.target)
        report=self.verify_graph();self.assertEqual(len(rebuild.recorded_matches(report,self.ledger,self.root)),3)
        top, bottom, mid=report['functions']
        self.assertEqual(top['dependencies'],{'mid':rebuild.digest(rebuild.canonical(mid))})
        self.assertEqual(mid['dependencies'],{'dep':rebuild.digest(rebuild.canonical(bottom))})
        # A valid-looking changed leaf result invalidates its parent's saved digest.
        bottom['object_sha256']='0'*64
        with self.assertRaisesRegex(ValueError,'dependency result'):rebuild.recorded_matches(report,self.ledger,self.root)
        # A fresh leaf mismatch prevents BOTH callers from compiling.
        self.graph_objects[self.dep_source.read_bytes()] = call_elf_fixture(code=CALLEE[:4]+struct.pack('<I',0x2402000d),
            functions=[('dep',0,8,2,1,0,1)],relocations=())
        failed=self.verify_graph()
        self.assertEqual([r['status'] for r in failed['functions']],['failed','mismatched','failed'])
        self.assertEqual(sum('-c' in c for c in self.graph_calls),1)

    def test_direct_graph_all_sources_stay_stable_through_entire_run(self):
        self.prepare_call_graph()
        count = [0]
        def late_change():
            count[0] += 1
            if count[0] == 2: self.dep_source.write_text(self.dep_source.read_text()+'// changed after dependency compile\n')
        report = self.verify_graph(mutation=late_change)
        self.assertEqual([r['status'] for r in report['functions']], ['failed','failed'])
        self.assertTrue(all('graph verification' in r['reason'] for r in report['functions']))

    def test_direct_graph_original_target_hash_and_load_mapping_fail_before_compile(self):
        self.prepare_call_graph()
        baseline = self.game.read_bytes()
        wrong_target = bytearray(baseline)
        offset = rebuild.Elf32(baseline).sections[1][4]
        struct.pack_into('<I', wrong_target, offset+8, 0x0c040009)
        weak_dep = change_symbol(baseline, 'original_dep', binding=2)
        alias_dep = change_symbol(baseline, 'original_dep', value=BASE+4)
        no_exec = bytearray(baseline); struct.pack_into('<I',no_exec,52+24,4)
        wrong_mapping = bytearray(baseline); struct.pack_into('<I',wrong_mapping,52+4,offset+4)
        for data in (bytes(wrong_target), weak_dep, alias_dep, bytes(no_exec), bytes(wrong_mapping)):
            self.game.write_bytes(data); self.target['baseline_sha256']=rebuild.digest(data)
            self.ledger['target']=copy.deepcopy(self.target)
            with self.subTest(data_hash=rebuild.digest(data)), self.ee_fixture() as calls, \
                 patch.object(rebuild,'BASELINE_SIZE',len(data)), self.assertRaises(ValueError):
                rebuild.verify(self.ledger,self.game,None,self.output,self.root,ee_compiler=self.ee_driver)
            self.assertEqual(calls,[])
        self.game.write_bytes(baseline); self.target['baseline_sha256']=rebuild.digest(baseline); self.ledger['target']=copy.deepcopy(self.target)
        self.ledger['functions'][0]['call_bindings'][0]['body_sha256']='0'*64
        with self.ee_fixture() as calls, patch.object(rebuild,'BASELINE_SIZE',len(baseline)), self.assertRaisesRegex(ValueError,'body hash'):
            rebuild.verify(self.ledger,self.game,None,self.output,self.root,ee_compiler=self.ee_driver)
        self.assertEqual(calls,[])

    def test_direct_object_relocation_table_shape_and_symbol_indices(self):
        self.prepare_call_graph(); bindings=self.ledger['functions'][0]['call_bindings']
        valid=call_elf_fixture()
        mutations=[change_section(valid,'.rel.text',field,value) for field,value in
                   ((1,4),(2,0),(3,4),(4,len(valid)+4),(5,7),(6,3),(7,4),(8,8),(9,16))]
        mutations += [call_elf_fixture(relocations=r) for r in
                      ((),((8,0x204),(8,0x204)),((8,0x104),),((8,0x304),),((8,0x202),),((9,0x204),),((32,0x204),))]
        for data in mutations:
            with self.subTest(hash=rebuild.digest(data)),self.assertRaises(ValueError):
                rebuild.Elf32(data).rebuilt('caller',bindings)
        code,records=rebuild.Elf32(valid).rebuilt('caller',bindings)
        self.assertEqual(code,CALLER);self.assertEqual(records,[{'offset':8,'type':4,'symbol':'dep'}])
        with self.assertRaises(ValueError):rebuild.Elf32(valid).rebuilt('caller')  # leaves stay closed

    def test_direct_object_rejects_weak_absolute_typed_or_ambiguous_imports(self):
        self.prepare_call_graph(); bindings=self.ledger['functions'][0]['call_bindings'];valid=call_elf_fixture()
        for changes in ({'binding':2},{'binding':0},{'other':1},{'type':1},{'value':4},{'size':4},{'section':0xfff1},{'section':0xfff2}):
            with self.subTest(changes=changes),self.assertRaises(ValueError):
                rebuild.Elf32(change_symbol(valid,'dep',**changes)).rebuilt('caller',bindings)
        # STT_FUNC undefined is a declaration, not a second defined body.
        self.assertEqual(rebuild.Elf32(change_symbol(valid,'dep',type=2)).rebuilt('caller',bindings)[0],CALLER)
        extra=[('caller',0,32,2,1,0,1),('dep',0,0,0,1,0,0),('dep',0,0,0,1,0,0)]
        with self.assertRaises(ValueError):rebuild.Elf32(call_elf_fixture(functions=extra)).rebuilt('caller',bindings)
        for section,field,value in (('.symtab',7,2),('.data',1,2),('.data',1,11),('.text',2,7),('.pdr',2,2)):
            with self.subTest(section=section,field=field),self.assertRaises(ValueError):
                rebuild.Elf32(change_section(valid,section,field,value)).rebuilt('caller',bindings)
        duplicate=change_section(valid,'.data',0,rebuild.Elf32(valid).sections[6][0])
        with self.assertRaisesRegex(ValueError,'duplicate'):rebuild.Elf32(duplicate).rebuilt('caller',bindings)

    def test_direct_instruction_audit_is_closed_and_checks_delay_slots(self):
        rebuild.audit_integer_body(CALLER,[8]);rebuild.audit_integer_body(CALLEE)
        for word in (0x08000000,0x0c000000,0x0320f809,0x03e00008,0x10000000,0x50000000,
                     0x42000018,0x0000000c,0x0000000d,0x70000000,0x00200000,0x00000061,0x44800000,0x241f0000):
            bad=bytearray(CALLER);struct.pack_into('<I',bad,12,word)
            with self.subTest(word=hex(word)),self.assertRaises(ValueError):rebuild.audit_integer_body(bytes(bad),[8])
        with self.assertRaises(ValueError):rebuild.audit_integer_body(CALLER,[])
        with self.assertRaises(ValueError):rebuild.audit_integer_body(CALLER,[8,8])
        # JAL immediately before terminal JR would put a control transfer in its delay slot.
        with self.assertRaises(ValueError):rebuild.audit_integer_body(struct.pack('<4I',0,0x0c000000,0x03e00008,0),[4])
        with self.assertRaises(ValueError):rebuild.audit_integer_body(CALLER[:-4],[8])

    def test_direct_object_accepts_only_observed_mips_section_and_file_metadata(self):
        self.prepare_call_graph();bindings=self.ledger['functions'][0]['call_bindings']
        rows=[('.text',0,0,3,0,0,1),('fixture.cpp',0,0,4,0,0,0xfff1),
              ('caller',0,32,2,1,0,1),('dep',0,0,0,1,0,0)]
        data=call_elf_fixture(functions=rows,relocations=((8,0x404),))
        data=change_section(data,'.symtab',7,2)  # SGI split before local STT_FILE
        elf=rebuild.Elf32(data);_,pdr=elf.named_section('.rel.pdr')
        data=bytearray(data);struct.pack_into('<II',data,pdr[4],0,0x302);data=bytes(data)
        self.assertEqual(rebuild.Elf32(data).rebuilt('caller',bindings)[0],CALLER)
        self.assertEqual(rebuild.Elf32(change_section(data,'.symtab',7,3)).rebuilt('caller',bindings)[0],CALLER)
        for change in ({'value':4},{'size':4},{'section':1},{'type':0},{'binding':1},{'other':1}):
            with self.subTest(change=change),self.assertRaises(ValueError):
                rebuild.Elf32(change_symbol(data,'fixture.cpp',**change)).rebuilt('caller',bindings)
        for boundary in (0,1,4,5):
            with self.subTest(boundary=boundary),self.assertRaises(ValueError):
                rebuild.Elf32(change_section(data,'.symtab',7,boundary)).rebuilt('caller',bindings)
        with self.assertRaises(ValueError):
            rebuild.Elf32(change_symbol(data,'.text',section=4)).rebuilt('caller',bindings)

    def test_direct_relocation_checks_zero_addend_original_and_all_other_bytes(self):
        self.prepare_call_graph();item=self.ledger['functions'][0]
        code,_=rebuild.relocate_calls(CALLER,LINKED_CALLER,item);self.assertEqual(code,LINKED_CALLER)
        for word in (0x0c000001,0x08000000,0x00000000):
            bad=bytearray(CALLER);struct.pack_into('<I',bad,8,word)
            with self.subTest(word=word),self.assertRaises(ValueError):rebuild.relocate_calls(bytes(bad),LINKED_CALLER,item)
        bad=bytearray(LINKED_CALLER);struct.pack_into('<I',bad,8,0x0c040009)
        with self.assertRaisesRegex(ValueError,'original JAL'):rebuild.relocate_calls(CALLER,bytes(bad),item)
        bad=bytearray(CALLER);struct.pack_into('<I',bad,20,0x24420002)
        self.graph_objects[self.source.read_bytes()]=call_elf_fixture(code=bytes(bad))
        report=self.verify_graph()
        self.assertEqual(report['functions'][0]['status'],'mismatched')
        self.assertEqual(len(rebuild.recorded_matches(report,self.ledger,self.root)),1)

    def test_direct_pc_plus_four_region_and_extent_guards(self):
        self.prepare_call_graph();item=copy.deepcopy(self.ledger['functions'][0])
        item['address']='0x0ffffff4';item['call_bindings'][0]['address']='0x10002000'
        self.assertEqual(rebuild.callsite_evidence(item)[0]['relocated_word'],'0x0c000800')
        for target in ('0x0fff2000','0x10002001'):
            item['call_bindings'][0]['address']=target
            with self.assertRaises(ValueError):rebuild.callsite_evidence(item)
        item['address']='0xfffffff0'
        with self.assertRaises(ValueError):rebuild.callsite_evidence(item)

    def test_direct_receipts_reject_dependency_and_policy_tampering(self):
        self.prepare_call_graph();report=self.verify_graph()
        for kind in ('schema','policy','policy_hash','audit','mode','link','raw','relocation','dependencies','dep_status','dep_body','dep_source','dep_compiler'):
            bad=copy.deepcopy(report);caller,dep=bad['functions']
            if kind=='schema':bad['schema_version']=2
            elif kind=='policy':bad['call_policy']['addend']=4
            elif kind=='policy_hash':bad['call_policy_sha256']='0'*64
            elif kind=='audit':del dep['control_flow_policy']
            elif kind=='mode':del caller['comparison_mode']
            elif kind=='link':caller['link_address']='0x00100004'
            elif kind=='raw':caller['raw_rebuilt_sha256']='bad'
            elif kind=='relocation':caller['relocations'][0]['target_address']='0x00100024'
            elif kind=='dependencies':caller['dependencies']['dep']='0'*64
            elif kind=='dep_status':dep['status']='failed'
            elif kind=='dep_body':dep['original_sha256']=dep['rebuilt_sha256']='0'*64
            elif kind=='dep_source':dep['source_sha256']='0'*64
            else:dep['toolchain_sha256']='0'*64
            with self.subTest(kind=kind),self.assertRaises(ValueError):rebuild.recorded_matches(bad,self.ledger,self.root)

    def test_cli_help(self):
        result = subprocess.run([sys.executable, '-I', '-B', rebuild.__file__, '--help'], capture_output=True, text=True, timeout=15)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('verify', result.stdout)


class HistoricalTests(unittest.TestCase):
    setUp = RebuildTests.setUp

    def prepare(self):
        self.root = self.root.resolve()
        self.driver = self.root / rebuild.HIST_LAYOUT['driver_directory'] / 'xgcc'
        self.assembler = self.root / rebuild.HIST_LAYOUT['assembler']
        self.cpp = self.driver.parent / 'cpp'
        self.cc1plus = self.driver.parent / 'cc1plus'
        for path in (self.driver, self.cpp, self.cc1plus, self.assembler):
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(('synthetic ' + path.name).encode())
        self.descriptor = self.root / rebuild.HIST_DESCRIPTOR
        self.descriptor.parent.mkdir(parents=True)
        self.descriptor.write_text(json.dumps(rebuild.HIST_LAYOUT))
        # Independent small branch body, not game bytes: both outcomes return.
        self.code = struct.pack('<6I', 0x10800002, 0x24020001, 0x24020002, 0, 0x03e00008, 0)
        self.game.write_bytes(call_elf_fixture(kind=2, code=self.code,
            functions=[('original', BASE, len(self.code), 2, 1, 0, 1)]))
        self.target.update(baseline_sha256=rebuild.digest(self.game.read_bytes()))
        self.target['text']['size'] = len(self.code)
        self.size_patch.stop()
        self.size_patch = patch.object(rebuild, 'BASELINE_SIZE', self.game.stat().st_size)
        self.size_patch.start(); self.addCleanup(self.size_patch.stop)
        self.ledger['target'] = copy.deepcopy(self.target)
        self.ledger['functions'][0].update(size=len(self.code), toolchain=rebuild.HIST_FLAVOR,
            comparison_mode=rebuild.HIST_MODE, compiler_args=list(rebuild.HIST_FLAGS))

    def object(self, code=None, **kwargs):
        obj = bytearray(elf_fixture(code=self.code if code is None else code, pdr=True, **kwargs))
        struct.pack_into('<I', obj, 36, rebuild.HIST_OBJECT_FLAGS)
        return bytes(obj)

    @contextlib.contextmanager
    def fixture(self, overrides=None, mutation=None, payload=None, fail=None):
        overrides = overrides or {}
        calls = []
        def run(command, **kwargs):
            calls.append(command)
            self.assertIn(command[0], (str(self.driver), str(self.assembler)))
            self.assertNotIn('shell', kwargs)
            self.assertEqual(set(kwargs['env']), {'PATH', 'LANG', 'LC_ALL', 'TMPDIR'})
            self.assertEqual(str(kwargs['cwd']), kwargs['env']['TMPDIR'])
            self.assertTrue(Path(kwargs['cwd']).is_dir())
            option = command[-1]
            if command[0] == str(self.assembler) and option == '--version':
                output = b'GNU assembler (GNU Binutils) 2.45.1\nThis assembler was configured for mips64r5900el-ps2-elf.\n'
            elif option == '-dumpversion': output = b'2.95.2\n'
            elif option == '-dumpmachine': output = b'mips64el-unknown-elf\n'
            elif option == '-print-file-name=specs': output = b'specs\n'
            elif option == '-print-prog-name=cpp': output = str(self.cpp).encode() + b'\n'
            elif option == '-print-prog-name=cc1plus': output = str(self.cc1plus).encode() + b'\n'
            elif option == '-dumpspecs': output = b'synthetic builtin specs'
            elif option == '-print-search-dirs':
                d = self.driver.parent
                p = self.root / rebuild.HIST_LAYOUT['configured_prefix']
                s, t, v = p / 'lib/gcc-lib', 'mips64el-unknown-elf', '2.95.2'
                libs = [d/t/v,d,s/t/v,Path('/usr/lib/gcc')/t/v,p/t/'lib'/t/v,p/t/'lib']
                programs = [d/t/v,d,s/t/v,s/t,Path('/usr/lib/gcc')/t/v,Path('/usr/lib/gcc')/t,p/t/'bin'/t/v,p/t/'bin']
                output = ('install: '+str(s/t/v)+'/\nprograms: '+':'.join(str(x)+'/' for x in programs)+
                          '\nlibraries: '+':'.join(str(x)+'/' for x in libs)+'\n').encode()
            elif '-dM' in command:
                output = '\n'.join('#define '+k+' '+v for k,v in rebuild.HIST_MACROS.items()).encode()
                option = 'macros'
            else:
                output = None
            if output is not None:
                return subprocess.CompletedProcess(command, 0, overrides.get(option, output), b'')
            destination = Path(command[-1])
            phase = 'model-compile' if destination.name == 'model.s' else 'compile' if destination.suffix == '.s' else 'model-as' if destination.name == 'model.o' else 'assemble'
            if phase == fail:
                return subprocess.CompletedProcess(command, 1, b'', b'failed')
            if destination.suffix == '.s':
                snapshot = Path(command[-3])
                expected = rebuild.HIST_MODEL_SOURCE if phase == 'model-compile' else self.source.read_bytes()
                self.assertEqual(snapshot.read_bytes(), expected)
                self.assertIn('-nostdinc', command)
                self.assertIn('-nostdinc++', command)
                destination.write_bytes(b'fresh generated assembly ' + rebuild.digest(expected).encode())
            else:
                assembly = destination.with_suffix('.s')
                self.assertEqual(kwargs['input'], assembly.read_bytes())
                data = self.object(code=struct.pack('<2I', 0x03e00008, 0x24020004), symbol='historical_model_probe') if phase == 'model-as' else (payload or self.object())
                destination.write_bytes(data)
            if mutation: mutation(phase, command)
            return subprocess.CompletedProcess(command, 0, b'', b'')
        with patch.object(rebuild.subprocess, 'run', side_effect=run):
            yield calls

    def verify(self, **kwargs):
        with self.fixture(**kwargs):
            return rebuild.verify(self.ledger, self.game, None, self.output, self.root,
                historical_compiler=self.driver, historical_assembler=self.assembler)

    def test_fresh_two_stage_match_and_complete_record(self):
        self.prepare()
        report = self.verify()
        self.assertEqual(report['functions'][0]['status'], 'matched')
        self.assertEqual(report['schema_version'], 4)
        self.assertEqual(len(rebuild.recorded_matches(report, self.ledger, self.root)), 1)
        self.assertEqual(set(report['toolchains'][rebuild.HIST_FLAVOR]['components']), {'cpp','cc1plus','as'})
        self.assertNotIn(str(self.root), json.dumps(report))
        self.assertEqual(report['functions'][0]['object_flags'], 0x20002001)

    def test_relative_compiler_and_assembler_resolve_before_private_cwd(self):
        self.prepare()
        previous = Path.cwd()
        try:
            os.chdir(self.root)
            with self.fixture():
                report = rebuild.verify(self.ledger, self.game, None, self.output, self.root,
                    historical_compiler=self.driver.relative_to(self.root),
                    historical_assembler=self.assembler.relative_to(self.root))
            self.assertEqual(report['functions'][0]['status'], 'matched')
        finally:
            os.chdir(previous)

    def test_all_normal_and_secondary_specs_rejected_before_probes(self):
        self.prepare()
        p = self.root / rebuild.HIST_LAYOUT['configured_prefix']
        t = rebuild.HIST_TARGET; v = rebuild.HIST_VERSION; d = self.driver.parent
        paths = [d/'specs',d/t/v/'specs',p/'lib/gcc-lib'/t/v/'specs',p/t/'lib'/t/v/'specs',
                 p/t/'lib/specs',p/'lib/gcc-lib'/t/'specs']
        for filename in paths:
            filename.parent.mkdir(parents=True, exist_ok=True)
            filename.symlink_to(filename.parent / 'absent')
            with self.subTest(path=filename), self.fixture() as calls, self.assertRaisesRegex(ValueError,'specs'):
                rebuild.verify(self.ledger,self.game,None,self.output,self.root,historical_compiler=self.driver,historical_assembler=self.assembler)
            self.assertEqual(calls,[])
            filename.unlink()

    def test_shadow_component_prefix_and_macro_identity_rejected(self):
        self.prepare()
        for option,value in [('-dumpversion',b'3.2.3'),('-dumpmachine',b'mips-ee'),
                ('-print-search-dirs',b'install: /unrelated\n'),('-print-prog-name=cpp',b'/outside/cpp'),
                ('-print-prog-name=cc1plus',str(self.driver.parent/rebuild.HIST_TARGET/rebuild.HIST_VERSION/'cc1plus').encode()),
                ('-print-file-name=specs',b'/outside/specs'),('--version',b'GNU assembler 1.0')]:
            with self.subTest(option=option), self.assertRaises(ValueError):
                self.verify(overrides={option:value})
        report=self.verify(overrides={'macros':b'#define __mips_eabi 1\n'})
        self.assertEqual(report['functions'][0]['status'],'failed')

    def test_fixed_layout_symlink_and_missing_components(self):
        self.prepare()
        content=self.cpp.read_bytes();self.cpp.unlink();self.cpp.symlink_to(self.cc1plus)
        with self.assertRaisesRegex(ValueError,'regular'): self.verify()
        self.cpp.unlink()
        with self.assertRaisesRegex(ValueError,'regular'): self.verify()
        self.cpp.write_bytes(content)
        with self.fixture(), self.assertRaisesRegex(ValueError,'missing historical'):
            rebuild.verify(self.ledger,self.game,None,self.output,self.root)

    def test_failed_stages_and_component_source_assembly_changes(self):
        self.prepare()
        for phase in ('model-compile','model-as','compile','assemble'):
            with self.subTest(phase=phase):
                self.assertEqual(self.verify(fail=phase)['functions'][0]['status'],'failed')
        for path in (self.source,self.driver,self.cpp,self.cc1plus,self.assembler):
            before=path.read_bytes()
            def mutate(phase, command):
                if phase=='assemble': path.write_bytes(before+b' changed')
            report=self.verify(mutation=mutate)
            self.assertEqual(report['functions'][0]['status'],'failed')
            path.write_bytes(before)
        def mutate_assembly(phase, command):
            if phase=='assemble': Path(command[-1]).with_suffix('.s').write_bytes(b'changed')
        report=self.verify(mutation=mutate_assembly)
        self.assertEqual(report['functions'][0]['status'],'failed')
        self.assertIn('assembly changed',report['functions'][0]['reason'])

    def test_historical_environment_rejected_and_descriptor_protected(self):
        self.prepare()
        for name in ('LPATH','DEPENDENCIES_OUTPUT','SUNPRO_DEPENDENCIES','OBJCPLUS_INCLUDE_PATH','COLLECT_GCC_OPTIONS'):
            with patch.dict(os.environ,{name:'injected'}), self.fixture() as calls, self.assertRaisesRegex(ValueError,'unset'):
                rebuild.verify(self.ledger,self.game,None,self.output,self.root,historical_compiler=self.driver,historical_assembler=self.assembler)
            self.assertEqual(calls,[])
        before=self.descriptor.read_bytes()
        with self.assertRaisesRegex(ValueError,'protected'):
            rebuild.invalidate_report(self.descriptor,self.ledger_path,self.ledger,self.game,None,self.root)
        self.assertEqual(self.descriptor.read_bytes(),before)

    def test_historical_bad_object_has_no_credit(self):
        self.prepare()
        cases=[self.object(allocated_data=True),elf_fixture(code=self.code,relocation=True),
               self.object(undefined=True),self.object(extra_function=True),self.object(symbol_size=4),
               self.object(symbol_type=1),self.object(code=struct.pack('<2I',0x03e00008,0)),
               elf_fixture(code=self.code),self.object()[:70]]
        for data in cases:
            with self.subTest(size=len(data)):
                report=self.verify(payload=data)
                self.assertEqual(report['functions'][0]['status'],'failed')
                self.assertEqual(rebuild.recorded_matches(report,self.ledger,self.root),[])

    def test_historical_receipt_policy_stage_and_component_tampering(self):
        self.prepare(); report=self.verify()
        for kind in ('schema','policy','model','assembly','assembler','flags','component','descriptor','mode','missing'):
            bad=copy.deepcopy(report); result=bad['functions'][0]
            if kind=='schema': bad['schema_version']=3
            elif kind=='policy': bad['historical_policy']['dependencies']=True
            elif kind=='model': result['data_model']['compiled']=False
            elif kind=='assembly': result['assembly_sha256']='0'*64
            elif kind=='assembler': result['assembler_args'].append('--other')
            elif kind=='flags': result['object_flags']=rebuild.BASELINE_FLAGS
            elif kind=='component': bad['toolchains'][rebuild.HIST_FLAVOR]['components']['cpp']['sha256']='0'*64
            elif kind=='descriptor': bad['toolchains'][rebuild.HIST_FLAVOR]['descriptor_sha256']='0'*64
            elif kind=='mode': result['comparison_mode']=rebuild.DIRECT_MODE
            else: del result['historical_evidence_sha256']
            with self.subTest(kind=kind),self.assertRaises(ValueError):
                rebuild.recorded_matches(bad,self.ledger,self.root)

    def test_exact_profile_cannot_be_used_as_ee_or_unmarked_leaf(self):
        self.prepare()
        for field,value in [('toolchain','ee-gcc'),('comparison_mode',None),('compiler_args',list(reversed(rebuild.HIST_FLAGS)))]:
            ledger=copy.deepcopy(self.ledger); ledger['functions'][0][field]=value
            with self.assertRaises(ValueError): rebuild.validate_ledger(ledger,self.root)
        ledger=copy.deepcopy(self.ledger);ledger['functions'][0]['call_bindings']=[]
        with self.assertRaises(ValueError):rebuild.validate_ledger(ledger,self.root)

    def test_exact_historical_marker_metadata_only(self):
        self.prepare()
        funcs=[('source.cpp',0,0,4,0,0,0xfff1),('gcc2_compiled.',0,0,0,0,0,1),
               ('__gnu_compiled_cplusplus',0,0,0,0,0,1),('candidate',0,len(self.code),2,1,0,1)]
        obj=bytearray(call_elf_fixture(code=self.code,functions=funcs,relocations=()))
        _, rel=rebuild.Elf32(obj).named_section('.rel.pdr')
        struct.pack_into('<II',obj,rel[4],0,0x402)
        struct.pack_into('<I',obj,36,rebuild.HIST_OBJECT_FLAGS)
        obj=bytes(obj)
        elf=rebuild.Elf32(obj)
        elf.rebuilt('candidate')
        elf.call_symbols(elf.function('candidate')[0],[],historical=True)
        with self.assertRaises(ValueError): elf.call_symbols(elf.function('candidate')[0],[])
        for changes in ({'value':4},{'size':4},{'binding':1},{'section':3},{'type':2},{'other':1}):
            bad=change_symbol(obj,'gcc2_compiled.',**changes)
            with self.subTest(changes=changes),self.assertRaises(ValueError):
                altered=rebuild.Elf32(bad)
                altered.call_symbols(altered.function('candidate')[0],[],historical=True)
        for boundary in (0,2,3,5):
            altered=rebuild.Elf32(change_section(obj,'.symtab',7,boundary))
            with self.assertRaises(ValueError):altered.call_symbols(altered.function('candidate')[0],[],historical=True)
        renamed=obj.replace(b'gcc2_compiled.',b'bad2_compiled.')
        swapped=bytearray(obj)
        table=elf.sections[elf.function('candidate')[0]['table']]
        left,right=table[4]+2*16,table[4]+3*16
        swapped[left:left+16],swapped[right:right+16]=swapped[right:right+16],swapped[left:left+16]
        for bad in (renamed,bytes(swapped)):
            altered=rebuild.Elf32(bad)
            with self.assertRaises(ValueError):altered.call_symbols(altered.function('candidate')[0],[],historical=True)

    def test_no_stale_or_missing_stage_outputs(self):
        self.prepare()
        self.output.mkdir()
        (self.output/'fresh.o').write_bytes(self.object())
        (self.output/'fresh.s').write_bytes(b'stale assembly')
        for phase in ('compile','assemble'):
            def remove_output(stage,command):
                if stage==phase:Path(command[-1]).unlink()
            report=self.verify(mutation=remove_output)
            self.assertEqual(report['functions'][0]['status'],'failed')
        def precreate_object(stage,command):
            if stage=='model-as':Path(command[-1]).with_name('fresh.o').write_bytes(self.object())
        report=self.verify(mutation=precreate_object)
        self.assertEqual(report['functions'][0]['status'],'failed')
        self.assertIn('fresh',report['functions'][0]['reason'])

    def test_mixed_clang_ee_and_historical_keep_distinct_provenance(self):
        self.prepare()
        historic=copy.deepcopy(self.ledger['functions'][0])
        RebuildTests.prepare_ee(self)
        ee_source=self.root/'ee.cpp';ee_source.write_text('extern "C" int ee_leaf(){return 12;}')
        clang_source=self.root/'clang.cpp';clang_source.write_text('extern "C" int clang_leaf(){return 13;}')
        ee_code=CALLEE;clang_code=struct.pack('<2I',0x03e00008,0x2402000d)
        self.ledger['functions']=[historic,
            dict(historic,id='ee',source='ee.cpp',symbol='ee_leaf',original_symbol='ee_original',
                 address='0x00100018',size=8,toolchain='ee-gcc',compiler_args=sorted(rebuild.EE_FLAGS)),
            dict(historic,id='clang',source='clang.cpp',symbol='clang_leaf',original_symbol='clang_original',
                 address='0x00100020',size=8,toolchain='clang',compiler_args=sorted(rebuild.REQUIRED_FLAGS)+['-O2'])]
        for item in self.ledger['functions'][1:]: del item['comparison_mode']
        self.game.write_bytes(call_elf_fixture(kind=2,code=self.code+ee_code+clang_code,functions=[
            ('original',BASE,24,2,1,0,1),('ee_original',BASE+24,8,2,1,0,1),('clang_original',BASE+32,8,2,1,0,1)]))
        self.target['text']['size']=40;self.target['baseline_sha256']=rebuild.digest(self.game.read_bytes())
        self.ledger['target']=copy.deepcopy(self.target)
        def clang_run(command,**kwargs):
            if command[-1]=='--version':return subprocess.CompletedProcess(command,0,b'clang version 20.1.8\n',b'')
            self.assertEqual(Path(command[-3]).read_bytes(),clang_source.read_bytes())
            Path(command[-1]).write_bytes(elf_fixture(code=clang_code,symbol='clang_leaf'))
            return subprocess.CompletedProcess(command,0,b'',b'')
        with patch.object(rebuild,'BASELINE_SIZE',self.game.stat().st_size),self.fixture():
            historic_run=rebuild.subprocess.run.side_effect
            with RebuildTests.ee_fixture(self,source=ee_source,object_data=elf_fixture(code=ee_code,pdr=True,symbol='ee_leaf')):
                ee_run=rebuild.subprocess.run.side_effect
                def dispatch(command,**kwargs):
                    run=historic_run if command[0] in (str(self.driver),str(self.assembler)) else ee_run if command[0]==str(self.ee_driver) else clang_run
                    return run(command,**kwargs)
                with patch.object(rebuild.subprocess,'run',side_effect=dispatch),patch.object(rebuild.shutil,'which',side_effect=str):
                    report=rebuild.verify(self.ledger,self.game,self.compiler,self.output,self.root,ee_compiler=self.ee_driver,
                        historical_compiler=self.driver,historical_assembler=self.assembler)
                    with self.assertRaisesRegex(ValueError,'missing historical'):
                        rebuild.verify(self.ledger,self.game,self.compiler,self.output,self.root,ee_compiler=self.ee_driver)
        self.assertEqual([r['status'] for r in report['functions']],['matched']*3)
        self.assertEqual(len(rebuild.recorded_matches(report,self.ledger,self.root)),3)
        self.assertEqual(len({r['toolchain_sha256'] for r in report['functions']}),3)
        report['functions'][1]['toolchain_sha256']=report['functions'][0]['toolchain_sha256']
        with self.assertRaisesRegex(ValueError,'toolchain identity'):
            rebuild.recorded_matches(report,self.ledger,self.root)

    def test_branch_likely_and_ordinary_delays_are_bounded(self):
        # Independent branch fixture: delay instruction has an observable write;
        # both taken and false paths join a final JR with its own store delay.
        for op in (4,5,6,7,20,21,22,23):
            code=struct.pack('<6I',(op<<26)|(4<<21)|2,0x24020001,0x24020002,0,0x03e00008,0xac820000)
            rebuild.audit_historical_body(code)
        # A branch may target the next ordinary branch (Light's structural case).
        rebuild.audit_historical_body(struct.pack('<8I',0x50800002,0x24020001,0,0x50800002,0,0,0x03e00008,0))

    def test_invalid_cfg_and_reserved_encodings_rejected(self):
        good=[0x50800002,0x24020001,0x24020002,0,0x03e00008,0]
        variants=[]
        for word in (0x5080ffff,0x50800000,0x50800004,0x58810002,0x0c000000,0x08000000,0x45000000,
                     0x70000000,0x279c0001,0x27bdfff0,0x26100001,0x241f0000,0x0000000c,0x0000000d):
            changed=list(good);changed[0]=word;variants.append(changed)
        changed=list(good);changed[1]=0x10800001;variants.append(changed)
        changed=list(good);changed[0]=0x50800004;variants.append(changed) # JR delay slot
        # First branch targets delay slot of a second branch.
        variants.append([0x50800003,0,0,0x50800002,0,0,0x03e00008,0])
        for words in variants:
            with self.subTest(words=words),self.assertRaises(ValueError):
                rebuild.audit_historical_body(struct.pack('<'+'I'*len(words),*words))


if __name__ == '__main__':
    unittest.main()
