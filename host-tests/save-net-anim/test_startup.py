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

    def host(self, sources, driver, extra_flags=()):
        import tempfile
        with tempfile.TemporaryDirectory() as directory_name:
            directory = Path(directory_name)
            driver_path = directory / "driver.cpp"
            driver_path.write_text(driver)
            binary = directory / "driver"
            subprocess.run(
                ["c++", "-std=c++17", "-Wall", "-Wextra", "-Werror", *extra_flags,
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

    def test_mc_init(self):
        source = STARTUP / "mc_init.cpp"
        self.host([source], r'''
#include <string.h>
extern "C" const char* norrath_host_level_names[2] = {};
extern "C" int MC_Init__Fi(int);
extern "C" int disableMC = 0, flareAlpha = 0, curAct = 0, asyncMode = 0, oldAsyncMode = 0, configState = 0;
extern "C" int lastCardPort = 0, lastCardSlot = 0, browserState = 0, saveCheckPointCount = 0, flareDelta = 0;
extern "C" int MC_CheckPointAge = 0, slotFootPrint = 0, actCount = 0, bareSlotFootPrint = 0, totalClusters = 0;
extern "C" int errorCondition = 0, slotClusters = 0, totalFootPrint = 0, MCA_Port = 0, MCA_Slot = 0, MCA_LastDirCount = 0;
extern "C" int actLevelCount[5] = {};
extern "C" int _3map$autoMapBitsSize = 0;
extern "C" unsigned char frontEndWasRunning = 0, pauseStackPos = 0, keepPlayerPositions = 0;
extern "C" unsigned char MC_SavingOrLoading = 0, MC_LastModeSuccessful = 0, killPlayer = 0, MC_KickProtect = 0, MC_Debug = 0;
extern "C" unsigned char iconHeader[0xb29c8] = {};
extern "C" unsigned char sortedTimes[160] = {};
static unsigned char level_bytes[16];
extern "C" char* allActs[5] = {};
static int mc_init_result, new_game, config_value, tasks;
static const char* task_name[2];
static int task_argument[2];
static int sjis_called;
extern "C" void MCA_Init__Fv() {}
extern "C" void gameSetDefaultGameConfig__Fi(int value) { config_value = value; }
extern "C" int sceMcInit() { return mc_init_result; }
extern "C" void MC_NewGame__Fi(int value) { new_game = value; }
extern "C" void AsciiString2Sjis__FPUcPUs(unsigned char* ascii, unsigned short*) {
    if (strcmp(reinterpret_cast<char*>(ascii), "Champions Of Norrath") != 0) sjis_called = -1;
    else sjis_called = 1;
}
extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int, char* name, int argument) {
    (void)callback;
    task_name[tasks] = name;
    task_argument[tasks] = argument;
    tasks += 1;
}
extern "C" void MCA_Cmd__Fiiii(int, const void*, int, int) {}
extern "C" void MCA_Sync__FPi(int*) {}
extern "C" void mcHeartBeatRun__Fv() {}
extern "C" void mcHeartBeat__Fv() {}
static int fail_long(int code) {
    if (new_game != 1 || config_value != -1) return code;
    if (flareAlpha != 128 || frontEndWasRunning != 0xffu) return code;
    if (curAct != -1 || asyncMode != -1 || oldAsyncMode != -1) return code;
    if (MC_SavingOrLoading != 0 || MC_CheckPointAge != 0 || MC_KickProtect != 0) return code;
    if (strcmp(reinterpret_cast<char*>(sortedTimes + 64), "BASLUS-20565") != 0) return code;
    if (strcmp(reinterpret_cast<char*>(sortedTimes + 112), "BASLUS-20565_config") != 0) return code;
    if (totalClusters != 849 || slotClusters != 715 || totalFootPrint != 0xb6f5c) return code;
    if (slotFootPrint != 0xb29c8) return code;
    if (iconHeader[0] != 'P' || iconHeader[1] != 'S' || iconHeader[2] != '2' || iconHeader[3] != 'D') return code;
    if (iconHeader[6] != 26) return code;
    if (memcmp(iconHeader + 260, "icon.ico", 9) != 0) return code;
    if (memcmp(iconHeader + 324, "icon.ico", 9) != 0) return code;
    if (memcmp(iconHeader + 388, "icon.ico", 9) != 0) return code;
    unsigned bits = iconHeader[84] | (iconHeader[85] << 8) | (iconHeader[86] << 16) | (iconHeader[87] << 24);
    if (bits != 0x3f000000u) return code;
    if (sjis_called != 1 || tasks != 2) return code;
    if (strcmp(task_name[0], "Async Load/Save Daemon RUN") != 0 || task_argument[0] != 0) return code;
    if (strcmp(task_name[1], "Async Load/Save Daemon DRAW") != 0 || task_argument[1] != 1) return code;
    if (MCA_Slot != -1 || MCA_Port != -1) return code;
    return 0;
}
int main() {
    mc_init_result = 4;
    actCount = 0;
    if (MC_Init__Fi(0) != 1 || disableMC != 1) return 2;
    mc_init_result = 0;
    new_game = 0;
    tasks = 0;
    actCount = 1;
    actLevelCount[0] = 2;
    norrath_host_level_names[0] = "small";
    norrath_host_level_names[1] = "large";
    level_bytes[0] = 0;
    level_bytes[4] = 10;
    level_bytes[8] = 1;
    level_bytes[12] = 200;
    allActs[0] = reinterpret_cast<char*>(level_bytes);
    _3map$autoMapBitsSize = 18;
    MCA_LastDirCount = 3;
    int result = MC_Init__Fi(0);
    int bad = fail_long(3);
    if (bad != 0) return bad;
    if (result != -1 || disableMC != 0) return 4;
    int first = level_bytes[4] | (level_bytes[5] << 8) | (level_bytes[6] << 16) | (level_bytes[7] << 24);
    int second = level_bytes[12] | (level_bytes[13] << 8) | (level_bytes[14] << 16) | (level_bytes[15] << 24);
    if (first != 1024 || second != 1152) return 5;
    if (bareSlotFootPrint != 1024 + 1152 + 648) return 6;
    if (MC_Debug != 1) return 7;
    tasks = 0;
    new_game = 0;
    MCA_LastDirCount = 0;
    actCount = 0;
    if (MC_Init__Fi(9) != -1 || disableMC != 9) return 8;
    if (bareSlotFootPrint != 648 || MC_Debug != 0) return 9;
    return 0;
}
''', ("-DNORRATH_HOST_ARENA",))
        self.ee_exports(source, "MC_Init__Fi", [
            "AsciiString2Sjis__FPUcPUs", "MCA_Cmd__Fiiii", "MCA_Init__Fv",
            "MCA_LastDirCount", "MCA_Port", "MCA_Slot", "MCA_Sync__FPi",
            "MC_CheckPointAge", "MC_Debug", "MC_KickProtect", "MC_LastModeSuccessful",
            "MC_NewGame__Fi", "MC_SavingOrLoading", "_3map$autoMapBitsSize", "actCount",
            "actLevelCount", "allActs", "asyncMode", "bareSlotFootPrint", "browserState",
            "configState", "curAct", "disableMC", "engineAddTask__FPFv_viPci", "errorCondition",
            "flareAlpha", "flareDelta", "frontEndWasRunning", "gameSetDefaultGameConfig__Fi",
            "iconHeader", "keepPlayerPositions", "killPlayer", "lastCardPort", "lastCardSlot",
            "mcHeartBeatRun__Fv", "mcHeartBeat__Fv", "memset", "oldAsyncMode", "pauseStackPos",
            "saveCheckPointCount", "sceMcInit", "slotClusters", "slotFootPrint", "sortedTimes",
            "sprintf", "strcpy", "totalClusters", "totalFootPrint"])

    def test_sce_mc_init(self):
        source = STARTUP / "sce_mc_init.cpp"
        self.host([source], r'''
#include <string.h>
extern "C" int semaidRegFunc = 0;
extern "C" int semaid_timerfunc = 0;
extern "C" unsigned char mcClientID[40] = {};
extern "C" unsigned char sifParamOrd[48] = {};
extern "C" unsigned char retval[64] = {};
static int sema_calls = 0;
static int sema_a4[2], sema_a8[2], sema_a20[2];
static int sync_args[3];
static int waited = -1;
static int wait_id = -1;
static int init_rpc = -1;
static int binds = 0;
static int leave_unbound = 0;
static unsigned bind_server = 0;
static int bind_mode = -1;
static int rpc_command = 0;
static int rpc_mode = -1;
static int rpc_send_size = 0;
static int rpc_receive_size = 0;
static void* rpc_send = 0;
static void* rpc_receive = 0;
static void* rpc_done = 0;
static void* rpc_done_arg = 0;
static int rpc_result = 0;
static int signals = 0;
static int printed = 0;
static char printed_text[80];
extern "C" int CreateSema(void* param) {
    unsigned char* bytes = static_cast<unsigned char*>(param);
    int slot = sema_calls;
    if (slot < 2) {
        sema_a4[slot] = bytes[4] | (bytes[5] << 8) | (bytes[6] << 16) | (bytes[7] << 24);
        sema_a8[slot] = bytes[8] | (bytes[9] << 8) | (bytes[10] << 16) | (bytes[11] << 24);
        sema_a20[slot] = bytes[20] | (bytes[21] << 8) | (bytes[22] << 16) | (bytes[23] << 24);
    }
    sema_calls += 1;
    return sema_calls == 1 ? 7 : 8;
}
extern "C" int sceMcSync(int mode, int a, int b) {
    sync_args[0] = mode; sync_args[1] = a; sync_args[2] = b; return 0;
}
extern "C" int WaitSema(int id) { wait_id = id; return 0; }
extern "C" int sceSifInitRpc(int mode) { init_rpc = mode; return 0; }
extern "C" int sceSifBindRpc(void* client, unsigned int server, int mode) {
    binds += 1;
    bind_server = server;
    bind_mode = mode;
    if (client != mcClientID) return -2;
    if (leave_unbound > 0) {
        leave_unbound -= 1;
        mcClientID[36] = 0; mcClientID[37] = 0; mcClientID[38] = 0; mcClientID[39] = 0;
    } else {
        mcClientID[36] = 1; mcClientID[37] = 0; mcClientID[38] = 0; mcClientID[39] = 0;
    }
    return 0;
}
extern "C" int scePrintf(const char* text) {
    printed += 1;
    strncpy(printed_text, text, 79);
    printed_text[79] = 0;
    return 0;
}
static void put_word(unsigned char* base, int offset, int value) {
    unsigned bits = static_cast<unsigned>(value);
    base[offset] = static_cast<unsigned char>(bits);
    base[offset + 1] = static_cast<unsigned char>(bits >> 8);
    base[offset + 2] = static_cast<unsigned char>(bits >> 16);
    base[offset + 3] = static_cast<unsigned char>(bits >> 24);
}
static int word_at(unsigned char* base, int offset) {
    return (int)((unsigned)base[offset] | ((unsigned)base[offset + 1] << 8) |
                 ((unsigned)base[offset + 2] << 16) | ((unsigned)base[offset + 3] << 24));
}
extern "C" int sceSifCallRpc(void* client, int command, int mode, void* send,
                             int send_size, void* receive, int receive_size,
                             void* done, void* done_arg) {
    if (client != mcClientID) return -50;
    rpc_command = command;
    rpc_mode = mode;
    rpc_send = send;
    rpc_send_size = send_size;
    rpc_receive = receive;
    rpc_receive_size = receive_size;
    rpc_done = done;
    rpc_done_arg = done_arg;
    return rpc_result;
}
extern "C" int SignalSema(int id) { signals += 1; waited = id; return 0; }
extern "C" int sceMcInit();
int main() {
    semaidRegFunc = -1;
    leave_unbound = 1;
    rpc_result = 0;
    put_word(retval, 0, 9);
    put_word(retval, 4, 522);
    put_word(retval, 8, 526);
    if (sceMcInit() != 9) return 1;
    if (sema_calls != 2 || sema_a4[0] != 1 || sema_a8[0] != 1 || sema_a20[0] != 0) return 2;
    if (sema_a4[1] != 1 || sema_a8[1] != 0 || sema_a20[1] != 0) return 3;
    if (semaidRegFunc != 7 || semaid_timerfunc != 8) return 4;
    if (sync_args[0] != 0 || sync_args[1] != 0 || sync_args[2] != 0) return 5;
    if (init_rpc != 0 || binds != 2 || bind_server != 0x80000400u || bind_mode != 0) return 6;
    if (rpc_command != 254 || rpc_mode != 0 || rpc_send != sifParamOrd || rpc_send_size != 48) return 7;
    if (rpc_receive != retval || rpc_receive_size != 12 || rpc_done != 0 || rpc_done_arg != 0) return 8;
    if (signals != 1 || waited != 7 || wait_id != 7 || printed != 0) return 9;
    put_word(retval, 4, 521);
    if (sceMcInit() != -120 || printed != 1 || strcmp(printed_text, "libmc: too old release of mcserv.irx\n") != 0) return 10;
    if (word_at(mcClientID, 36) != 0) return 11;
    put_word(retval, 4, 522);
    put_word(retval, 8, 525);
    if (sceMcInit() != -121 || strcmp(printed_text, "libmc: too old release of mcman.irx\n") != 0) return 12;
    rpc_result = -3;
    put_word(retval, 8, 526);
    if (sceMcInit() != -103 || word_at(mcClientID, 36) != 0) return 13;
    int created = sema_calls;
    semaidRegFunc = 5;
    rpc_result = 0;
    put_word(retval, 0, 4);
    if (sceMcInit() != 4 || sema_calls != created || semaidRegFunc != 5) return 14;
    return 0;
}
''')
        self.ee_exports(source, "sceMcInit", [
            "CreateSema", "SignalSema", "WaitSema", "mcClientID", "retval",
            "sceMcSync", "scePrintf", "sceSifBindRpc", "sceSifCallRpc",
            "sceSifInitRpc", "semaidRegFunc", "semaid_timerfunc", "sifParamOrd"])

    def test_net_free(self):
        source = STARTUP / "net_free.cpp"
        self.host([source], r'''
extern "C" unsigned char netInitState = 0;
extern "C" int broadcastConnection = 0;
extern "C" unsigned char cd[4] = {};
extern "C" unsigned char g_net_buf[4] = {};
extern "C" unsigned char netInterfaceIsUp = 0;
extern "C" unsigned char netNetworkMode = 0;
extern "C" unsigned char netNmPartners = 0;
extern "C" unsigned char partners[70528] = {};
extern "C" int nmPartners = 0;
static char log[8];
static int logn = 0;
static int close_connection = 0;
static int close_timeout = 0;
static int down_mode = -1;
static int saw_nm_before_blank = -1;
static void note(char mark) { log[logn++] = mark; }
extern "C" int sceInetClose(unsigned char* client, unsigned char* buffer,
                            int connection, int timeout) {
    if (client != cd || buffer != g_net_buf) return 1;
    close_connection = connection;
    close_timeout = timeout;
    note('C');
    return 0;
}
extern "C" int sceInetCtlDownInterface(unsigned char* client, unsigned char* buffer, int mode) {
    if (client != cd || buffer != g_net_buf) return 1;
    down_mode = mode;
    note('D');
    return 0;
}
extern "C" int sceLibnetTerminate(unsigned char* client) {
    if (client != cd) return 1;
    note('T');
    return 0;
}
extern "C" void* memset(void* destination, int value, unsigned size) {
    unsigned char* bytes = static_cast<unsigned char*>(destination);
    if (destination == partners && size == 0x11380u) {
        saw_nm_before_blank = netNmPartners;
        note('M');
    }
    for (unsigned index = 0; index < size; ++index) bytes[index] = static_cast<unsigned char>(value);
    return destination;
}
extern "C" void netFree__Fb(int bring_down);
static int float_at(int offset) {
    return partners[offset] == 0 && partners[offset + 1] == 0 &&
           partners[offset + 2] == 0 && partners[offset + 3] == 0x43;
}
static int pattern_ok() {
    static const int offsets[] = {24, 40, 56, 72, 104, 120, 136, 152};
    for (int slot = 0; slot < 4; ++slot) {
        int base = slot * 17632;
        if (partners[base] != 0 || partners[base + 16] != 0 || partners[base + 92] != 0) return 0;
        if (partners[base + 17631] != 0) return 0;
        for (int item = 0; item < 8; ++item) {
            if (!float_at(base + offsets[item])) return 0;
        }
    }
    return 1;
}
int main() {
    for (unsigned index = 0; index < sizeof partners; ++index) partners[index] = 0xab;
    netInitState = 0;
    broadcastConnection = 42;
    netInterfaceIsUp = 7;
    netNetworkMode = 3;
    netNmPartners = 4;
    nmPartners = 9;
    netFree__Fb(1);
    if (logn != 0 || partners[0] != 0xab || netInitState != 0 || broadcastConnection != 42) return 1;
    if (netInterfaceIsUp != 7 || netNetworkMode != 3 || nmPartners != 9 || netNmPartners != 4) return 2;
    logn = 0;
    netInitState = 1;
    broadcastConnection = -5;
    netFree__Fb(0);
    if (logn != 2 || log[0] != 'T' || log[1] != 'M') return 3;
    if (broadcastConnection != -5 || netInterfaceIsUp != 7 || netInitState != 0) return 4;
    if (netNetworkMode != 0 || nmPartners != 0 || netNmPartners != 1 || saw_nm_before_blank != 1) return 5;
    if (!pattern_ok()) return 6;
    logn = 0;
    for (unsigned index = 0; index < sizeof partners; ++index) partners[index] = 0xcd;
    netInitState = 1;
    broadcastConnection = 42;
    netInterfaceIsUp = 7;
    netNetworkMode = 3;
    nmPartners = 9;
    netFree__Fb(1);
    if (logn != 4 || log[0] != 'C' || log[1] != 'D' || log[2] != 'T' || log[3] != 'M') return 7;
    if (close_connection != 42 || close_timeout != 1000 || down_mode != 0) return 8;
    if (broadcastConnection != -1 || netInterfaceIsUp != 0 || !pattern_ok()) return 9;
    return 0;
}
''')
        self.ee_exports(source, "netFree__Fb", [
            "broadcastConnection", "cd", "g_net_buf", "memset", "netInitState",
            "netInterfaceIsUp", "netNetworkMode", "netNmPartners", "nmPartners",
            "partners", "sceInetClose", "sceInetCtlDownInterface", "sceLibnetTerminate"])


if __name__ == "__main__":
    unittest.main()
