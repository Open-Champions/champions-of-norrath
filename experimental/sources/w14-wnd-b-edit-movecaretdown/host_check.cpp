// Host check only; not a build input. The unit stores 32-bit pointer words, so the check compiles a copy in which
// the word-to-pointer cast is redirected to an arena:
//   sed 's/static_cast<unsigned long>(word)/norrath_host_long(word)/' movecaretdown.cpp > /tmp/movecaretdown.cpp
//   c++ -std=c++17 -DMOVE=MoveCaretDown__9VIWndEdit -DUNIT='"/tmp/movecaretdown.cpp"' host_check.cpp -o host_check
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char arena[0x2000];
static const unsigned kBase = 0x100000u;
unsigned long norrath_host_long(unsigned word) { return reinterpret_cast<unsigned long>(arena + (word - kBase)); }
static unsigned addr(unsigned offset) { return kBase + offset; }
static void put32(unsigned offset, unsigned v) { std::memcpy(arena + offset, &v, 4); }
static void put16(unsigned offset, int v) { short s = static_cast<short>(v); std::memcpy(arena + offset, &s, 2); }
static int get32(unsigned offset) { int v; std::memcpy(&v, arena + offset, 4); return v; }

#include UNIT

unsigned _5VIWnd$mUI;
static int time_calls;
extern "C" unsigned long long GetTime__7VITimer() { ++time_calls; return 0x1122334455667788ull; }
extern "C" int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(void*, void*, void*, const unsigned short* t) {
    int w = 0; for (; *t; ++t) w += *t * 3 + 1; return w;
}
extern "C" void* memcpy_unused();

// object at 0x000, ui at 0x400, rows at 0x500, row texts at 0x600 + 0x40 * i
static const unsigned kObject = 0, kUi = 0x400, kRows = 0x500, kText = 0x600;
static const char* kWords[3] = {"abc", "dd", "e"};

static int run(int caret, int x, int flag2, bool up, int* result) {
    std::memset(arena, 0, sizeof arena);
    _5VIWnd$mUI = addr(kUi);
    put32(kObject + 404, caret); put32(kObject + 408, static_cast<unsigned>(x));
    put32(kObject + 432, addr(kRows)); put32(kObject + 436, 4);
    const int lens[3] = {3, 2, 1}, flags[3] = {0, 1, flag2};
    for (int i = 0; i < 3; ++i) {
        put32(kRows + 8 * i, addr(kText + 0x40 * i)); put16(kRows + 8 * i + 4, lens[i]); put16(kRows + 8 * i + 6, flags[i]);
        for (int k = 0; k < lens[i]; ++k) put16(kText + 0x40 * i + 2 * k, kWords[i][k]);
    }
    put32(kRows + 24, 0);
    time_calls = 0;
    void* self = arena + kObject;
    *result = MOVE(self);
    return get32(kObject + 404);
}

int main() {
    bool ok = true;
    int r;
    ok = ok && run(1, -1, 0, false, &r) == 3 && r == 1 && get32(408) == 292;
    ok = ok && run(1, 1000, 0, false, &r) == 4 && r == 1;
    ok = ok && run(5, -1, 0, false, &r) == 5 && r == 0 && get32(408) == -1;
    ok = ok && run(5, -1, 1, false, &r) == 6 && r == 1;
    unsigned long long t; std::memcpy(&t, arena + 416, 8);
    ok = ok && t == 0x1122334455667788ull && time_calls == 1;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
