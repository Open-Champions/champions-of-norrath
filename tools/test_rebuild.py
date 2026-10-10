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
            self.assertEqual(snapshot.read_bytes(), (source or self.source).read_bytes())
            Path(command[-1]).write_bytes(elf_fixture(pdr=True) if object_data is None else object_data)
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

    def test_cli_help(self):
        result = subprocess.run([sys.executable, '-I', '-B', rebuild.__file__, '--help'], capture_output=True, text=True, timeout=15)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('verify', result.stdout)


if __name__ == '__main__':
    unittest.main()
