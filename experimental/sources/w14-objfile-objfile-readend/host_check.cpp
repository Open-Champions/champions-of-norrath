// Host check only; not a build input.
#include "readend.cpp"
#include <cstdio>
#include <cstring>
alignas(8) static unsigned char o[1024];
static int seeks; static long long seek_off; static int seek_origin; static void* seek_obj;
extern "C" int Seek__6VIFilelQ26VIFile10SeekOrigin(void* f, long long off, int origin) { ++seeks; seek_obj = f; seek_off = off; seek_origin = origin; return 0; }
static int& w(int off) { return *reinterpret_cast<int*>(o + off); }
int main() {
    bool ok = true;
    // closed
    std::memset(o, 0x11, sizeof o); w(576) = 0; seeks = 0;
    if (ReadEnd__9VIObjFile(o) != -1 || seeks || w(232) != 0x11111111) ok = false;
    // depth 2, unread bytes remain in the scope: seek, zero, charge the parent
    const int lefts[] = {40, 0, -3};
    for (int left : lefts) {
        std::memset(o, 0x11, sizeof o); w(576) = 1; w(232) = 2; seeks = 0;
        w(256 + 64 + 8) = left;
        w(256 + 32 + 8) = 1000; w(256 + 32 + 20) = 100;
        if (ReadEnd__9VIObjFile(o) != 0) ok = false;
        if (left > 0 ? (seeks != 1 || seek_off != left || seek_origin != 0 || seek_obj != o || w(256 + 64 + 8) != 0) : (seeks != 0 || w(256 + 64 + 8) != left)) ok = false;
        if (w(232) != 1 || w(256 + 32 + 8) != 1000 - 100 - 12 || w(584) != 0 || w(576) != 1) ok = false;
    }
    // depth 0: pops below zero, clears the flag, parent untouched
    std::memset(o, 0x11, sizeof o); w(576) = 1; w(232) = 0; seeks = 0; w(256 + 8) = 7;
    if (ReadEnd__9VIObjFile(o) != 0 || seeks != 1 || seek_off != 7 || w(232) != -1 || w(576) != 0 || w(584) != 0 || w(256 + 8) != 0) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
