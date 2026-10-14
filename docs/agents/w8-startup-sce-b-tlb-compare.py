#!/usr/bin/env python3
"""Byte comparison of _kExitTLBHandler against the private executable.

Compiles experimental/sources/w8-startup-sce-b-k-exit-tlb-handler/k_exit_tlb_handler.cpp
with the reviewed EE toolchain, links it at the original function address with the
exception frame symbol defined at the original static block, and compares the 244
text bytes with the same range of the private SLUS_205.65. Nothing from the
executable is written anywhere; scratch files live in a fresh temporary directory.

  NORRATH_EXE=/path/SLUS_205.65 NORRATH_EE=/path/to/ee \\
      python3 docs/agents/w8-startup-sce-b-tlb-compare.py

Exit status 0 means the 244 bytes are identical. It is a function-body comparison
only and is not an entry in the exact-match ledger.
"""
import os
import subprocess
import sys
import tempfile

ADDRESS = 0x002E0100
LENGTH = 244
FRAME = 0x008E5780
TEXT_BASE = 0x00100000
TEXT_OFFSET = 0x1000
SOURCE = "experimental/sources/w8-startup-sce-b-k-exit-tlb-handler/k_exit_tlb_handler.cpp"
FLAGS = ("-G0 -O2 -ffreestanding -fno-asynchronous-unwind-tables -fno-exceptions -fno-pic "
         "-fno-rtti -fno-stack-protector -fno-unwind-tables -fomit-frame-pointer -fwrapv "
         "-mabi=eabi -march=r5900 -mfp32 -mgp64 -mlong32 -mno-abicalls -msingle-float "
         "-std=c++17 -nostdinc -nostdinc++").split()


def main():
    exe = os.environ["NORRATH_EXE"]
    tools = os.environ["NORRATH_EE"] + "/bin/mips64r5900el-ps2-elf-"
    root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    with tempfile.TemporaryDirectory() as scratch:
        obj = os.path.join(scratch, "unit.o")
        elf = os.path.join(scratch, "unit.elf")
        raw = os.path.join(scratch, "unit.bin")
        subprocess.run([tools + "g++", *FLAGS, "-x", "c++", "-c", os.path.join(root, SOURCE), "-o", obj], check=True)
        subprocess.run([tools + "ld", "-m", "elf32lr5900", "-Ttext=0x%x" % ADDRESS,
                        "--defsym", "norrath_ee_exception_frame=0x%x" % FRAME,
                        "-e", "_kExitTLBHandler", obj, "-o", elf], check=True)
        subprocess.run([tools + "objcopy", "-O", "binary", "-j", ".text", elf, raw], check=True)
        mine = open(raw, "rb").read()
    with open(exe, "rb") as handle:
        handle.seek(TEXT_OFFSET + ADDRESS - TEXT_BASE)
        original = handle.read(LENGTH)
    if len(mine) != LENGTH or mine != original:
        print("_kExitTLBHandler differs: %d bytes built, %d expected" % (len(mine), LENGTH))
        return 1
    print("_kExitTLBHandler: %d bytes identical" % LENGTH)
    return 0


if __name__ == "__main__":
    sys.exit(main())
