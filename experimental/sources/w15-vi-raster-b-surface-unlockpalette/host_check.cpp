// Host check only; not a build input.
static int calls; static unsigned char* seen;
extern "C" void ReorderPalette__9VISurface(unsigned char* s) { ++calls; seen = s; }
#include "unlock_palette.cpp"
#include <cstdio>
int main() {
    alignas(4) unsigned char s[0x80] = {};
    int* tok = reinterpret_cast<int*>(s + 0x1c); int* want = reinterpret_cast<int*>(s + 0x40);
    *tok = 5; *want = 5;
    bool ok = UnlockPalette__9VISurface(s) == 0 && calls == 1 && seen == s && *tok == -1 && *want == 5;
    *tok = 5; *want = 6;
    ok = ok && UnlockPalette__9VISurface(s) == -1 && calls == 1 && *tok == 5 && *want == 6;
    *tok = -1; *want = -1;
    ok = ok && UnlockPalette__9VISurface(s) == 0 && calls == 2 && *tok == -1;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
