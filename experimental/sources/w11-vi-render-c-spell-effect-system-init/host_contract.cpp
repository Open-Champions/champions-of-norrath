// The scene pointer is stored as a 32-bit word in the guest layout. The host test uses a
// four-byte stand-in for it by checking the low word written.
#include "init.cpp"

#include <cstring>

static char* inits[4];
static int ni;
static char* tails[40][2];
static int nt;
extern "C" void Init__7VILList(void* l) { inits[ni++] = static_cast<char*>(l); }
extern "C" void AddTail__7VILListP7VILNode(void* l, void* n) {
    tails[nt][0] = static_cast<char*>(l); tails[nt][1] = static_cast<char*>(n); ++nt;
}

int main() {
    alignas(8) static unsigned char object[1200];
    std::memset(object, 0x5a, sizeof object);
    void* scene = reinterpret_cast<void*>(0x1234);
    if (Init__19VISpellEffectSystemP7VIScene(object, scene) != 0) return 1;
    void* stored;
    std::memcpy(&stored, object, sizeof stored);
    if (stored != scene) return 2;
    if (ni != 2 || inits[0] != reinterpret_cast<char*>(object) + 100 || inits[1] != reinterpret_cast<char*>(object) + 112) return 3;
    if (nt != 24) return 4;
    for (int i = 0; i < 24; ++i)
        if (tails[i][0] != reinterpret_cast<char*>(object) + 112 || tails[i][1] != reinterpret_cast<char*>(object) + 128 + 40 * i) return 5;
    auto word = [&](int off) { int v; std::memcpy(&v, object + off, 4); return v; };
    for (int i = 0; i < 24; ++i) if (word(4 + 4 * i) != 0) return 6;
    if (word(1092) != 1 || word(1088) != 0x16062 || word(1108) != -1 || word(1096) != 0 ||
        word(1100) != 0 || word(1104) != 0) return 7;
    // The scene pointer slot is pointer-sized on the host; everything else outside the written
    // ranges (100..127 and 1112..) must be untouched.
    for (int off = 100; off < 128; ++off) if (object[off] != 0x5a) return 8;
    for (int off = 1112; off < 1200; ++off) if (object[off] != 0x5a) return 9;
    return 0;
}
