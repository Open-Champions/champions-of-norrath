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
    assert actual == set(files) | {'sources/runtime-memory/REVIEW.md'}, 'Unrecorded or missing source files'
    inputs = snapshot['inputs']
    selection = snapshot['selected_source_scope']
    prefix_count = selection['historical_prefix_inputs']
    assert prefix_count == 264
    prefix = inputs[:prefix_count]
    prefix_digest = hashlib.sha256(json.dumps(prefix, sort_keys=True,
                                            separators=(',', ':')).encode()).hexdigest()
    assert prefix_digest == selection['historical_prefix_records_sha256']
    assert prefix_digest == 'cb104d50f455d2885603f1db57d4cecbb349dfce2575b0da56326f3301369af2'
    assert [r['input_order'] for r in inputs] == list(range(len(inputs)))
    authored = [r for r in inputs if r['kind'] == 'independently_authored_source']
    external = [r for r in inputs if r['kind'] == 'external_upstream_source']
    assert len(authored) == selection['authored_default_inputs']
    assert len(external) == selection['optional_external_inputs'] == 2
    assert len(inputs) == selection['selected_inputs']
    historical_end = 279
    assert all(r['source'].endswith('.cpp') for r in inputs[prefix_count:historical_end])
    assert len({r['source'] for r in authored}) == len(authored)
    assert all(r['source'] in files for r in authored)
    assert all(r['vendored'] is False for r in external)
    assert sum(len(r['emitted_functions']) for r in prefix) == 388
    assert sum(f['compiled_body_bytes'] for r in prefix
               for f in r['emitted_functions']) == 231604
    assert sum(len(r['emitted_functions']) for r in inputs) == selection['emitted_function_definitions_including_helpers']
    assert sum(f['compiled_body_bytes'] for r in inputs
               for f in r['emitted_functions']) == selection['compiled_body_bytes_from_accepted_inputs']
    assert selection['coefficient_initializer_inputs'] == 0
    if selection['clean_source_successor_build_performed']:
        receipt_path = directory / selection['expanded_build_receipt']
        assert receipt_path.name == 'build-receipt-expanded.json'
        assert hashlib.sha256(receipt_path.read_bytes()).hexdigest() == selection['expanded_build_receipt_sha256']
        receipt = json.loads(receipt_path.read_text())
        assert receipt['source_inputs'] == receipt['source_compiler_invocations'] == 277
        assert receipt['relocatable_link_invocations'] == 1
        assert receipt['original_game_inputs_used'] is False
        assert receipt['retained_private_objects_used_as_inputs'] is False
        assert receipt['symbol_aliases_applied'] is False
        assert receipt['gameplay_verified'] is False
        assert (receipt['function_bodies'], receipt['function_body_bytes'],
                receipt['unresolved_symbols']) == (401, 241520, 848)
    assert selection['runtime_verified'] is False
    assert 'original264-input V43 prefix' in selection['original_receipt_scope']
    assert hashlib.sha256((directory / 'build-receipt.json').read_bytes()).hexdigest() == 'fd66baac8172c7b4ae3c4fc57ec45a166ecde8c22ad43ed16330542f75614c09'
    integration = snapshot['partial_integration']
    assert integration['additional_exact_original_matches'] == 0
    assert integration['runtime_executed'] is False
    assert integration['clean_checkout_game_build'] is False
    additions = snapshot.get('additional_implementations', [])
    assert sum(len(item['functions']) for item in additions) == selection['additional_selected_inputs']
    additional_functions = [name for item in additions for name in item['functions']]
    assert len(set(additional_functions)) == selection['additional_selected_inputs']
    assert set(additional_functions) == {
        'norrath_list_safe_iterator_construct', 'norrath_list_safe_iterator_next',
        'norrath_list_safe_iterator_destroy', 'norrath_megatile_add_pm_list',
        'norrath_script_set_script', 'norrath_megatile_cap_pm_pointers',
        'norrath_object_process_delete_list',
        'norrath_floor_clear', 'norrath_floor_add', 'norrath_light_floor_sprite',
        'norrath_find_lights_at_point', 'norrath_world_query_light_map',
        'ee_c_lt_s_word', 'norrath_integer_cosine', 'norrath_integer_sine',
    }
    additional_sources = {name for item in additions for name in item['files'] if name.endswith('.cpp')}
    assert {r['source'] for r in inputs[prefix_count:historical_end]} == additional_sources
    assert historical_end == prefix_count + len(additional_sources)
    assert len(authored) == 262 + len(additional_sources) + selection['startup_selected_inputs']
    assert {f['name'] for r in inputs[prefix_count:historical_end] for f in r['emitted_functions']} == set(additional_functions)
    selected_sources = {r['source'] for r in authored}
    assert not selected_sources & {f for draft in snapshot['drafts'] for f in draft['files']}
    assert not any(re.search(r'/(?:icos|isin)_(?:scale|cubic|quintic|septic)\.cpp$', f) for f in files)
    expected_tail = [
        'sources/game-list-safe-iterator-source/construct.cpp',
        'sources/game-list-safe-iterator-source/next.cpp',
        'sources/game-list-safe-iterator-source/destroy.cpp',
        'sources/game-megatile-name-pool-source/add_name.cpp',
        'sources/game-megatile-pm-pointer-cap-source/cap_pointers.cpp',
        'sources/game-script-set-script-source/set_script.cpp',
        'sources/game-object-process-delete-list-source/process_delete.cpp',
        'sources/game-floor-clear-source/floor_clear.cpp',
        'sources/game-floor-add-source-v2/floor_add.cpp',
        'sources/game-floor-lighting-kernel-source/light_floor.cpp',
        'sources/game-floor-lighting-kernel-source/find_lights.cpp',
        'sources/game-floor-lighting-kernel-source/world_light_map.cpp',
        'sources/game-floor-lighting-kernel-source/ee_compare.cpp',
        'sources/game-integer-trig-code-only-source/icos.cpp',
        'sources/game-integer-trig-code-only-source/isin.cpp',
    ]
    assert [r['source'] for r in inputs[prefix_count:historical_end]] == expected_tail
    startup = snapshot['startup_implementations']
    assert len(startup) == selection['startup_selected_inputs'] == len(inputs) - historical_end
    approved = [
        ('sources/runtime-memory/memset.cpp', 'memset'),
        ('sources/runtime-startup/main_guard.cpp', '__main'),
        ('sources/runtime-startup/destructors.cpp', '__do_global_dtors'),
        ('sources/runtime-startup/constructors.cpp', '__do_global_ctors'),
        ('sources/runtime-entry/start.S', '_start'),
        ('sources/runtime-kernel/root.S', '_root'),
        ('sources/runtime-kernel/flush_cache.S', 'FlushCache'),
        ('sources/runtime-kernel/init_system.cpp', '_InitSys'),
        ('sources/runtime-initsys/create_sema.S', 'CreateSema'),
        ('sources/runtime-initsys/delete_sema.S', 'DeleteSema'),
        ('sources/runtime-initsys/supplement_crt0.cpp', 'supplement_crt0'),
        ('sources/runtime-initsys/sif_reboot_notify.cpp', 'sceSifInitRebootNotify'),
        ('sources/runtime-initsys/start_timer_system_time.cpp', 'StartTimerSystemTime'),
        ('sources/runtime-initsys/init_thread.cpp', 'InitThread'),
        ('sources/runtime-initsys/init_exec_ps2.cpp', 'InitExecPS2'),
        ('sources/runtime-initsys/init_alarm.cpp', 'InitAlarm'),
        ('sources/runtime-initsys/init_tlb.cpp', 'InitTLBFunctions'),
        ('sources/runtime-initsys/init_syscall_table.cpp', 'InitSystemCallTableAddress'),
        ('sources/runtime-initsys/init_timer.cpp', 'InitTimer'),
        ('sources/runtime-main/main.cpp', 'main'),
        ('sources/runtime-launchsrs/launch_srs.cpp', 'launchSRS__Fv'),
        ('sources/runtime-initsys-callees/add_intc_handler2.S', 'AddIntcHandler2'),
        ('sources/runtime-initsys-callees/create_thread.S', 'CreateThread'),
        ('sources/runtime-initsys-callees/start_thread.S', 'StartThread'),
        ('sources/runtime-initsys-callees/change_thread_priority.S', 'ChangeThreadPriority'),
        ('sources/runtime-initsys-callees/get_thread_id.S', 'GetThreadId'),
        ('sources/runtime-initsys-callees/find_address.S', 'FindAddress'),
        ('sources/runtime-initsys-callees/dintr.S', 'DIntr'),
        ('sources/runtime-initsys-callees/eintr.S', 'EIntr'),
        ('sources/runtime-initsys-callees/set_t2.S', 'SetT2'),
        ('sources/runtime-initsys-callees/set_t2_count.S', 'SetT2_COUNT'),
        ('sources/runtime-initsys-callees/set_t2_mode.S', 'SetT2_MODE'),
        ('sources/runtime-initsys-callees/set_t2_comp.S', 'SetT2_COMP'),
    ]
    assert [(r['source'], r['symbol']) for r in startup] == approved
    for record, item in zip(startup, inputs[historical_end:]):
        assert record['source'] == item['source']
        assert item['emitted_functions'] == [{'name': record['symbol'],
                                             'compiled_body_bytes': record['compiled_body_bytes']}]
        assert record['source_sha256'] == files[record['source']]
        assert record['object_sha256'] == item['private_object_sha256']
        assert record['game_runtime_verified'] is False
        assert record['exact_match_ledger_admission'] is False
        review = root / record['review']
        assert review.is_file() and review.suffix == '.md'
    provenance = snapshot['source_provenance']
    assert len(provenance) == 15
    for name, record in provenance.items():
        assert name in files and record['sha256'] == files[name]
        assert record['literal_copy'] is True
        assert record['private_source'].startswith('work/')
    expected_versions = {
        'sources/game-floor-add-source-v2/floor_add.cpp': 'work/game-floor-add-source-v2/floor_add.cpp',
        'sources/game-floor-lighting-kernel-source/light_floor.cpp': 'work/game-floor-lighting-source/light_floor.cpp',
        'sources/game-floor-lighting-kernel-source/find_lights.cpp': 'work/game-floor-lighting-source-v3/find_lights.cpp',
        'sources/game-floor-lighting-kernel-source/world_light_map.cpp': 'work/game-floor-lighting-source/world_light_map.cpp',
        'sources/game-floor-lighting-kernel-source/ee_compare.cpp': 'work/game-floor-lighting-source-v2/ee_compare.cpp',
    }
    for name, version in expected_versions.items():
        assert provenance[name]['private_source'] == version
    assert not any(f.startswith(('sources/game-floor-add-source/', 'sources/game-floor-lighting-source/',
                                 'sources/game-floor-lighting-source-v2/', 'sources/game-floor-lighting-source-v3/')) for f in files)
    new_items = additions[-8:]
    assert sum(item['implementation_kind'] == 'complete_game_function' for item in new_items) == selection['new_selected_game_functions'] == 7
    assert sum(item['implementation_kind'] == 'authored_architectural_primitive' for item in new_items) == selection['new_selected_architectural_primitives'] == 1
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
        assert set(item['files']) & selected_sources, component
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
    assert latest['revision'] == 'V47'
    assert (latest['input_objects'], latest['emitted_function_definitions_including_helpers'],
            latest['compiled_body_bytes'], latest['encoded_relocation_fields'],
            latest['unresolved_imports']) == (279, 403, 241992, 5511, 845)
    assert 'pending_private_partial_integration' not in snapshot
    assert (latest['initialized_mutable_data_bytes'], latest['bss_bytes'],
            latest['new_source_constant_pool_bytes'],
            latest['trigonometric_coefficient_imports_unresolved']) == (32, 9344, 40, 8)
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
    root = Path(__file__).resolve().parents[1]
    count = check(root)
    selection = json.loads((root / 'experimental/snapshot.json').read_text())['selected_source_scope']
    print(f"PASS: {count} source/header files; {selection['authored_default_inputs']} authored inputs, "
          f"{selection['optional_external_inputs']} optional upstream references; historical prefix and draft holds retained. "
          'No source build, game match or runtime verification performed.')
