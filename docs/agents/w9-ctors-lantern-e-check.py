#!/usr/bin/env python3
"""Check the w9-ctors-lantern-e single-float definitions.

usage: w9-ctors-lantern-e-check.py <SLUS_205.65> <ee-toolchain> <outdir> <addr-hex>...

For each address: compile the source with the reviewed EE g++ authored_cpp flags,
require a four-byte object symbol, compare its bytes with the executable's bytes
at the address, then build and run the host comparison against the same source.
Prints only pass/fail verdicts.
"""
import os, struct, subprocess, sys

FLAGS = ["-G0", "-O2", "-ffreestanding", "-fno-asynchronous-unwind-tables", "-fno-exceptions", "-fno-pic",
         "-fno-rtti", "-fno-stack-protector", "-fno-unwind-tables", "-fomit-frame-pointer", "-fwrapv",
         "-mabi=eabi", "-march=r5900", "-mfp32", "-mgp64", "-mlong32", "-mno-abicalls", "-msingle-float",
         "-std=c++17", "-nostdinc", "-nostdinc++", "-x", "c++"]
root = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "experimental", "sources")


def elf(path):
    data = open(path, "rb").read()
    shoff, = struct.unpack_from("<I", data, 0x20)
    es, n, _ = struct.unpack_from("<HHH", data, 0x2E)
    return data, [struct.unpack_from("<10I", data, shoff + i * es) for i in range(n)]


def exe_word(addr):
    for s in exe_secs:
        if s[3] and s[3] <= addr < s[3] + s[5] and s[1] == 1:
            return exe[s[4] + addr - s[3]:s[4] + addr - s[3] + 4]


exe_path, tc, out, *addrs = sys.argv[1:]
exe, exe_secs = elf(exe_path)
os.makedirs(out, exist_ok=True)
bad = 0
for h in addrs:
    sym = "norrath_ctors_ant_lantern_view_" + h
    src = next(os.path.join(root, g, sym + ".cpp") for g in sorted(os.listdir(root))
               if g.startswith("w9-ctors-lantern-e-") and g.endswith("-views") and os.path.exists(os.path.join(root, g, sym + ".cpp")))
    obj = os.path.join(out, sym + ".o")
    subprocess.run([tc + "/bin/mips64r5900el-ps2-elf-g++", *FLAGS, "-c", src, "-o", obj], check=True)
    data, secs = elf(obj)
    found = None
    for s in secs:
        if s[1] == 2:
            st = secs[s[6]]
            for i in range(s[5] // 16):
                o, v, sz, info, oth, sh = struct.unpack_from("<IIIBBH", data, s[4] + i * 16)
                if data[st[4] + o:data.index(b"\0", st[4] + o)].decode() == sym and sh:
                    found = (v, sz, secs[sh])
    ok = bool(found) and found[1] == 4 and data[found[2][4] + found[0]:found[2][4] + found[0] + 4] == exe_word(int(h, 16))
    host = os.path.join(root, "w9-ctors-lantern-e-host-checks", sym + ".cpp")
    exe_h = os.path.join(out, sym + ".host")
    subprocess.run(["c++", "-std=c++17", src, host, "-o", exe_h], check=True)
    ok = ok and subprocess.run([exe_h]).returncode == 0
    print("ok" if ok else "FAIL", sym)
    bad += not ok
sys.exit(1 if bad else 0)
