"""Assemble i_wakeup_thread_syscall.S and check its four instruction words against the contract in its
header: load service number -52 into v1, syscall, return, nop. Not a build input.
usage: check_words.py <ee-toolchain-root>
"""
import re, subprocess, sys, tempfile, pathlib
root = pathlib.Path(sys.argv[1])
src = pathlib.Path(__file__).resolve().parents[2] / 'experimental/sources/w10-startup-b-i-wakeup-thread-syscall' / 'i_wakeup_thread_syscall.S'
with tempfile.TemporaryDirectory() as t:
    obj = pathlib.Path(t) / 'x.o'
    subprocess.check_call([str(root / 'bin/mips64r5900el-ps2-elf-gcc'), '-c', '-x', 'assembler-with-cpp', str(src), '-o', str(obj)])
    out = subprocess.check_output([str(root / 'bin/mips64r5900el-ps2-elf-objdump'), '-d', '-z', str(obj)]).decode()
words = [int(m.group(1), 16) for m in re.finditer(r'^\s*[0-9a-f]+:\t([0-9a-f]{8}) ', out, re.M)]
want = [0x24030000 | ((-52) & 0xffff), 0x0000000c, 0x03e00008, 0x00000000]
assert words == want, (words, want)
print('_iWakeupThread: 4 words match the stated contract')
