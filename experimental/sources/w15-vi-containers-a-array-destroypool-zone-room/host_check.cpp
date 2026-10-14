// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "destroypool.cpp"
#include <cstdio>
#include <cstring>
#include <vector>
alignas(16) static unsigned char arena[0x2000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static std::vector<unsigned> addrs; static std::vector<int> flagv;
static unsigned* obj;
extern "C" void _$_10VIZoneRoom(void* room, int flags) {
    addrs.push_back(static_cast<unsigned>(static_cast<unsigned char*>(room) - arena)); flagv.push_back(flags);
}
int main() {
    bool ok = true;
    obj = reinterpret_cast<unsigned*>(arena + 0x100);
    for (unsigned n : {0u, 1u, 2u, 5u}) {
        std::memset(arena, 0x5a, sizeof arena); addrs.clear(); flagv.clear();
        obj[0] = n; obj[1] = 0x400;
        if (DestroyPool__t7VIArray1Z10VIZoneRoom(obj) != 0 || addrs.size() != n) ok = false;
        for (unsigned i = 0; i < addrs.size(); ++i) if (addrs[i] != 0x400 + 120 * i || flagv[i] != 2) ok = false;
        if (obj[0] != n || obj[1] != 0x400) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
