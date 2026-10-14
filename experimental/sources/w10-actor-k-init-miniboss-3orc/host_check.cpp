// Host contract for initMiniboss__3Orc; not an EE input. The random source is replaced by a
// queue of values; the byte at +76 must be value % 18 + 1 and no other byte may change.
#include <cstring>
static unsigned queue[8]; static int qn, qi; static void* arg;
extern "C" unsigned eRand__10GameObject(void* self) { arg = self; return queue[qi++]; }
#include "init_miniboss.cpp"
int main() {
    static unsigned char o[128], ref[128];
    const unsigned vals[] = {0u, 17u, 18u, 35u, 36u, 0xffffffffu, 0x80000000u, 12345u};
    int bad = 0;
    for (unsigned v : vals) {
        std::memset(o, 0x5a, sizeof o); std::memcpy(ref, o, sizeof o);
        qi = 0; queue[0] = v; arg = nullptr;
        initMiniboss__3Orc(o);
        ref[76] = static_cast<unsigned char>(v % 18u + 1u);
        if (arg != o || qi != 1 || std::memcmp(o, ref, sizeof o) != 0) bad = 1;
    }
    return bad;
}
