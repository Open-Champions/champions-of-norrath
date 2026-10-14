// Host check only; not a build input.
#include "allocate.cpp"
alignas(8) static unsigned char object[64];
static unsigned char block[16];
static int calls;
static unsigned lastSize;
static bool fail;
extern "C" void* __builtin_vec_new(unsigned size) { ++calls; lastSize = size; return fail ? nullptr : block; }
int main() {
    unsigned* w = reinterpret_cast<unsigned*>(object);
    if (Allocate__13VIHSpriteAnim(object) != 0 || calls != 0 || w[13] != 0) return 1;
    w[12] = 0x240;
    if (Allocate__13VIHSpriteAnim(object) != 0 || calls != 1 || lastSize != 0x240) return 2;
    if (w[13] != static_cast<unsigned>(reinterpret_cast<unsigned long>(block))) return 3;
    fail = true; w[13] = 77;
    if (Allocate__13VIHSpriteAnim(object) != -1 || w[13] != 0) return 4;
    return 0;
}
