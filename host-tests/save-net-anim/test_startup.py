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

    def test_mc_protect_ram_slot(self):
        self.expect_object("mc_protect_ram_slot.cpp", "MC_ProtectRamSlot", 1, b"\0")

    def test_net_game_launch_info(self):
        self.expect_object("net_game_launch_info.cpp", "netGameLaunchInfo", 328, bytes(328))

    def test_cut_init_net(self):
        source = STARTUP / "cut_init_net.cpp"
        self.host([source], r'''
extern "C" int cutMsgHandler = 0;
extern "C" void cutMsgHandlerFunction__FPUci(unsigned char*, int) {}
extern "C" void cutInitNet__Fv();
static int calls;
extern "C" int netRegisterMessageHandler__FPFPUci_v(void (*handler)(unsigned char*, int)) {
    if (handler != cutMsgHandlerFunction__FPUci) return 1;
    calls += 1;
    return 41;
}
int main() {
    cutMsgHandler = 0;
    cutInitNet__Fv();
    if (calls != 1 || cutMsgHandler != 41) return 2;
    return 0;
}
''')
        self.ee_exports(source, "cutInitNet__Fv", [
            "cutMsgHandler", "cutMsgHandlerFunction__FPUci",
            "netRegisterMessageHandler__FPFPUci_v"])

    def test_comm_init_net(self):
        source = STARTUP / "comm_init_net.cpp"
        self.host([source], r'''
extern "C" int commHandle = 0;
extern "C" int commStatusHandle = 0;
extern "C" void commHandler__FPUci(unsigned char*, int) {}
extern "C" void commStatusHandler__FPUci(unsigned char*, int) {}
extern "C" void commInitNet__Fv();
extern "C" int netRegisterPacketHandler__FPFPUci_v(void (*handler)(unsigned char*, int)) {
    if (handler != commHandler__FPUci || commHandle != 0) return 9;
    return 7;
}
extern "C" int netRegisterMessageHandler__FPFPUci_v(void (*handler)(unsigned char*, int)) {
    if (handler != commStatusHandler__FPUci || commHandle != 7) return 9;
    return 8;
}
int main() {
    commHandle = commStatusHandle = 0;
    commInitNet__Fv();
    if (commHandle != 7 || commStatusHandle != 8) return 2;
    return 0;
}
''')
        self.ee_exports(source, "commInitNet__Fv", [
            "commHandle", "commHandler__FPUci", "commStatusHandle",
            "commStatusHandler__FPUci", "netRegisterMessageHandler__FPFPUci_v",
            "netRegisterPacketHandler__FPFPUci_v"])

    def test_dlg_init_net(self):
        source = STARTUP / "dlg_init_net.cpp"
        self.host([source], r'''
extern "C" int dialogMsgHandle = 0;
extern "C" void dialogMsgCallback__FPUci(unsigned char*, int) {}
extern "C" void DLG_InitNet__Fv();
extern "C" int netRegisterMessageHandler__FPFPUci_v(void (*handler)(unsigned char*, int)) {
    if (handler != dialogMsgCallback__FPUci) return 1;
    return 15;
}
int main() {
    dialogMsgHandle = 0;
    DLG_InitNet__Fv();
    if (dialogMsgHandle != 15) return 2;
    return 0;
}
''')
        self.ee_exports(source, "DLG_InitNet__Fv", [
            "dialogMsgCallback__FPUci", "dialogMsgHandle",
            "netRegisterMessageHandler__FPFPUci_v"])

    def test_hud_init_net(self):
        source = STARTUP / "hud_init_net.cpp"
        self.host([source], r'''
extern "C" int hudMsgHandle = 0;
extern "C" int netWaitMsgHandler = 0;
extern "C" void hudMsgCallback__FPUci(unsigned char*, int) {}
extern "C" void netWaitCallBack__FPUci(unsigned char*, int) {}
extern "C" void hudInitNet__Fv();
extern "C" int netRegisterMessageHandler__FPFPUci_v(void (*handler)(unsigned char*, int)) {
    if (handler == hudMsgCallback__FPUci) return 11;
    if (handler == netWaitCallBack__FPUci) {
        if (hudMsgHandle != 11) return 3;
        return 12;
    }
    return 4;
}
int main() {
    hudMsgHandle = netWaitMsgHandler = 0;
    hudInitNet__Fv();
    if (hudMsgHandle != 11 || netWaitMsgHandler != 12) return 2;
    return 0;
}
''')
        self.ee_exports(source, "hudInitNet__Fv", [
            "hudMsgCallback__FPUci", "hudMsgHandle",
            "netRegisterMessageHandler__FPFPUci_v", "netWaitCallBack__FPUci",
            "netWaitMsgHandler"])

    def test_text_init_net(self):
        source = STARTUP / "text_init_net.cpp"
        self.host([source], r'''
extern "C" int dataHandle = 0;
extern "C" void textHandler__FPUci(unsigned char*, int) {}
extern "C" void textInitNet__Fv();
extern "C" int netRegisterMessageHandler__FPFPUci_v(void (*handler)(unsigned char*, int)) {
    if (handler != textHandler__FPUci) return 1;
    return 21;
}
int main() {
    dataHandle = 0;
    textInitNet__Fv();
    if (dataHandle != 21) return 2;
    return 0;
}
''')
        self.ee_exports(source, "textInitNet__Fv", [
            "dataHandle", "netRegisterMessageHandler__FPFPUci_v", "textHandler__FPUci"])

    def test_mc_configure(self):
        source = STARTUP / "mc_configure.cpp"
        self.host([source], r'''
extern "C" int MC_Configure__Fv();
static int stopped;
extern "C" void stopCardQuery__Fv() { stopped = 1; }
extern "C" int newMode__Fii(int mode, int argument) {
    if (!stopped || mode != 0 || argument != 0) return 9;
    return 5;
}
int main() {
    if (MC_Configure__Fv() != 5) return 2;
    return 0;
}
''')
        self.ee_exports(source, "MC_Configure__Fv", ["newMode__Fii", "stopCardQuery__Fv"])

    def test_mc_reset(self):
        source = STARTUP / "mc_reset.cpp"
        self.host([source], r'''
extern "C" int MCA_Slot = 0;
extern "C" int MCA_Port = 0;
extern "C" int MC_Reset__Fv();
static int cleared;
extern "C" void MCA_ClearInfo__Fv() { cleared = 1; }
int main() {
    MCA_Slot = 4;
    MCA_Port = 6;
    if (MC_Reset__Fv() != -1) return 2;
    if (!cleared || MCA_Slot != -1 || MCA_Port != -1) return 3;
    return 0;
}
''')
        self.ee_exports(source, "MC_Reset__Fv", ["MCA_ClearInfo__Fv", "MCA_Port", "MCA_Slot"])

    def test_mc_process(self):
        source = STARTUP / "mc_process.cpp"
        self.host([source], r'''
extern "C" int gameBailFromGameLoop = 0;
extern "C" int mcProcess__Fv();
static int beats;
extern "C" void mcHeartBeat__Fv() { beats += 1; }
int main() {
    gameBailFromGameLoop = 0;
    if (mcProcess__Fv() != 0 || beats != 1) return 2;
    gameBailFromGameLoop = -1;
    if (mcProcess__Fv() != 1 || beats != 2) return 3;
    gameBailFromGameLoop = 2;
    if (mcProcess__Fv() != 1) return 4;
    return 0;
}
''')
        self.ee_exports(source, "mcProcess__Fv", ["gameBailFromGameLoop", "mcHeartBeat__Fv"])


if __name__ == "__main__":
    unittest.main()
