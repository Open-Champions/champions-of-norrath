// Host check only; not a build input. Build: c++ -std=c++17 -arch x86_64 -Wl,-pagezero_size,0x1000 host_check.cpp
// (statics must sit below 4 GiB because the text pointer is a 32-bit word).
// The unit's memmove is renamed so it does not clash with the host declaration.
#define memmove memmove_target
#include "backspace.cpp"
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

int main() {
    bool ok = true;
    if (reinterpret_cast<unsigned long>(text) > 0xffffffffUL) { std::puts("FAIL (statics above 4 GiB)"); return 1; }
    for (int c : {-1, 0, 1, 3, 5}) {
        std::memset(object, 0, sizeof object);
        for (int i = 0; i < 16; ++i) text[i] = static_cast<unsigned short>(100 + i);
        *reinterpret_cast<unsigned*>(object + 424) = static_cast<unsigned>(reinterpret_cast<unsigned long>(text));
        *reinterpret_cast<int*>(object + 428) = 6;
        caret = c; caret_calls = 0; trace.clear();
        const int r = Backspace__9VIWndEdit(object);
        ok = ok && *reinterpret_cast<unsigned long long*>(object + 416) == 0x1122334455667788ull;
        if (c <= 0) {
            ok = ok && r == 0 && trace == "time;" && caret_calls == 1 && text[0] == 100;
        } else {
            ok = ok && r == 1 && caret_calls == 5 && trace == "time;mv" + std::to_string(c - 1) + "," + std::to_string(c) + "," + std::to_string(2 * (6 - c)) + ";set" + std::to_string(c - 1) + ";";
            if (c == 3) ok = ok && text[2] == 103 && text[3] == 104 && text[4] == 105 && text[5] == 105;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
