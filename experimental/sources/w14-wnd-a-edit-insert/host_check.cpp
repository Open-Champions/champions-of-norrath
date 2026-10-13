// Host check only; not a build input. Build: c++ -std=c++17 -arch x86_64 -Wl,-pagezero_size,0x1000 host_check.cpp
// (statics must sit below 4 GiB because the text pointer is a 32-bit word).
// The unit's memmove is renamed so it does not clash with the host declaration.
#define memmove memmove_target
#include "insert.cpp"
#undef memmove
#include <cstdio>
#include <cstring>
#include <string>

alignas(8) static unsigned char object[512];
static unsigned short text[16];
static int caret, caret_calls;
static std::string trace;
extern "C" {
unsigned long long GetTime__7VITimer() { trace += "time;"; return 0x1122334455667788ull; }
int CaretOffset__9VIWndEdit(void*) { ++caret_calls; return caret; }
int SetCaretOffset__9VIWndEditi(void*, int o) { trace += "set" + std::to_string(o) + ";"; return 0; }
void* memmove_target(void* d, const void* s, unsigned n) {
    trace += "mv" + std::to_string(static_cast<unsigned short*>(d) - text) + "," + std::to_string(static_cast<const unsigned short*>(s) - text) + "," + std::to_string(n) + ";";
    return __builtin_memmove(d, s, n);
}
}

static int run(int c, unsigned ch, int flag, int n) {
    std::memset(object, 0, sizeof object);
    for (int i = 0; i < 16; ++i) text[i] = static_cast<unsigned short>(100 + i);
    *reinterpret_cast<unsigned*>(object + 424) = static_cast<unsigned>(reinterpret_cast<unsigned long>(text));
    *reinterpret_cast<int*>(object + 428) = n;
    *reinterpret_cast<int*>(object + 444) = flag;
    caret = c; caret_calls = 0; trace.clear();
    return Insert__9VIWndEditUs(object, ch);
}

int main() {
    bool ok = true;
    if (reinterpret_cast<unsigned long>(text) > 0xffffffffUL) { std::puts("FAIL (statics above 4 GiB)"); return 1; }
    // Space is refused without the flag, accepted with it; high argument bits are ignored.
    ok = ok && run(2, 32, 0, 8) == 0 && trace.empty() && caret_calls == 0;
    ok = ok && run(2, 0x10020, 0, 8) == 0 && trace.empty();
    ok = ok && run(2, 32, 1, 8) == 1;
    // A full buffer (caret + 1 not below n) is refused after one caret query and no write.
    ok = ok && run(7, 65, 0, 8) == 0 && caret_calls == 1 && trace.empty() && text[7] == 107;
    // Normal insert at 2 with n = 8: tail text[2..6] moves up one place, last slot cleared, new char at 2.
    ok = ok && run(2, 0x20041, 0, 8) == 1 && caret_calls == 6 &&
         trace == "time;mv3,2,10;set3;" &&
         text[2] == 0x41 && text[3] == 102 && text[4] == 103 && text[5] == 104 && text[6] == 105 && text[7] == 0 &&
         *reinterpret_cast<unsigned long long*>(object + 416) == 0x1122334455667788ull;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
