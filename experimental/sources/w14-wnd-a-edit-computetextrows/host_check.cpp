// Host check only; not a build input. Build: c++ -std=c++17 -arch x86_64 -Wl,-pagezero_size,0x1000 host_check.cpp
// (statics must sit below 4 GiB because the pointers are 32-bit words).
#define memcpy memcpy_target
#include "computetextrows.cpp"
#undef memcpy
#include <cstdio>
#include <cstring>

unsigned _5VIWnd$mUI;
alignas(8) static unsigned char object[512];
alignas(8) static unsigned char ui[128];
static unsigned short text[64];
static EditRow rows[16];
extern "C" {
int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(void*, void*, void*, const unsigned short* t) {
    int n = 0;
    while (t[n]) ++n;
    return n * 100;  // four units reach the 389 limit
}
void* memcpy_target(void* d, const void* s, unsigned n) { return std::memcpy(d, s, n); }
}

static int run(const char* s, int capacity) {
    std::memset(object, 0, sizeof object);
    std::memset(rows, 0x55, sizeof rows);
    std::memset(text, 0, sizeof text);
    int n = 0;
    for (; s[n]; ++n) text[n] = static_cast<unsigned char>(s[n]);
    *reinterpret_cast<unsigned*>(object + 424) = static_cast<unsigned>(reinterpret_cast<unsigned long>(text));
    *reinterpret_cast<int*>(object + 428) = capacity;
    *reinterpret_cast<unsigned*>(object + 432) = static_cast<unsigned>(reinterpret_cast<unsigned long>(rows));
    *reinterpret_cast<int*>(object + 436) = 99;
    ComputeTextRows__9VIWndEdit(object);
    return *reinterpret_cast<int*>(object + 436);
}
static bool row(int i, int start, int length, int flag) {
    return rows[i].start == static_cast<unsigned>(reinterpret_cast<unsigned long>(text + start)) && rows[i].length == length && rows[i].flag == flag;
}

int main() {
    bool ok = true;
    if (reinterpret_cast<unsigned long>(text) > 0xffffffffUL) { std::puts("FAIL (statics above 4 GiB)"); return 1; }
    _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(ui));
    ok = ok && run("", 4) == 0 && rows[0].start == 0 && rows[3].length == 0 && rows[4].start == 0x55555555u;
    ok = ok && run("ab", 4) == 1 && row(0, 0, 2, 0);
    ok = ok && run("ab\ncd", 4) == 2 && row(0, 0, 3, 1) && row(1, 3, 2, 0);
    ok = ok && run("aaaa bb", 4) == 2 && row(0, 0, 5, 1) && row(1, 5, 2, 0);
    ok = ok && run("aaaaaaa", 4) == 2 && row(0, 0, 4, 1) && row(1, 4, 3, 0);
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
