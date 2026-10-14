// Host check for setup_main__Fv. Not an EE plan input.
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#define printf norrath_host_printf
#include "setup_main.cpp"
#undef printf

extern "C" int sModuleId[16] = {0};
static std::string trace;
static std::vector<int> process_results;
static size_t process_at = 0;
static int patcher_result = 0;
static int init_result = 0;
static unsigned char* heap_object = nullptr;
static int saved_word = 0;

extern "C" int norrath_host_printf(const char* format, ...) {
    char text[128];
    va_list ap; va_start(ap, format);
    std::vsnprintf(text, sizeof text, format, ap);
    va_end(ap);
    trace += "[" + std::string(text) + "]";
    return 0;
}
extern "C" void* __builtin_new(unsigned size) {
    trace += "N" + std::to_string(size) + ";";
    static unsigned char storage[0x153f0 + 16];
    heap_object = storage;
    std::memset(storage, 0, sizeof storage);
    std::memcpy(storage + 86976, &saved_word, 4);
    return storage;
}
extern "C" void* __7VISetup(void* o) { trace += "C;"; return o; }
extern "C" void _$_7VISetup(void* o, int f) { trace += "D" + std::to_string(f) + ";"; (void)o; }
extern "C" int Init__7VISetupi(void* o, int v) {
    trace += "I" + std::to_string(v) + (o == heap_object ? "" : "?") + ";";
    for (int& m : sModuleId) m = 77;   // stands for modules loaded by Init
    return init_result;
}
extern "C" int Process__7VISetup(void*) { trace += "P;"; return process_results[process_at++]; }
extern "C" int RunPatcher__7VISetup(void*) { trace += "U;"; return patcher_result; }
extern "C" void RestartConnect__7VISetup(void*) { trace += "R;"; }
extern "C" void Free__7VISetup(void*) { trace += "F;"; }
extern "C" void initEzNetCnf() {
    trace += "i";
    for (int m : sModuleId) if (m != -1) trace += "!";
    trace += ";";
}
extern "C" void exitEzNetCnf() { trace += "e;"; }
extern "C" int ezNetCtlDownInterface(int v) { trace += "Z" + std::to_string(v) + ";"; return 0; }
extern "C" void unet_shutdown__Fv() { trace += "u;"; }
extern "C" void sceSifMExitRpc() { trace += "x;"; }
extern "C" int sceSifStopModule(int id, int n, const char* a, int* r) {
    trace += "S" + std::to_string(id) + "," + std::to_string(n) + (a ? "a" : "n") + (r ? "r" : "-") + ";"; return 0;
}
extern "C" int sceSifUnloadModule(int id) { trace += "L" + std::to_string(id) + ";"; return 0; }

static std::string run(std::vector<int> processing, int init, int patch, int saved, int* result) {
    trace.clear(); process_results = processing; process_at = 0; init_result = init; patcher_result = patch; saved_word = saved;
    *result = setup_main__Fv();
    return trace;
}

int main() {
    int r;
    // Negative init: returns -1 with no cleanup at all.
    std::string t = run({}, -2, 0, 0, &r);
    if (r != -1 || t != "N87024;C;i;I1;[ret = -2\n]") return 1;
    // Result 2: no module work even with a saved word.
    t = run({0, 2}, 0, 0, 1, &r);
    if (r != 2 || t != "N87024;C;i;I1;[ret = 0\n]P;P;F;Z0;e;[deleting setup\n]D3;") return 2;
    // Result 4 with zero saved word: returns 4, no shutdown calls.
    t = run({4}, 5, 0, 0, &r);
    if (r != 4 || t != "N87024;C;i;I1;[ret = 5\n]P;F;e;[deleting setup\n]D3;") return 3;
    // Result 3 with patcher result 4 restarts twice, then ends with 7: a saved word means
    // shutdown, then every module is stopped and unloaded and cleared.
    t = run({3, 3, 7}, 0, 4, 9, &r);
    std::string expect = "N87024;C;i;I1;[ret = 0\n]P;U;R;P;U;R;P;F;Z0;e;[deleting setup\n]D3;u;x;";
    for (int i = 0; i < 16; ++i) expect += "S77,0nr;L77;";
    if (r != 5 || t != expect) return 4;
    for (int i = 0; i < 16; ++i) if (sModuleId[i] != -1) return 5;
    // Patcher result other than 4 ends processing without restart.
    t = run({3}, 0, 1, 0, &r);
    if (r != 0 || t != "N87024;C;i;I1;[ret = 0\n]P;U;F;e;[deleting setup\n]D3;") return 6;
    return 0;
}
