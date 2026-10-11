"""Compile each save/network/animation view and check its EE object."""

import struct
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
VIEWS = ROOT / "experimental/sources/save-net-anim-views"
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
    data = path.read_bytes()
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


def compile_and_read(source_name):
    with tempfile.TemporaryDirectory() as directory:
        output = Path(directory) / "view.o"
        subprocess.run([str(EE), *FLAGS, str(VIEWS / source_name), "-o", str(output)], check=True)
        return elf(output)


def defined(symbols, name):
    rows = [item for item in symbols if item["name"] == name and item["shndx"] not in (0, 0xfff1)]
    if len(rows) != 1:
        raise AssertionError(f"{name} definitions: {rows}")
    return rows[0]


class ViewTests(unittest.TestCase):
    def object_bytes(self, data, sections, symbol):
        section = sections[symbol["shndx"]]
        if section["type"] == 8:
            return bytes(symbol["size"])
        start = section["offset"] + symbol["value"]
        return data[start:start + symbol["size"]]

    def expect(self, filename, symbol, size, content=None, require_zero=False):
        data, sections, symbols, relocations = compile_and_read(filename)
        row = defined(symbols, symbol)
        self.assertEqual(row["size"], size, symbol)
        self.assertGreaterEqual(row["info"] >> 4, 1, symbol)
        actual = self.object_bytes(data, sections, row)
        self.assertEqual(len(actual), size, symbol)
        if content is not None:
            self.assertEqual(actual, content, symbol)
        elif require_zero:
            self.assertEqual(actual, bytes(size), symbol)
        return data, sections, symbols, relocations

    def test_network_mode_byte(self):
        self.expect("norrath_network_mode_byte_view.cpp", "norrath_network_mode_byte_view", 1, b"\0")

    def test_hud_no_controller_byte(self):
        self.expect("norrath_game_hud_no_controller_byte_view.cpp",
                    "norrath_game_hud_no_controller_byte_view", 1, b"\0")

    def test_disable_startup_script(self):
        self.expect("norrath_mc_disable_startup_script_bytes.cpp",
                    "norrath_mc_disable_startup_script_bytes", 1, b"\0")

    def test_lump_format(self):
        self.expect("norrath_mc_lump_format_bytes.cpp", "norrath_mc_lump_format_bytes", 7, b"%s.lmp\0")


if __name__ == "__main__":
    unittest.main()
