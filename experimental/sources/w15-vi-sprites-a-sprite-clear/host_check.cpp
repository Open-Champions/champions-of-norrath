// Host check only; not a build input.
#include "clear.cpp"
#include <cstring>
static char order[16]; static int n; static int failAt; static void* seen[6];
static int step(int k, void* c) { order[n] = static_cast<char>('A' + k); seen[n] = c; ++n; return k == failAt ? -3 : k + 1; }
extern "C" int Clear__t8VIVector1Z13VIHSpriteNode(void* c) { return step(0, c); }
extern "C" int Clear__t8VIVector1Z10VIMatrix44(void* c) { return step(1, c); }
extern "C" int Clear__t7VIArray1Z16VIHSpriteTrigger(void* c) { return step(2, c); }
extern "C" int Clear__t6VIPool1Z19VIHSpriteAttachment(void* c) { return step(3, c); }
extern "C" int Clear__t6VIPool1Z13VIHSpritePlay(void* c) { return step(4, c); }
extern "C" int Clear__t6VIPool1Z17VIHSpritePlayNode(void* c) { return step(5, c); }
int main() {
    alignas(8) unsigned char sprite[512];
    const int offs[6] = {64, 76, 88, 96, 120, 144};
    for (int fail = -1; fail < 6; ++fail) {
        std::memset(sprite, 0x11, sizeof sprite); n = 0; std::memset(order, 0, sizeof order); failAt = fail;
        int r = Clear__9VIHSprite(sprite);
        int want = fail < 0 ? 6 : fail + 1;
        if (n != want || r != (fail < 0 ? 0 : -1)) return 1 + fail;
        for (int i = 0; i < n; ++i) if (order[i] != 'A' + i || seen[i] != sprite + offs[i]) return 10;
        int a, b; float f; std::memcpy(&a, sprite + 388, 4); std::memcpy(&b, sprite + 384, 4); std::memcpy(&f, sprite + 208, 4);
        bool done = fail < 0;
        if (done != (a == -1 && b == -1 && f == 1.0f)) return 11;
        if (!done && (a != 0x11111111 || b != 0x11111111)) return 12;
    }
    return 0;
}
