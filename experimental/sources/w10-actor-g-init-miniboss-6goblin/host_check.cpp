// Host contract for initMiniboss__6Goblin; not an EE input.
#include <cstdio>
#include <cstring>
static unsigned roll; static void* rollSelf;
extern "C" unsigned eRand__10GameObject(void* s) { rollSelf = s; return roll; }
#include "init_miniboss.cpp"
int main() {
    alignas(16) unsigned char object[128]; int bad = 0;
    const unsigned rolls[] = {0, 1, 14, 15, 16, 29, 30, 0xfffffffeu, 0xffffffffu};
    for (unsigned r : rolls) {
        std::memset(object, 0x77, sizeof object); roll = r;
        initMiniboss__6Goblin(object);
        if (object[76] != r % 15u + 1u || rollSelf != object) { std::printf("fail roll %u\n", r); bad = 1; }
        for (int i = 0; i < 128; ++i) if (i != 76 && object[i] != 0x77) { std::puts("fail: stray write"); bad = 1; }
    }
    if (!bad) std::puts("goblin initMiniboss ok");
    return bad;
}
