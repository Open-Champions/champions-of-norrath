// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "allocate.cpp"
#include <cstdio>
#include <cstring>
alignas(16) static unsigned char arena[0x2000];
unsigned norrath_host_address(const void* pointer) { return pointer ? static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - arena) : 0u; }
static unsigned want_size; static bool fail_new; static int new_calls;
extern "C" void* __builtin_vec_new(unsigned size) { ++new_calls; want_size = size; return fail_new ? nullptr : arena + 0x800; }

int main() {
    bool ok = true;
    unsigned* o = reinterpret_cast<unsigned*>(arena + 0x100);
    std::memset(arena, 0x5a, sizeof arena); o[0] = 0; o[1] = 0x77; new_calls = 0; fail_new = false;
    if (Allocate__t7VIArray1Z10VIZoneLeaf(o) != 0 || new_calls || o[1] != 0x77) ok = false;
    std::memset(arena, 0x5a, sizeof arena); o[0] = 3; o[1] = 0x77; fail_new = true; new_calls = 0;
    if (Allocate__t7VIArray1Z10VIZoneLeaf(o) != -1 || new_calls != 1 || want_size != 24 || o[1] != 0) ok = false;
    std::memset(arena, 0x5a, sizeof arena); o[0] = 3; fail_new = false; new_calls = 0;
    if (Allocate__t7VIArray1Z10VIZoneLeaf(o) != 0 || o[0] != 3 || o[1] != 0x800 || want_size != 24) ok = false;
    for (int b = 0; b < 0x100; ++b) if (arena[0x800 + b] != 0x5a) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
