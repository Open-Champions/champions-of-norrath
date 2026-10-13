#include "ctor.cpp"

#include <cstring>

static char* seen[64];
static int kind[64];
static int n;
extern "C" void __7VILList(void* l) { kind[n] = 1; seen[n++] = static_cast<char*>(l); }
extern "C" void __7VILNode(void* l) { kind[n] = 2; seen[n++] = static_cast<char*>(l); }
extern "C" void __t6VIPool1ZP13VISpellEffect(void* l) { kind[n] = 3; seen[n++] = static_cast<char*>(l); }

int main() {
    alignas(8) static unsigned char object[1200];
    std::memset(object, 0x5a, sizeof object);
    __19VISpellEffectSystem(object);
    if (n != 27) return 1;
    if (kind[0] != 1 || seen[0] != reinterpret_cast<char*>(object) + 100) return 2;
    if (kind[1] != 1 || seen[1] != reinterpret_cast<char*>(object) + 112) return 3;
    for (int i = 0; i < 24; ++i)
        if (kind[2 + i] != 2 || seen[2 + i] != reinterpret_cast<char*>(object) + 128 + 40 * i) return 4;
    if (kind[26] != 3 || seen[26] != reinterpret_cast<char*>(object) + 1112) return 5;
    int v;
    std::memcpy(&v, object + 1092, 4);
    if (v != 0) return 6;
    for (unsigned i = 0; i < sizeof object; ++i)
        if ((i < 1092 || i >= 1096) && object[i] != 0x5a) return 7;
    return 0;
}
