// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "get.cpp"
#include <cstdint>
static unsigned char arena[0x2000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
extern "C" void* norrath_host_function(unsigned) { return nullptr; }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
static int lastIndex = -99; static unsigned char* lastScene;
extern "C" unsigned Sprite__7VIScenei(unsigned char* scene, int index) {
    lastScene = scene; lastIndex = index;
    return word_of(arena + 0x400 + 0x100 * (index & 3));
}
int main() {
    for (int i = 0; i < 0x2000; ++i) arena[i] = static_cast<unsigned char>(i * 3);
    for (int k = 0; k < 4; ++k) *reinterpret_cast<int*>(arena + 0x400 + 0x100 * k + 52) = 100 + k;
    const int indices[] = {0, 1, 2, 3, 7};
    for (int index : indices) {
        int got = GetSpriteType__7VIScenei(arena + 8, index);
        if (got != 100 + (index & 3)) return 1;
        if (lastIndex != index || lastScene != arena + 8) return 2;
    }
    return 0;
}
