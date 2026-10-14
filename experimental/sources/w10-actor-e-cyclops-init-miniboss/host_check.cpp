// Host contract for initMiniboss__7Cyclops. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "init_miniboss.cpp"
#include <cstdio>
static int next_roll = 0, calls = 0;
static void* seen = nullptr;
extern "C" int eRand__10GameObject(void* object) { ++calls; seen = object; return next_roll; }
int main() {
    const int rolls[] = {0, 1, 8, 9, 10, 17, 18, 100, 0x7fffffff, -1, -9, int(0x80000000u)};
    for (int roll : rolls) {
        unsigned char object[96];
        for (unsigned i = 0; i < sizeof object; ++i) object[i] = 0x5a;
        next_roll = roll; calls = 0;
        initMiniboss__7Cyclops(object);
        const unsigned want = static_cast<unsigned>(roll) % 9u + 1u;
        if (calls != 1 || seen != object || object[76] != want) { std::printf("fail roll %d\n", roll); return 1; }
        for (unsigned i = 0; i < sizeof object; ++i)
            if (i != 76 && object[i] != 0x5a) { std::printf("stray store at %u\n", i); return 1; }
    }
    return 0;
}
