// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "replace_proxy_rooms.cpp"
#include <cstdio>
#include <cstring>
alignas(16) static unsigned char arena[0x8000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

static int mode;
static int finds;
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* value) {
    ++finds;
    if (dictionary != arena + 0x6000) return -100;
    if (mode == 0) return 0;
    *type = mode == 1 ? 4 : 3;
    *value = static_cast<int>(id) + 1000;
    return 1;
}

int main() {
    bool ok = true;
    for (int m = 0; m < 3; ++m) {
        std::memset(arena, 0, sizeof arena);
        mode = m;
        finds = 0;
        unsigned* scene = reinterpret_cast<unsigned*>(arena);
        scene[19352 / 4] = 0x6000;
        unsigned char* zone = arena + 0x4000;
        unsigned* za = reinterpret_cast<unsigned*>(zone + 92);
        za[0] = 3;
        za[1] = 0x1000;
        unsigned* rooms = reinterpret_cast<unsigned*>(arena + 0x1000);
        for (int i = 0; i < 3; ++i) {
            rooms[30 * i] = (i == 1) ? 0x5u : 0x7u;  // only records 0 and 2 have mask 0x2
            rooms[30 * i + 10] = 40 + i;
            rooms[30 * i + 7] = 0x77;
        }
        int r = ReplaceProxyRooms__7VISceneR6VIZone(arena, zone);
        if (r != (m == 2 ? -1 : 0)) ok = false;
        if (finds != (m == 2 ? 1 : 2)) ok = false;
        for (int i = 0; i < 3; ++i) {
            bool resolved = m == 1 && i != 1;
            if (rooms[30 * i] != (resolved ? 0x5u : (i == 1 ? 0x5u : 0x7u))) ok = false;
            if (rooms[30 * i + 7] != (resolved ? 1040u + i : 0x77u)) ok = false;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
