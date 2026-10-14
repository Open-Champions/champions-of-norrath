#include "power_off_cb.cpp"

#include <cstdio>
#include <cstring>

int Init_seq = 0;
int _icmd_bind = -1;
static int enabled_next = 1;
static char log_text[24];
static unsigned command_seen;
static void (*handler_seen)(void*, void*);
static void* arg_seen = &enabled_next;
static void mark(const char* s) { std::strcat(log_text, s); }

extern "C" int DIntr() { mark(Init_seq == 1 ? "D1;" : "D?;"); return enabled_next; }
extern "C" int EIntr() { mark("E;"); return 1; }
extern "C" void _sceCd_Poff_Intr(void*, void*) {}
extern "C" int sceSifAddCmdHandler(unsigned c, void (*h)(void*, void*), void* a) {
    mark(Init_seq == 1 ? "A1;" : "A?;");
    command_seen = c; handler_seen = h; arg_seen = a;
    return 0;
}

int main() {
    if (PowerOffCB() != 1) return 1;
    if (std::strcmp(log_text, "D1;A1;E;") != 0) return 2;
    if (command_seen != 0x80000012u || handler_seen != _sceCd_Poff_Intr || arg_seen != nullptr) return 3;
    if (Init_seq != 0 || _icmd_bind != 1) return 4;
    log_text[0] = 0; _icmd_bind = -1; enabled_next = 0;
    if (PowerOffCB() != 1 || std::strcmp(log_text, "D1;A1;") != 0 || _icmd_bind != 1) return 5;
    std::puts("power off cb contract passed");
    return 0;
}
