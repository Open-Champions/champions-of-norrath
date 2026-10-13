#include "clear.cpp"
#include <cstdlib>
#include <initializer_list>
#include <cstring>

static void* freed[4];
static int nfreed;
extern "C" void __builtin_vec_delete(void* p) { freed[nfreed++] = p; }

int main() {
    // host pointers are 8 bytes, so use a layout-independent check on the 32-bit-safe words only
    for (int with_array = 0; with_array < 2; ++with_array) {
        nfreed = 0;
        alignas(8) unsigned char object[96];
        std::memset(object, 0x77, sizeof object);
        alignas(8) unsigned char want[96];
        void* block = &want;
        if (with_array) std::memcpy(object + 52, &block, sizeof block); else std::memset(object + 52, 0, sizeof block);
        std::memcpy(want, object, sizeof object);
        int zero = 0; float ten = 10.0f, one = 1.0f;
        for (int off : {8, 12, 16, 32, 36, 40, 44, 48}) std::memcpy(want + off, &zero, 4);
        std::memcpy(want + 24, &ten, 4); std::memcpy(want + 28, &one, 4);
        std::memset(want + 52, 0, sizeof block);
        if (Clear__13VIHSpriteAnim(object) != 0) return 1;
        if (nfreed != with_array) return 2;
        if (with_array && freed[0] != block) return 3;
        if (std::memcmp(want, object, 52) != 0) return 4;
        if (std::memcmp(want + 52 + sizeof block, object + 52 + sizeof block, 96 - 52 - sizeof block) != 0) return 5;
        if (std::memcmp(object + 52, want + 52, sizeof block) != 0) return 6;
    }
    return 0;
}
