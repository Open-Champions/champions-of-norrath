#!/usr/bin/env python3
"""Check the w12-startup-rt-c overlay buffer object.

Usage: w12-startup-rt-c-data-check.py <ee-toolchain-dir>

Compiles experimental/sources/w12-startup-rt-c-overlay-iop-buffers/iop_buffers.cpp with the
reviewed authored_cpp flags and checks sizes, section and aliasing with nm.
"""
import subprocess, sys, tempfile, pathlib

tool = pathlib.Path(sys.argv[1]) / "bin" / "mips64r5900el-ps2-elf"
root = pathlib.Path(__file__).resolve().parents[2]
src = root / "experimental/sources/w12-startup-rt-c-overlay-iop-buffers/iop_buffers.cpp"
flags = ("-G0 -O2 -ffreestanding -fno-asynchronous-unwind-tables -fno-exceptions -fno-pic -fno-rtti "
         "-fno-stack-protector -fno-unwind-tables -fomit-frame-pointer -fwrapv -mabi=eabi -march=r5900 "
         "-mfp32 -mgp64 -mlong32 -mno-abicalls -msingle-float -std=c++17 -nostdinc -nostdinc++ -x c++").split()
with tempfile.TemporaryDirectory() as d:
    obj = pathlib.Path(d) / "iop_buffers.o"
    subprocess.run([str(tool) + "-g++", *flags, "-c", str(src), "-o", str(obj)], check=True)
    out = subprocess.run([str(tool) + "-nm", "-S", str(obj)], check=True, capture_output=True, text=True).stdout
syms = {}
for line in out.splitlines():
    addr, size, kind, name = line.split()
    syms[name] = (int(addr, 16), int(size, 16), kind)
expect = {"sSend": 512, "sRecv": 512, "sFunctionIndex": 8, "sIopFunctionData": 512, "sSifClient": 56}
for name, size in expect.items():
    assert syms[name][1] == size and syms[name][2] == "B", (name, syms.get(name))
for alias, real in (("norrath_overlay_sSend_11edb00", "sSend"),
                    ("norrath_overlay_sFunctionIndex_11edf00", "sFunctionIndex"),
                    ("norrath_overlay_sIopFunctionData_11edf08", "sIopFunctionData")):
    assert syms[alias] == syms[real], (alias, syms[alias], syms[real])
assert syms["sSend"][0] % 64 == 0 and syms["sRecv"][0] % 64 == 0
assert len({syms[n][0] for n in expect}) == len(expect) and all(
    a[0] + a[1] <= b[0] or b[0] + b[1] <= a[0]
    for i, n in enumerate(expect) for m in list(expect)[i + 1:] for a, b in [(syms[n], syms[m])])
print("overlay buffer object ok")
