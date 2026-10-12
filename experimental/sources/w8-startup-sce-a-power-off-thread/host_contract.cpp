#define NORRATH_HOST_CONTRACT 1
#define printf norrath_host_printf
#include "power_off_thread.cpp"

#include <cstdio>
#include <cstring>

int sid = 12;
unsigned char powerOffMode = 0;
static char log_text[160];
static void note(const char* text) { std::strcat(log_text, text); }
static int devctl_calls, cd_calls, spins;

extern "C" int WaitSema(int semaphore) { note(semaphore == 12 ? "W;" : "W?;"); return 0; }
extern "C" int scePrintf(const char* f, ...) { note(powerOffMode == 1 ? "[m1]" : "[m0]"); note(f); note(";"); return 0; }
extern "C" int norrath_host_printf(const char* f, ...) { note(f); note(";"); return 0; }
extern "C" void netFree__Fb(bool flag) { note(flag ? "N1;" : "N0;"); }
extern "C" int sceDevctl(const char* d, int c, void* a, int b, void* e, int f) {
    if (std::strcmp(d, "dev9x:") != 0 || c != 0x4402 || a || b || e || f) note("DEVBAD;");
    note("V;");
    return ++devctl_calls < 3 ? -1 : 0;
}
extern "C" int sceCdPowerOff(int* result) {
    note("C;");
    ++cd_calls;
    if (cd_calls == 1) return 0;        // retried
    if (cd_calls == 2) { *result = 5; return 1; }  // retried: word nonzero
    *result = 0;
    return 1;
}
extern "C" bool norrath_host_keep_spinning() { return ++spins < 3; }

int main() {
    PowerOffThread__FPv(nullptr);
    const char* expected =
        "W;[m1]power off request has come.\n;N1;dev9 go down!\n;V;V;V;dev9 down\n;C;C;C;power off\n;";
    if (std::strcmp(log_text, expected) != 0) { std::fputs(log_text, stderr); return 1; }
    if (spins != 3) return 2;
    std::puts("power off thread contract passed");
    return 0;
}
