// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "ref_map.cpp"
#include <cstdio>
alignas(16) static unsigned char arena[0x2000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

int main() {
    bool ok = true;
    unsigned* scene = reinterpret_cast<unsigned*>(arena);
    scene[1052 / 4] = 0x800;
    unsigned* table = reinterpret_cast<unsigned*>(arena + 0x800);
    for (int i = 0; i < 12; ++i) table[i] = 0x1000 + i;
    for (int i = 0; i < 4; ++i)
        if (RefMap__7VIScenei(arena, i) != 0x1000u + 3 * i) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
