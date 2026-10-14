#define printf norrath_host_printf
#include "launcher_free.cpp"
#undef printf

#include <cstdarg>
#include <cstdio>
#include <cstring>

extern "C" {
int gPowerOffTid = -1;
int gPowerOffSid = -1;
int gPowerOffTerminate;
int sModuleId[16];
}

static char log_text[24][64];
static int log_count;
static int signal_id;
static int rotates;
static int refer_calls;
static int refer_thread;
static void* refer_status;
static int deleted_thread;
static int deleted_sema;
static int stop_id;
static int stop_length;
static const char* stop_argument;
static int* stop_result;
static int stop_return;
static int unload_id;
static int unload_return;

static void add_log(const char* text) {
    if (log_count < 24) std::snprintf(log_text[log_count], sizeof log_text[log_count], "%s", text);
    ++log_count;
}

extern "C" int norrath_host_printf(const char* text, ...) {
    char buffer[64];
    va_list args;
    va_start(args, text);
    std::vsnprintf(buffer, sizeof buffer, text, args);
    va_end(args);
    add_log(buffer);
    return 0;
}
extern "C" void SignalSema(int identifier) { signal_id = identifier; }
extern "C" void RotateThreadReadyQueue(int priority) {
    if (priority != 1) rotates = -1;
    ++rotates;
}
extern "C" int ReferThreadStatus(int thread, void* status) {
    refer_thread = thread;
    refer_status = status;
    ++refer_calls;
    if (refer_calls == 1) return 1;
    return 16;
}
extern "C" void DeleteThread(int thread) { deleted_thread = thread; }
extern "C" void DeleteSema(int identifier) { deleted_sema = identifier; }
extern "C" void scePadEnd() { add_log("pad"); }
extern "C" void sceMcEnd() { add_log("mc"); }
extern "C" int exitEzNetCnf() {
    add_log("cnf");
    return 7;
}
extern "C" int exitEzNetCtl() {
    add_log("ctl");
    return 8;
}
extern "C" int sceSifStopModule(int identifier, int argument_length, const char* argument, int* result) {
    stop_id = identifier;
    stop_length = argument_length;
    stop_argument = argument;
    stop_result = result;
    return stop_return;
}
extern "C" int sceSifUnloadModule(int identifier) {
    unload_id = identifier;
    return unload_return;
}

int main() {
    unsigned char object[8];
    std::memset(object, 0xa5, sizeof object);
    for (int& id : sModuleId) id = -1;
    sModuleId[15] = 5;
    sModuleId[0] = 7;
    gPowerOffTid = -1;
    gPowerOffSid = 3;
    Free__10VILauncher(reinterpret_cast<VILauncher*>(object));
    if (signal_id != 0 || refer_calls != 0 || deleted_thread != 0) return 1;
    if (object[0] != 0xa5) return 2;
    if (std::strcmp(log_text[0], "pad") != 0 || std::strcmp(log_text[1], "mc") != 0) return 3;
    if (std::strcmp(log_text[2], "UNLOADING MODULES\n") != 0) return 4;
    if (std::strcmp(log_text[3], "cnf") != 0 || std::strcmp(log_text[4], "ctl") != 0) return 5;
    if (std::strcmp(log_text[5], "unload 5\n") != 0 || std::strcmp(log_text[6], "unload 7\n") != 0) return 6;
    if (sModuleId[15] != -1 || sModuleId[0] != -1) return 7;
    if (stop_id != 7 || unload_id != 7 || stop_length != 0 || stop_argument != nullptr) return 8;
    if (stop_result == nullptr) return 9;

    log_count = 0;
    refer_calls = 0;
    gPowerOffTid = 3;
    gPowerOffSid = 8;
    gPowerOffTerminate = 0;
    sModuleId[4] = 9;
    stop_return = -5;
    unload_return = -6;
    Free__10VILauncher(reinterpret_cast<VILauncher*>(object));
    if (gPowerOffTerminate != 1 || signal_id != 8) return 10;
    if (refer_calls != 2 || refer_thread != 3 || refer_status != nullptr || rotates != 1) return 11;
    if (deleted_thread != 3 || deleted_sema != 8) return 12;
    if (gPowerOffTid != -1 || gPowerOffSid != -1) return 13;
    if (std::strcmp(log_text[5], "unload 9\n") != 0) return 14;
    if (std::strcmp(log_text[6], "stop failed -5\n") != 0) return 15;
    if (std::strcmp(log_text[7], "unload failed -6\n") != 0) return 16;
    if (sModuleId[4] != -1) return 17;
    return 0;
}
