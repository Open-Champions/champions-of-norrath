"""Original startup names for save, network, and animation state."""

import subprocess
import unittest
from pathlib import Path

import importlib.util

ROOT = Path(__file__).resolve().parents[2]
STARTUP = ROOT / "experimental/sources/save-net-anim-startup"
VIEWS = ROOT / "experimental/sources/save-net-anim-views"
EE = Path(
    "/Users/syndicatemike/Documents/Codex/projects/open-champions/"
    "champions-of-norrath/work/toolchains/ee/bin/mips64r5900el-ps2-elf-g++"
)
EE_FLAGS = [
    "-G0", "-O2", "-ffreestanding", "-fno-asynchronous-unwind-tables",
    "-fno-exceptions", "-fno-pic", "-fno-rtti", "-fno-stack-protector",
    "-fno-unwind-tables", "-fomit-frame-pointer", "-fwrapv", "-mabi=eabi",
    "-march=r5900", "-mfp32", "-mgp64", "-mlong32", "-mno-abicalls",
    "-msingle-float", "-std=c++17", "-nostdinc", "-nostdinc++", "-c",
]
views_spec = importlib.util.spec_from_file_location(
    "test_views", Path(__file__).with_name("test_views.py"))
views = importlib.util.module_from_spec(views_spec)
views_spec.loader.exec_module(views)


class StartupTests(unittest.TestCase):
    def expect_object(self, filename, symbol, size, content=None, directory=STARTUP):
        if directory == VIEWS:
            data, sections, symbols, _relocations = views.compile_and_read(filename)
        else:
            import tempfile
            with tempfile.TemporaryDirectory() as directory_name:
                output = Path(directory_name) / "unit.o"
                subprocess.run(
                    [str(EE), *EE_FLAGS, str(directory / filename), "-o", str(output)],
                    check=True)
                data, sections, symbols, _relocations = views.elf(output)
        row = views.defined(symbols, symbol)
        self.assertEqual(row["size"], size, symbol)
        self.assertGreaterEqual(row["info"] >> 4, 1, symbol)
        actual = self._bytes(data, sections, row)
        self.assertEqual(len(actual), size, symbol)
        if content is not None:
            self.assertEqual(actual, content, symbol)
        return data, sections, symbols

    def _bytes(self, data, sections, symbol):
        section = sections[symbol["shndx"]]
        if section["type"] == 8:
            return bytes(symbol["size"])
        start = section["offset"] + symbol["value"]
        return data[start:start + symbol["size"]]

    def host(self, sources, driver):
        import tempfile
        with tempfile.TemporaryDirectory() as directory_name:
            directory = Path(directory_name)
            driver_path = directory / "driver.cpp"
            driver_path.write_text(driver)
            binary = directory / "driver"
            subprocess.run(
                ["c++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                 *[str(path) for path in sources], str(driver_path), "-o", str(binary)],
                check=True)
            subprocess.run([str(binary)], check=True)

    def ee_exports(self, source, defined_name, undefined):
        import tempfile
        with tempfile.TemporaryDirectory() as directory_name:
            output = Path(directory_name) / "unit.o"
            subprocess.run([str(EE), *EE_FLAGS, str(source), "-o", str(output)], check=True)
            nm = EE.with_name("mips64r5900el-ps2-elf-nm")
            text = subprocess.run([str(nm), str(output)], check=True,
                                  capture_output=True, text=True).stdout
        defined = sorted(line.split()[-1] for line in text.splitlines() if " T " in line or " D " in line or " B " in line or " R " in line)
        missing = sorted(line.split()[-1] for line in text.splitlines() if " U " in line)
        self.assertIn(defined_name, defined)
        self.assertEqual(missing, sorted(undefined))

    def test_network_mode_alias(self):
        data, sections, symbols = self.expect_object(
            "norrath_network_mode_byte_view.cpp", "norrath_network_mode_byte_view",
            1, b"\0", VIEWS)
        alias = views.defined(symbols, "netNetworkMode")
        original = views.defined(symbols, "norrath_network_mode_byte_view")
        self.assertEqual(alias["size"], 1)
        self.assertEqual(alias["value"], original["value"])
        self.assertEqual(alias["shndx"], original["shndx"])
        self.assertEqual(self._bytes(data, sections, alias), b"\0")

    def test_net_interface_is_up(self):
        self.expect_object("net_interface_is_up.cpp", "netInterfaceIsUp", 1, b"\0")

    def test_net_init_state(self):
        self.expect_object("net_init_state.cpp", "netInitState", 1, b"\0")

    def test_from_srs(self):
        self.expect_object("from_srs.cpp", "fromSRS", 1, b"\0")

    def test_from_srs_but_entering_game(self):
        self.expect_object("from_srs_but_entering_game.cpp", "fromSRSButEnteringGame", 1, b"\0")

    def test_core_been_in_srs(self):
        self.expect_object("core_been_in_srs.cpp", "coreBeenInSRS", 1, b"\0")

    def test_core_launch_srs(self):
        self.expect_object("core_launch_srs.cpp", "coreLaunchSRS", 1, b"\0")


if __name__ == "__main__":
    unittest.main()
