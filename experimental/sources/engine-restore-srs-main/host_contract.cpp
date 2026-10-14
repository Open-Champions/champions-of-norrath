#define NORRATH_HOST_CONTRACT
#define printf norrath_host_printf
#define memset norrath_host_memset
#define sprintf norrath_host_sprintf
#define strcpy norrath_host_strcpy
#include "srs_main.cpp"
#undef printf
#undef memset
#undef sprintf
#undef strcpy
#include <cstring>

static int error;
static const char* log_names[48];
static int log_count;
static int network_result = 1;
static int init_result;
static int setup_result;
static int station_result;
static int language_result = 7;
static int saw_or;
static int exit_code = 99;
static int station_mode;
static const char* station_name;
static const char* station_135;
static const char* station_copied;
static int station_byte;
static int setup_calls;
static int dma_calls;
static int loadexec_calls;
static int early_frees;

static void fail(int code) {
    if (error == 0) error = code;
}
static void note(const char* name) {
    if (log_count < 48) log_names[log_count] = name;
    ++log_count;
}
static int saw(const char* name) {
    for (int i = 0; i < log_count; ++i) if (std::strcmp(log_names[i], name) == 0) return 1;
    return 0;
}

extern "C" void norrath_host_or32(unsigned address, unsigned mask) {
    if (address != 0x1000e020u || mask != 2u) fail(1);
    ++saw_or;
}
extern "C" void __10VILauncher(VILauncher*) { note("ctor"); }
extern "C" void __15VINetworkStatus(VINetworkStatus*) { note("net-ctor"); }
extern "C" void _$_15VINetworkStatus(VINetworkStatus*, int flags) {
    note("net-dtor");
    if (flags != 2) fail(2);
}
extern "C" void Initialize__15VINetworkStatus(VINetworkStatus*) { note("net-init"); }
extern "C" int CheckNetwork__15VINetworkStatus(VINetworkStatus*) {
    note("check");
    return network_result;
}
extern "C" int norrath_host_printf(const char* text, ...) {
    note(text);
    return 0;
}
extern "C" void* norrath_host_memset(void* destination, int value, unsigned count) {
    unsigned char* bytes = static_cast<unsigned char*>(destination);
    for (unsigned i = 0; i < count; ++i) bytes[i] = static_cast<unsigned char>(value);
    return destination;
}
extern "C" int norrath_host_sprintf(char* destination, const char* format, int value) {
    if (destination != s_buf || format != norrath_srs_decimal_format || value != language_result) fail(11);
    destination[0] = '7';
    destination[1] = 0;
    note("sprintf");
    return 1;
}
extern "C" char* norrath_host_strcpy(char* destination, const char* source) {
    char* out = destination;
    while ((*destination++ = *source++) != 0) {}
    return out;
}
extern "C" void sceGsResetPath() { note("gs"); }
extern "C" void sceDmaReset(int value) {
    note("dma");
    if (value != 1) fail(3);
    ++dma_calls;
}
extern "C" void sceSifInitRpc() { note("rpc"); }
extern "C" void sceSifInitIopHeap() { note("heap"); }
extern "C" int Init__10VILauncherb(VILauncher*, int network_up) {
    note("init");
    if (network_up != (network_result != 0)) fail(4);
    return init_result;
}
extern "C" void exit(int code) {
    note("exit");
    exit_code = code;
}
extern "C" void SetDefault__10VILanguage12VILanguageID(int value) {
    note("lang");
    if (value != 0) fail(5);
}
extern "C" void VIMemCardSetProductId__FPCc(const char* text) {
    if (std::strcmp(text, "BASLUS-20565EQONLINE") != 0) fail(6);
    note("id");
}
extern "C" void VIMemCardSetProductTitle__FPCc(const char* text) {
    if (std::strcmp(text, "EverQuest Online\nAdventures") != 0) fail(7);
    note("title");
}
extern "C" void VIDisplayLoadingScreen__Fb(int value) {
    if (value != 1) fail(8);
    note("screen");
}
extern "C" void Init__7VITimer() { note("timer"); }
extern "C" int Setup__10VILauncher(VILauncher*) {
    note("setup");
    ++setup_calls;
    return setup_result;
}
extern "C" void ClearVRAM__FUcUcUcUc(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
    if ((a | b | c | d) != 0) fail(9);
    note("vram");
}
extern "C" int GetDefault__10VILanguage() {
    note("get-lang");
    if (s_argv[0] != norrath_srs_language_word || s_argv[1] != s_buf) fail(10);
    return language_result;
}
extern "C" void Free__10VILauncher(VILauncher*) {
    note("free");
    ++early_frees;
}
extern "C" void LoadExecPS2(const char* path, int argc, char** argv) {
    if (std::strcmp(path, "cdrom0:\\NTGUI2.ELF;1") != 0 || argc != 2) fail(12);
    if (argv != s_argv || std::strcmp(s_buf, "7") != 0) fail(13);
    note("exec");
    ++loadexec_calls;
}
extern "C" void Close__7VITimer() { note("close"); }
extern "C" void ezNetCtlDownInterface(int value) {
    if (value != 0) fail(14);
    note("down");
}
extern "C" void exitEzNetCnf() { note("cnf"); }
extern "C" void exitEzNetCtl() { note("ctl"); }
extern "C" void _$_10VILauncher(VILauncher*, int flags) {
    if (flags != 2) fail(15);
    note("dtor");
}
extern "C" int Station__10VILauncheriPCcN22i(
    VILauncher*, int mode, const char* name, const char* text_135, const char* copied, int byte) {
    note("station");
    station_mode = mode;
    station_name = name;
    station_135 = text_135;
    station_copied = copied;
    station_byte = byte;
    g_mode = 9;
    g_myIndex = 4;
    g_numClients = 2;
    std::memcpy(g_ips, "one", 4);
    std::memcpy(g_ips + 24, "two", 4);
    g_ports[0] = 11;
    g_ports[1] = 22;
    std::memcpy(g_localClientIps, "aaa", 4);
    std::memcpy(g_localClientIps + 24, "bbb", 4);
    g_localClientPorts[0] = 33;
    g_localClientPorts[1] = 44;
    return station_result;
}

