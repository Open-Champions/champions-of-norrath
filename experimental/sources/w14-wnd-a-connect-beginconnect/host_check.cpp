// Host check only; not a build input. Build: c++ -std=c++17 -arch x86_64 -Wl,-pagezero_size,0x1000 host_check.cpp
// (statics must sit below 4 GiB because the record pointer is a 32-bit word).
#define printf host_printf_unit
#define strncmp host_strncmp_unit
#include "beginconnect.cpp"
#undef printf
#undef strncmp
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

alignas(8) static int window[600];
alignas(8) static unsigned char records[620 * 2];
static std::string trace;
static int sif_failures;
extern "C" {
int host_printf_unit(const char* f, ...) {
    trace += "P[";
    trace += f;
    trace += "]";
    return 0;
}
int host_strncmp_unit(const char* a, const char* b, unsigned n) { return std::strncmp(a, b, n); }
int snprintf__8VIStringPciPCce(char* buffer, int size, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    const char* arg = va_arg(ap, const char*);
    va_end(ap);
    trace += "S";
    return std::snprintf(buffer, size, format, arg);
}
int sceGsSyncV(int) { trace += "v"; return 0; }
int sceSifLoadModule(const char* path, int, const char*) {
    trace += std::string("M<") + path + ">";
    return sif_failures-- > 0 ? -5 : 7;
}
int VIEELoadIrx__FPCciT0(const char* name, int, int) { trace += std::string("L<") + name + ">"; return 11; }
void VIEEUnloadIrx__Fi(int handle) { trace += "U" + std::to_string(handle); }
int BringUpNetwork__12VIWndConnect(void*) { trace += "B"; return 42; }
}
static void prepare(int kind, const char* text) {
    std::memset(window, 0, sizeof window);
    std::memset(records, 0, sizeof records);
    window[479] = 1;
    window[493] = static_cast<int>(reinterpret_cast<unsigned long>(records));
    *reinterpret_cast<int*>(records + 620 + 516 + 4) = kind;
    std::strcpy(reinterpret_cast<char*>(records + 620 + 516 + 72), text);
    for (int k : {482, 484, 485, 486, 487}) window[k] = -1;
    trace.clear();
}

int main() {
    bool ok = true;
    if (reinterpret_cast<unsigned long>(records) > 0xffffffffUL) { std::puts("FAIL (statics above 4 GiB)"); return 1; }
    // Adapter kind 3 with the SCE text: the SMAP module is loaded after two failures.
    prepare(3, "SCE");
    sif_failures = 2;
    ok = ok && BeginConnect__12VIWndConnect(window) == 42 && window[482] == 7 &&
         trace == "vvvvSP[%s\n]M<cdrom0:\\MODULES\\SMAP.IRX;1>P[sceSifLoadModule: cannot load '%s'\n]"
                  "M<cdrom0:\\MODULES\\SMAP.IRX;1>P[sceSifLoadModule: cannot load '%s'\n]M<cdrom0:\\MODULES\\SMAP.IRX;1>B";
    // Kind 2 (dial-up) loads PPP and PPPoE and, for a non-SCE text, the AN986 module.
    prepare(2, "other");
    ok = ok && BeginConnect__12VIWndConnect(window) == 42 && window[485] == 11 && window[486] == 11 && window[484] == 11 &&
         trace == "vvvvL<PPP.IRX>L<PPPOE.IRX>L<AN986.IRX>B";
    // Existing handles are unloaded first; an unknown kind returns -1 after the syncs.
    prepare(9, "SCE");
    window[482] = 3; window[487] = 4; window[486] = 5;
    ok = ok && BeginConnect__12VIWndConnect(window) == -1 && window[482] == -1 && window[487] == -1 && window[486] == -1 &&
         trace == "P[unloading smap\n]U3P[unloading spduart\n]U4P[unloading pppoe\n]U5vvvv";
    // A failure past the retry limit stores -1.
    prepare(3, "SCE");
    sif_failures = 100;
    BeginConnect__12VIWndConnect(window);
    ok = ok && window[482] == -1;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
