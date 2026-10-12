#define printf norrath_host_printf
#include "launcher_init.cpp"
#undef printf

#include <cstring>

static const char* log_name[16];
static int log_value[16];
static int log_count;
static unsigned char* clock_seen;
static int thread_id = 42;
static int seen_priority;
static int seen_thread;
static int mc_args[5];
static int sync_mode;
static void* sync_first;
static void* sync_second;
static int pad_mode;

static void note(const char* name, int value) {
    if (log_count < 16) {
        log_name[log_count] = name;
        log_value[log_count] = value;
    }
    ++log_count;
}

extern "C" int norrath_host_printf(const char* text, ...) {
    note(text, 0);
    return 0;
}
extern "C" int sceCdReadClock(unsigned char* record) {
    clock_seen = record;
    record[1] = 0x3c;
    note("clock", 0);
    return 0;
}
extern "C" void srand(unsigned seed) {
    note("srand", static_cast<int>(seed));
}
extern "C" void VIDisplayLoadingScreen__Fb(int argument) {
    note("loading", argument);
}
extern "C" void initEzNetCtl() { note("eznet", 0); }
extern "C" int GetThreadId() {
    note("thread", 0);
    return thread_id;
}
extern "C" void ChangeThreadPriority(int thread, int priority) {
    seen_thread = thread;
    seen_priority = priority;
    note("priority", priority);
}
extern "C" int sceMcGetInfo(int first, int second, int third, int fourth, int fifth) {
    mc_args[0] = first;
    mc_args[1] = second;
    mc_args[2] = third;
    mc_args[3] = fourth;
    mc_args[4] = fifth;
    note("mcinfo", 0);
    return 0;
}
extern "C" int sceMcSync(int mode, void* first, void* second) {
    sync_mode = mode;
    sync_first = first;
    sync_second = second;
    note("mcsync", mode);
    return 0;
}
extern "C" void scePadInit(int mode) {
    pad_mode = mode;
    note("pad", mode);
}

static int same(int index, const char* name, int value) {
    if (std::strcmp(log_name[index], name) != 0 || log_value[index] != value) return 1;
    return 0;
}

int main() {
    unsigned char object[272];
    std::memset(object, 0xa5, sizeof object);
    for (int index = 0; index < 16; ++index) sModuleId[index] = 7;
    gPatchPresent = 4;
    gPowerOffTid = 5;
    gPowerOffSid = 6;
    int first = Init__10VILauncherb(reinterpret_cast<VILauncher*>(object), 0);
    int second = Init__10VILauncherb(reinterpret_cast<VILauncher*>(object), 1);
    if (first != 0 || second != 0) return 1;
    if (log_count != 26) return 2;
    if (same(0, "clock", 0) || same(1, "srand", 0x3c) || same(2, "loading", 0)) return 3;
    if (same(3, "za\n", 0) || same(4, "zb\n", 0) || same(5, "eznet", 0) || same(6, "zc\n", 0)) return 4;
    if (same(7, "thread", 0) || same(8, "priority", 2) || same(9, "mcinfo", 0)) return 5;
    if (same(10, "mcsync", 0) || same(11, "pad", 0) || same(12, "loading", 1)) return 6;
    if (seen_thread != 42 || seen_priority != 2 || pad_mode != 0 || sync_mode != 0) return 7;
    if (sync_first == nullptr || sync_second == nullptr || sync_first == sync_second) return 8;
    for (int arg : mc_args) {
        if (arg != 0) return 9;
    }
    if (*reinterpret_cast<unsigned*>(object) != 0 || *reinterpret_cast<unsigned*>(object + 4) != 0) return 10;
    if (object[8] != 0 || object[264] != 0 || object[9] != 0xa5 || object[263] != 0xa5 || object[265] != 0xa5)
        return 11;
    for (int id : sModuleId) {
        if (id != -1) return 12;
    }
    if (gPatchPresent != 0 || gPowerOffTid != -1 || gPowerOffSid != -1) return 13;
    if (clock_seen == nullptr) return 14;
    return 0;
}
