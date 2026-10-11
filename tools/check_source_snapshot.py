"""Check publication integrity only; this is not game reconstruction verification."""
import hashlib
import json
from pathlib import Path
import re


def check(root):
    directory = root / 'experimental'
    snapshot = json.loads((directory / 'snapshot.json').read_text())
    assert snapshot['schema'] == 1
    files = snapshot['files']
    for name, expected in files.items():
        path = directory / name
        assert name.startswith('sources/') and '..' not in Path(name).parts
        assert path.suffix in ('.cpp', '.hpp', '.S') and not path.is_symlink()
        assert hashlib.sha256(path.read_bytes()).hexdigest() == expected, name
        text = path.read_text()
        for include in re.findall(r'^\s*#\s*include\s*"([^"\n]+)"', text, re.M):
            assert not Path(include).is_absolute(), (name, include)
            header = (path.parent / include).resolve()
            relative = header.relative_to(directory.resolve()).as_posix()
            assert relative in files, (name, include)
    actual = {p.relative_to(directory).as_posix() for p in
              (directory / 'sources').rglob('*') if p.is_file()}
    assert actual == set(files), 'Unrecorded or missing source files'
    inputs = snapshot['inputs']
    assert len(inputs) == 264
    assert [r['input_order'] for r in inputs] == list(range(264))
    authored = [r for r in inputs if r['kind'] == 'independently_authored_source']
    external = [r for r in inputs if r['kind'] == 'external_upstream_source']
    assert len(authored) == 262 and len(external) == 2
    assert all(r['source'] in files for r in authored)
    assert all(r['vendored'] is False for r in external)
    assert sum(len(r['emitted_functions']) for r in inputs) == 388
    assert sum(f['compiled_body_bytes'] for r in inputs
               for f in r['emitted_functions']) == 231604
    integration = snapshot['partial_integration']
    assert integration['additional_exact_original_matches'] == 0
    assert integration['runtime_executed'] is False
    assert integration['clean_checkout_game_build'] is False
    additions = snapshot.get('additional_implementations', [])
    assert sum(len(item['functions']) for item in additions) == 7
    additional_functions = [name for item in additions for name in item['functions']]
    assert len(set(additional_functions)) == 7
    assert set(additional_functions) == {
        'norrath_list_safe_iterator_construct', 'norrath_list_safe_iterator_next',
        'norrath_list_safe_iterator_destroy', 'norrath_megatile_add_pm_list',
        'norrath_script_set_script', 'norrath_megatile_cap_pm_pointers',
        'norrath_object_process_delete_list',
    }
    for item in additions:
        assert all(name in files for name in item['files'])
        assert item['additional_exact_original_matches'] == 0
        assert item['runtime_executed'] is False
    by_component = {item['component']: item for item in additions}
    for component, function, body_bytes in (
        ('game-script-set-script-source', 'norrath_script_set_script', 84),
        ('game-megatile-pm-pointer-cap-source', 'norrath_megatile_cap_pm_pointers', 1080),
        ('game-object-process-delete-list-source', 'norrath_object_process_delete_list', 496),
    ):
        item = by_component[component]
        assert item['functions'] == [function]
        assert item['first_compiled_body_bytes'] == body_bytes
        for field in ('private_FIRST_manifest_sha256', 'first_compiled_object_sha256',
                      'private_ROOT_actual_review_manifest_sha256'):
            assert re.fullmatch(r'[0-9a-f]{64}', item[field]), (component, field)
        assert not set(item['files']) & {entry['source'] for entry in authored}, component
    deletion = by_component['game-object-process-delete-list-source']
    assert deletion['callback_scope'] == {
        'declared_arguments': 2, 'declared_return': 'void', 'flags_argument': 3,
        'incidental_A2_A3_verified': False,
        'arbitrary_native_signatures_verified': False,
        'native_physical_frame_upper_register_lifetime_IRQ_fault_verified': False,
        'runtime_verified': False,
    }
    assert all(term in deletion['scope'] for term in
               ('ordinary-RAM', 'incidental A2/A3', 'physical access/frame', 'lifetime', 'IRQ/fault'))
    deletion_header = (directory / 'sources/game-object-process-delete-list-source/process_delete.hpp').read_text()
    for declaration in (
        'void norrath_object_process_delete_list();',
        'void norrath_list_safe_iterator_construct(unsigned char* iterator, unsigned char* head, int mask);',
        'void norrath_list_safe_iterator_next(unsigned char* iterator);',
        'void norrath_list_safe_iterator_destroy(unsigned char* iterator, int flags);',
        'using Callback = void (*)(unsigned char*, int);',
    ):
        assert declaration in deletion_header, declaration
    latest = snapshot['latest_private_partial_integration']
    assert (latest['revision'], latest['input_objects'],
            latest['emitted_function_definitions_including_helpers'],
            latest['compiled_body_bytes'], latest['encoded_relocation_fields'],
            latest['unresolved_imports']) == ('V46', 271, 395, 234600, 5384, 827)
    assert latest['same_mutable_owners'] == 8 and latest['old_code_and_storage_preserved'] is True
    assert latest['fresh_reproduction_performed'] is False and latest['runtime_verified'] is False
    assert latest['additional_exact_original_matches'] == 0
    for field in ('object_sha256', 'private_FIRST_manifest_sha256', 'private_ROOT_manifest_sha256'):
        assert re.fullmatch(r'[0-9a-f]{64}', latest[field]), field
    behavior = by_component['game-megatile-pm-pointer-cap-source']['bounded_behavior']
    assert behavior['fixtures'] == behavior['paired_calls'] == 12
    assert behavior['comparison_runs'] == 1
    for field in ('persistent_pointer_counter_memory_matched', 'returned_low32_pointer_matched',
                  'each_count_publication_matched', 'saved_low64_SP_GP_matched',
                  'sparse_alias_probes_do_not_establish_pool_capacity_or_game_inputs'):
        assert behavior[field] is True, field
    assert (behavior['original_words_covered'], behavior['original_whole_words']) == (97, 99)
    assert (behavior['compiled_words_covered'], behavior['compiled_whole_words']) == (256, 270)
    assert behavior['earlier_cases_replayed'] is False
    assert behavior['provider_calls'] == behavior['new_ISA'] == 0
    assert behavior['native_upper128_physical_frame_IRQ_fault_or_hardware_gameplay_verified'] is False
    transport = directory / 'sources/game-amx-exec-native-transport-source-v4'
    for name in ('exec.cpp', 'context.cpp', 'transport.S'):
        assert re.search(r'^#error .*HOLD', (transport / name).read_text(), re.M), name
    return len(files)


if __name__ == '__main__':
    count = check(Path(__file__).resolve().parents[1])
    print(f'PASS: {count} source/header files; 262 authored inputs, 2 upstream references; '
          'draft holds retained. No game match or runtime verification performed.')
