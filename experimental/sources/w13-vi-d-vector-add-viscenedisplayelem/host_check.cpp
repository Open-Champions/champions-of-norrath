// Host check only; not a build input. Record words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "add.cpp"
#include <cstdio>
#include <cstring>
alignas(16) static unsigned char arena[0x1000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static int allocCalls, allocRet;
extern "C" int Allocate__t8VIVector1Z18VISceneDisplayElem(void* vector) {
    ++allocCalls;
    unsigned* w = static_cast<unsigned*>(vector);
    if (allocRet >= 0) w[2] = 0x400;   // storage appears at 0x400 after a successful allocation
    return allocRet;
}
int main() {
    unsigned* v = reinterpret_cast<unsigned*>(arena + 0x100);
    unsigned elem[3] = {0x11111111u, 0x22222222u, 0x33333333u};
    bool ok = true;
    struct Case { unsigned count, cap, data; int allocRet; int result; unsigned newCap, newCount; bool alloc; unsigned slot; bool copied; } cases[] = {
        {0, 0, 0, 0, 0, 1, 1, true, 0x400, true},
        {0, 0, 0, -4, -1, 1, 0, true, 0, false},
        {3, 3, 0x300, 0, 3, 6, 4, true, 0x400 + 36, true},
        {3, 3, 0x300, -1, -1, 6, 3, true, 0, false},
        {2, 8, 0x300, 0, 2, 8, 3, false, 0x300 + 24, true},
        {0, 5, 0x300, 0, 0, 5, 1, false, 0x300, true},
    };
    for (auto& c : cases) {
        std::memset(arena, 0x5a, sizeof arena);
        v[0] = c.count; v[1] = c.cap; v[2] = c.data;
        allocCalls = 0; allocRet = c.allocRet;
        int r = Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem(v, elem);
        if (r != c.result || v[0] != c.newCount || v[1] != c.newCap || allocCalls != (c.alloc ? 1 : 0)) ok = false;
        if (c.copied && std::memcmp(arena + c.slot, elem, 12) != 0) ok = false;
        if (!c.copied && arena[0x400] != 0x5a) ok = false;
    }
    // A zero storage word with index zero makes a zero slot address: nothing is copied but the count still advances.
    std::memset(arena, 0x5a, sizeof arena);
    v[0] = 0; v[1] = 4; v[2] = 0; allocCalls = 0;
    if (Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem(v, elem) != 0 || v[0] != 1 || allocCalls != 0) ok = false;
    if (arena[0] != 0x5a) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
