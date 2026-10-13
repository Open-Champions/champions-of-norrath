// Host check only; not an EE build input.
#include "init_miniboss.cpp"
#include <cstdio>
#include <cstring>
static int failures, calls;
static unsigned next_draw; static void* seen;
extern "C" int eRand__10GameObject(void* self) { ++calls; seen = self; return static_cast<int>(next_draw); }
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    unsigned char object[96];
    const unsigned draws[] = {0u, 1u, 19u, 20u, 39u, 0x7fffffffu, 0x80000000u, 0xffffffffu};
    const unsigned want[] = {1, 2, 20, 1, 20, 8, 9, 16};
    for (int i = 0; i < 8; ++i) {
        std::memset(object, 0xaa, sizeof object);
        next_draw = draws[i]; calls = 0;
        initMiniboss__6Wraith(object);
        expect(calls == 1 && seen == object, "one eRand call on the object");
        expect(object[76] == want[i], "unsigned remainder plus one");
        for (int j = 0; j < 96; ++j) if (j != 76) expect(object[j] == 0xaa, "only +76 written");
    }
    if (!failures) std::puts("initMiniboss ok");
    return failures ? 1 : 0;
}
