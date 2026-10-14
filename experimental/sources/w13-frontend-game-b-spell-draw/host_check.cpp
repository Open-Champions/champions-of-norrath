// Host contract for draw__5Spell. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" void draw__5Spell(void*);
int main() {
    unsigned char object[64];
    std::memset(object, 0x5a, sizeof object);
    draw__5Spell(object);
    for (unsigned char b : object)
        if (b != 0x5a) { std::fprintf(stderr, "fail: object touched\n"); return 1; }
    std::printf("Spell::draw contract passed\n");
    return 0;
}
