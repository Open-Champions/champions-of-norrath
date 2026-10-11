"""EE and host checks for the card-progress package. Not a partial-build input."""

import struct
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SOURCES = ROOT / "experimental/sources"
EE = Path(
    "/Users/syndicatemike/Documents/Codex/projects/open-champions/"
    "champions-of-norrath/work/toolchains/ee/bin/mips64r5900el-ps2-elf-g++"
)
FLAGS = [
    "-G0", "-O2", "-ffreestanding", "-fno-asynchronous-unwind-tables",
    "-fno-exceptions", "-fno-pic", "-fno-rtti", "-fno-stack-protector",
    "-fno-unwind-tables", "-fomit-frame-pointer", "-fwrapv", "-mabi=eabi",
    "-march=r5900", "-mfp32", "-mgp64", "-mlong32", "-mno-abicalls",
    "-msingle-float", "-std=c++17", "-nostdinc", "-nostdinc++", "-c",
]


def elf(path):
    data = Path(path).read_bytes()
    if data[:7] != b"\x7fELF\x01\x01\x01":
        raise AssertionError("not ELF32")
    e_shoff = struct.unpack_from("<I", data, 32)[0]
    e_shentsize, e_shnum, e_shstrndx = struct.unpack_from("<HHH", data, 46)
    sections = []
    for index in range(e_shnum):
        off = e_shoff + index * e_shentsize
        sections.append(struct.unpack_from("<10I", data, off))
    names = data[sections[e_shstrndx][4]:sections[e_shstrndx][4] + sections[e_shstrndx][5]]

    def cstring(table, offset):
        end = table.find(b"\0", offset)
        return table[offset:end].decode()

    parsed = []
    for section in sections:
        parsed.append({
            "name": cstring(names, section[0]),
            "type": section[1],
            "flags": section[2],
            "addr": section[3],
            "offset": section[4],
            "size": section[5],
            "link": section[6],
            "info": section[7],
            "align": section[8],
            "entsize": section[9],
        })
    symbols = []
    symbol_tables = {}
    relocation_blobs = []
    for index, section in enumerate(parsed):
        blob = data[section["offset"]:section["offset"] + section["size"]]
        if section["type"] == 2:
            strings = parsed[section["link"]]
            text = data[strings["offset"]:strings["offset"] + strings["size"]]
            rows = []
            for at in range(0, len(blob), 16):
                nameoff, value, size, info, other, shndx = struct.unpack_from("<IIIBBH", blob, at)
                row = {"name": cstring(text, nameoff), "value": value, "size": size,
                       "info": info, "shndx": shndx}
                rows.append(row)
                symbols.append(row)
            symbol_tables[index] = rows
        elif section["type"] == 9 and section["entsize"] == 8:
            relocation_blobs.append((section["name"], blob, section["link"]))
    resolved = []
    for name, blob, link in relocation_blobs:
        rows = symbol_tables[link]
        for at in range(0, len(blob), 8):
            offset, info = struct.unpack_from("<II", blob, at)
            resolved.append({"section": name, "offset": offset, "type": info & 0xff,
                             "symbol": rows[info >> 8]["name"]})
    resolved.sort(key=lambda item: (item["section"], item["offset"]))
    return data, parsed, symbols, resolved


def compile_ee(source):
    with tempfile.TemporaryDirectory() as directory:
        output = Path(directory) / "unit.o"
        subprocess.run([str(EE), *FLAGS, str(source), "-o", str(output)], check=True)
        return elf(output)


def defined(symbols, name):
    rows = [item for item in symbols if item["name"] == name and item["shndx"] not in (0, 0xfff1, 0xfff2)]
    if len(rows) != 1:
        raise AssertionError(f"{name} definitions: {rows}")
    return rows[0]


def object_bytes(data, sections, symbol):
    section = sections[symbol["shndx"]]
    if section["type"] == 8:
        return bytes(symbol["size"])
    start = section["offset"] + symbol["value"]
    return data[start:start + symbol["size"]]


def undefined(symbols):
    return sorted(item["name"] for item in symbols if item["shndx"] == 0 and item["name"])


class CardTest(unittest.TestCase):
    def expect_object(self, source, symbol, size, content=None, imports=()):
        data, sections, symbols, relocations = compile_ee(source)
        row = defined(symbols, symbol)
        self.assertEqual(row["size"], size, symbol)
        self.assertGreaterEqual(row["info"] >> 4, 1, symbol)
        self.assertLess(row["shndx"], 0xff00, symbol)
        actual = object_bytes(data, sections, row)
        self.assertEqual(len(actual), size, symbol)
        if content is not None:
            self.assertEqual(actual, content, symbol)
        self.assertEqual(undefined(symbols), sorted(imports), symbol)
        return data, sections, symbols, relocations

    def host(self, sources, driver):
        with tempfile.TemporaryDirectory() as directory:
            directory = Path(directory)
            driver_path = directory / "driver.cpp"
            driver_path.write_text(driver)
            binary = directory / "driver"
            subprocess.run(
                ["c++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                 *[str(path) for path in sources], str(driver_path), "-o", str(binary)],
                check=True)
            subprocess.run([str(binary)], check=True)
