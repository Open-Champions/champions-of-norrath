"""Assemble sce_idc.S and check the structure its header states. Not a build input.
usage: check_structure.py <ee-toolchain-root>
"""
import pathlib, re, subprocess, sys, tempfile
root = pathlib.Path(sys.argv[1])
src = pathlib.Path(__file__).resolve().parents[2] / 'experimental/sources/w10-startup-b-sce-idc' / 'sce_idc.S'
with tempfile.TemporaryDirectory() as t:
    obj = pathlib.Path(t) / 'x.o'
    subprocess.check_call([str(root / 'bin/mips64r5900el-ps2-elf-gcc'), '-c', '-x', 'assembler-with-cpp', str(src), '-o', str(obj)])
    out = subprocess.check_output([str(root / 'bin/mips64r5900el-ps2-elf-objdump'), '-d', '-z', str(obj)]).decode()
rows = re.findall(r'^[ \t]*([0-9a-f]+):\t([0-9a-f]{8}) +\t(\S+)[ \t]*(.*)$', out, re.M)
rows = rows[:41]  # the section is padded to 8 bytes after the 164-byte body
assert len(rows) == 41
ops = [r[2] for r in rows]
assert ops.count('cache') == 4 and ops.count('mfc0') == 2 and ops.count('sync') == 9, ops
caches = [r[3] for r in rows if r[2] == "cache"]
assert caches == ['0x10,0(a2)', '0x16,0(a2)', '0x10,1(a2)', '0x16,1(a2)'], caches
assert any(r[2] == 'slti' and r[3].endswith('4096') for r in rows)
assert any(r[2] == 'mfc0' and 'taglo' in r[3] for r in rows)
assert ops[-2:] == ['jr', 'nop']
print('_sceIDC: structure matches the stated contract')