static char slot_a[2][32];
static char slot_c[2][32];
static char* first_slots[2];
static char* third_slots[2];
static int second_words[2];
static int fourth_words[2];
static int flag_word;
static int count_word;
static int zero_word;
static char name_text[] = "name";
static char text_135[] = "t135";
static char copied_text[] = "copied";

static void reset() {
    error = 0;
    log_count = 0;
    saw_or = 0;
    exit_code = 99;
    setup_calls = 0;
    dma_calls = 0;
    loadexec_calls = 0;
    early_frees = 0;
    station_mode = -1;
    norrath_srs_first_flag = 1;
    g_mode = 3;
    g_myIndex = 0;
    g_numClients = 0;
    flag_word = 1;
    count_word = 55;
    zero_word = 55;
    std::memset(slot_a, 0x11, sizeof slot_a);
    std::memset(slot_c, 0x11, sizeof slot_c);
    first_slots[0] = slot_a[0];
    first_slots[1] = slot_a[1];
    third_slots[0] = slot_c[0];
    third_slots[1] = slot_c[1];
    second_words[0] = second_words[1] = 0;
    fourth_words[0] = fourth_words[1] = 0;
}

static int run() {
    return srs_main__FPCcPiT1PPcT1T1T3T1bT0T0i(
        name_text, &flag_word, &count_word, first_slots, second_words, &zero_word,
        third_slots, fourth_words, 1, text_135, copied_text, 295);
}

int main() {
    reset();
    network_result = 1;
    init_result = 0;
    setup_result = 0;
    station_result = 0;
    int result = run();
    if (error != 0) return error;
    if (result != 100 || flag_word != 9 || count_word != 2 || zero_word != 4) return 20;
    if (std::strcmp(slot_a[0], "one") != 0 || std::strcmp(slot_a[1], "two") != 0) return 21;
    if (std::strcmp(slot_c[0], "aaa") != 0 || std::strcmp(slot_c[1], "bbb") != 0) return 22;
    if (second_words[0] != 11 || second_words[1] != 22) return 23;
    if (fourth_words[0] != 33 || fourth_words[1] != 44) return 24;
    if (dma_calls != 1 || saw_or != 1 || norrath_srs_first_flag != 0) return 25;
    if (setup_calls != 1 || loadexec_calls != 0) return 26;
    if (station_mode != 1 || station_name != name_text || station_byte != 295) return 27;
    if (station_135 != text_135 || station_copied != copied_text) return 28;
    if (!saw("Server mode\n") || saw("exec") || !saw("Whoop de doo!\n")) return 29;
    if (log_names[0] == nullptr || std::strcmp(log_names[0], "ctor") != 0) return 30;

    reset();
    norrath_srs_first_flag = 0;
    network_result = 0;
    station_result = 1;
    flag_word = 2;
    result = run();
    if (error != 0) return error;
    if (result != 0 || dma_calls != 0 || setup_calls != 0) return 31;
    if (!saw("Client mode\n") || saw("Server mode\n")) return 32;
    if (std::strcmp(slot_a[0], "one") != 0) return 33;

    reset();
    init_result = -1;
    station_result = 0;
    network_result = 1;
    flag_word = 9;
    result = run();
    if (exit_code != -1 || !saw("exit")) return 34;
    if (!saw("Unknown mode\n") || !saw("station")) return 35;
    if (result != 100) return 36;

    reset();
    network_result = 1;
    init_result = 0;
    setup_result = 2;
    station_result = 7;
    result = run();
    if (loadexec_calls != 1 || result != 100 || !saw("station")) return 37;

    reset();
    setup_result = 4;
    result = run();
    if (result != 0 || saw("station") || saw("exec") || !saw("down")) return 38;
    if (early_frees != 1 || flag_word != 1) return 39;

    reset();
    setup_result = 5;
    result = run();
    if (result != 0 || saw("station")) return 40;

    reset();
    setup_result = 6;
    station_result = 1;
    result = run();
    if (result != 0 || loadexec_calls != 0 || !saw("station")) return 41;
    if (saw("down")) return 42;
    return error;
}
