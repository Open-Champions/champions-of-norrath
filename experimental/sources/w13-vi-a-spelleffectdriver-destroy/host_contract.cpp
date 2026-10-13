#include <stdint.h>
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<uintptr_t>(word))
#include "destroy.cpp"
#include <string.h>
#include <string>

static std::string tr; static int len_ret, nodes; static unsigned char* base;
extern "C" int Length__C7VILList(const void* l) { tr += "L" + std::to_string((const unsigned char*)l - base) + ";"; return len_ret; }
extern "C" void Reset__14VICameraEffect(void* e) { tr += "X" + std::to_string((uintptr_t)e) + ";"; }
extern "C" void* RemHead__7VILList(void* l) { tr += "H" + std::to_string((unsigned char*)l - base) + ";"; return nodes-- > 0 ? (void*)(base + 0x200 + nodes) : 0; }
extern "C" void _$_18VISpellEventDriver(void* d, int f) { tr += "D" + std::to_string((unsigned char*)d - base) + "," + std::to_string(f) + ";"; }

int main() {
    alignas(4) static unsigned char d[0x400]; base = d;
    memset(d, 0x55, 0x200);
    *(int*)(d + 4) = 0;
    Destroy__19VISpellEffectDriver(d);
    if (!tr.empty()) return 1;
    for (int c = 0; c < 3; ++c) {
        memset(d, 0x55, 0x200); tr.clear();
        *(int*)(d + 4) = 9; *(int*)(d + 92) = 0x1234; *(int*)(d + 100) = c == 2 ? 0 : 3; len_ret = c == 0 ? 0 : 2; nodes = 2;
        Destroy__19VISpellEffectDriver(d);
        std::string want = "L72;";
        if (c == 1) want += "X4660;";
        want += "H72;D513,3;H72;D512,3;H72;";
        if (tr != want) return 2 + c;
        unsigned char e[0x200]; memset(e, 0x55, sizeof e);
        auto put = [&](int off, int v) { memcpy(e + off, &v, 4); };
        put(0, 0); put(4, 0); put(8, -1); put(12, -1); put(16, 0); put(92, 0); put(100, 0);
        for (int i = 0; i < 8; ++i) put(20 + 4 * i, -1);
        if (memcmp(d, e, 0x200) != 0) return 6 + c;
    }
    return 0;
}
