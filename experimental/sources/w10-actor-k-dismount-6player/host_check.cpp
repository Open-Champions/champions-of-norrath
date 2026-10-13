// Host contract for dismount__6Player; not an EE input. For several signed byte values at +292
// the object must change exactly in the two indexed floats and the two words.
#include <cstring>
extern "C" { unsigned frameNm = 0x13572468u; }
#include <initializer_list>
#include "dismount.cpp"
int main() {
    alignas(16) static unsigned char arena[4096], ref[4096];
    int bad = 0;
    for (int i : {0, 1, 5, 20, 55, -1, -3, -8}) {
        unsigned char* o = arena + 1024;
        std::memset(arena, 0x6d, sizeof arena);
        o[292] = static_cast<unsigned char>(i);
        std::memcpy(ref, arena, sizeof arena);
        dismount__6Player(o);
        const float one = 1.0f, k = __builtin_bit_cast(float, 0xbdaaaaaau); const int two = 2;
        unsigned char* r = ref + 1024;
        std::memcpy(r + i * 32 + 280, &one, 4);
        std::memcpy(r + 1400, &two, 4);
        std::memcpy(r + 1088, &frameNm, 4);
        std::memcpy(r + i * 32 + 284, &k, 4);
        if (std::memcmp(arena, ref, sizeof arena) != 0) bad = 1;
    }
    return bad;
}
