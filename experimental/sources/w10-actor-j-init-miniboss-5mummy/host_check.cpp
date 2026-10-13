// Host contract for initMiniboss__5Mummy; not an EE input.
#include <cstdio>
#include <cstring>
static unsigned roll; static void* seen; static int calls;
extern "C" unsigned eRand__10GameObject(void* s) { ++calls; seen = s; return roll; }
#include "init_miniboss.cpp"
int main() {
    alignas(16) static unsigned char o[128];
    int bad = 0;
    struct { unsigned roll; unsigned char want; } cases[] = {{0, 1}, {17, 18}, {18, 1}, {35, 18}, {0xffffffffu, 0xffffffffu % 18 + 1}, {0x80000000u, 0x80000000u % 18 + 1}};
    for (auto& c : cases) {
        std::memset(o, 0x5a, sizeof o); roll = c.roll; calls = 0; seen = nullptr;
        initMiniboss__5Mummy(o);
        if (o[76] != c.want || calls != 1 || seen != o) { std::printf("fail %u\n", c.roll); bad = 1; }
        for (int i = 0; i < 128; ++i) if (i != 76 && o[i] != 0x5a) bad = 1;
    }
    return bad;
}
