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
    transport = directory / 'sources/game-amx-exec-native-transport-source-v4'
    for name in ('exec.cpp', 'context.cpp', 'transport.S'):
        assert re.search(r'^#error .*HOLD', (transport / name).read_text(), re.M), name
    return len(files)


if __name__ == '__main__':
    count = check(Path(__file__).resolve().parents[1])
    print(f'PASS: {count} source/header files; 262 authored inputs, 2 upstream references; '
          'draft holds retained. No game match or runtime verification performed.')
