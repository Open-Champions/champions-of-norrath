// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "next.cpp"
#include <cstdint>
static unsigned char arena[0x4000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
extern "C" void* norrath_host_function(unsigned) { return nullptr; }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
int main() {
    unsigned char* scene = arena;
    unsigned char* records = arena + 0x1000;
    unsigned char* obj = arena + 0x1800;
    unsigned char* vals = arena + 0x2000;
    for (int i = 0; i < 0x4000; ++i) arena[i] = 0;
    *reinterpret_cast<unsigned*>(scene + 1076) = word_of(records);
    for (int k = 0; k < 4; ++k) *reinterpret_cast<unsigned*>(records + 12 * k) = word_of(obj + 0x40 * (k == 2 ? 1 : 0));
    // object for index 2 has three values; index 0 object has none
    unsigned char* o2 = obj + 0x40;
    *reinterpret_cast<int*>(o2 + 8) = 3;
    *reinterpret_cast<unsigned*>(o2 + 12) = word_of(vals);
    for (int i = 0; i < 3; ++i) *reinterpret_cast<int*>(vals + 4 * i) = 700 + i;
    // no current: -1 and nothing changes
    *reinterpret_cast<int*>(scene + 1092) = -1; *reinterpret_cast<int*>(scene + 1096) = 5;
    if (GetNextColorBuffer__7VIScene(scene) != -1) return 1;
    if (*reinterpret_cast<int*>(scene + 1096) != 5) return 2;
    *reinterpret_cast<int*>(scene + 1092) = 2; *reinterpret_cast<int*>(scene + 1096) = -1;
    if (GetNextColorBuffer__7VIScene(scene) != 700) return 3;
    if (GetNextColorBuffer__7VIScene(scene) != 701) return 4;
    if (GetNextColorBuffer__7VIScene(scene) != 702) return 5;
    if (GetNextColorBuffer__7VIScene(scene) != -1) return 6;
    if (*reinterpret_cast<int*>(scene + 1096) != 3) return 7;
    if (GetNextColorBuffer__7VIScene(scene) != -1) return 8;
    if (*reinterpret_cast<int*>(scene + 1096) != 4) return 9;
    // object with an empty array
    *reinterpret_cast<int*>(scene + 1092) = 0; *reinterpret_cast<int*>(scene + 1096) = 0;
    if (GetNextColorBuffer__7VIScene(scene) != -1) return 10;
    return 0;
}
