// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "animation.cpp"
#include <cstdint>
static unsigned char arena[0x2000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
extern "C" void* norrath_host_function(unsigned) { return nullptr; }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
int main() {
    unsigned char* scene = arena;
    unsigned char* records = arena + 0x1000;
    for (int i = 0; i < 0x2000; ++i) arena[i] = 0;
    *reinterpret_cast<unsigned*>(scene + 1028) = word_of(records);
    for (int i = 0; i < 6; ++i) {
        *reinterpret_cast<unsigned*>(records + 12 * i) = 0x1000u + 17u * i;
        *reinterpret_cast<unsigned*>(records + 12 * i + 4) = 0xdead0000u + i;
        *reinterpret_cast<unsigned*>(records + 12 * i + 8) = 0xbeef0000u + i;
    }
    for (int i = 0; i < 6; ++i) if (Animation__7VIScenei(scene, i) != 0x1000u + 17u * i) return 1;
    unsigned char copy[0x2000];
    for (int i = 0; i < 0x2000; ++i) copy[i] = arena[i];
    Animation__7VIScenei(scene, 3);
    for (int i = 0; i < 0x2000; ++i) if (copy[i] != arena[i]) return 2;
    return 0;
}
