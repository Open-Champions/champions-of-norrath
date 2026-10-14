#!/usr/bin/env python3
"""Compare compiled data definitions with the private executable.

usage: w8-audio-calls-data-check.py <SLUS_205.65> <compiled .o> name=address[,size] ...

Reads only the ELF symbol tables and section bytes. For each name the compiled
object must define the symbol with the given size (default: the executable's
st_size), and for symbols in initialized sections the bytes must equal the
executable's bytes at the given address. NOBITS sections compare as zeros.
Prints nothing from the executable except the pass/fail verdict.
"""
import struct
import sys


def load(path):
    data = open(path, "rb").read()
    assert data[:4] == b"\x7fELF" and data[4] == 1 and data[5] == 1
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2E)
    secs = []
    for i in range(shnum):
        f = struct.unpack_from("<10I", data, shoff + i * shentsize)
        secs.append(dict(name=f[0], type=f[1], flags=f[2], addr=f[3], off=f[4], size=f[5], link=f[6], entsize=f[9]))
    return data, secs


def cstr(blob, off):
    return blob[off:blob.index(b"\0", off)].decode()


def symbols(data, secs):
    out = []
    for sec in secs:
        if sec["type"] != 2:
            continue
        strsec = secs[sec["link"]]
        strtab = data[strsec["off"]:strsec["off"] + strsec["size"]]
        for i in range(sec["size"] // 16):
            n, value, size, info, other, shndx = struct.unpack_from("<IIIBBH", data, sec["off"] + i * 16)
            out.append((cstr(strtab, n), value, size, shndx))
    return out


def main():
    exe_path, obj_path, *specs = sys.argv[1:]
    exe, exe_secs = load(exe_path)
    obj, obj_secs = load(obj_path)
    exe_syms = symbols(exe, exe_secs)
    obj_syms = {n: (v, s, x) for n, v, s, x in symbols(obj, obj_secs) if x != 0}
    failures = 0
    for spec in specs:
        name, rest = spec.split("=")
        parts = rest.split(",")
        addr = int(parts[0], 16)
        size = int(parts[1]) if len(parts) > 1 else None
        match = [s for s in exe_syms if s[1] == addr and s[3] != 0]
        if size is None:
            size = max(s[2] for s in match)
        if name not in obj_syms:
            print("FAIL", name, "not defined")
            failures += 1
            continue
        ov, osz, oshndx = obj_syms[name]
        if osz != size:
            print("FAIL", name, "size", osz, "expected", size)
            failures += 1
            continue
        osec = obj_secs[oshndx]
        if osec["type"] == 8:
            obytes = bytes(size)
        else:
            obytes = obj[osec["off"] + ov:osec["off"] + ov + size]
        esec = next(s for s in exe_secs if s["addr"] <= addr < s["addr"] + s["size"] and s["flags"] & 2)
        if esec["type"] == 8:
            ebytes = bytes(size)
        else:
            ebytes = exe[esec["off"] + addr - esec["addr"]:esec["off"] + addr - esec["addr"] + size]
        if obytes != ebytes:
            print("FAIL", name, "initial bytes differ")
            failures += 1
        else:
            print("ok", name, size)
    sys.exit(1 if failures else 0)


main()
