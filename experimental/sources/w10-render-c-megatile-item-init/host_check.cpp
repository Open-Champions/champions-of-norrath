// Host contract for megatileItemInit__FP8MegatilePv. Not an EE plan input.
#include <cstdio>
struct Megatile;
extern "C" void megatileItemInit__FP8MegatilePv(Megatile*, void*);
int main() {
    unsigned w[64];
    for (int i = 0; i < 64; i++) w[i] = i * 10;
    megatileItemInit__FP8MegatilePv(reinterpret_cast<Megatile*>(w), reinterpret_cast<void*>(7));
    for (int i = 0; i < 64; i++) {
        unsigned want = i * 10 + ((i == 0 || i == 2 || i == 50) ? 7 : 0);
        if (w[i] != want) return 1 + i;
    }
    w[0] = 0xfffffffeu;
    megatileItemInit__FP8MegatilePv(reinterpret_cast<Megatile*>(w), reinterpret_cast<void*>(5));
    if (w[0] != 3) return 100;
    std::printf("megatileItemInit contract passed\n");
    return 0;
}
