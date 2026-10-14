// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "render_leaf.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
alignas(16) static unsigned char arena[0x4000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

static int calls, last_room, room_result;
static unsigned char* last_zone;
static unsigned char* last_vars;
extern "C" int RenderRoom__6VIZoneiR15VISceneRendVars(unsigned char* zone, int room, unsigned char* vars) {
    ++calls; last_zone = zone; last_room = room; last_vars = vars;
    return room_result;
}

int main() {
    bool ok = true;
    unsigned char* zone = arena + 0x100;
    unsigned char* vars = arena + 0x200;
    for (unsigned stamp_equal = 0; stamp_equal < 2; ++stamp_equal)
    for (unsigned flags : {0u, 1u})
    for (int vars44 : {0, 7})
    for (int result : {0, 5, -3}) {
        std::memset(arena, 0, sizeof arena);
        unsigned* z = reinterpret_cast<unsigned*>(zone);
        z[20] = 0x1000;
        z[24] = 0x2000;
        unsigned* table = reinterpret_cast<unsigned*>(arena + 0x1000);
        table[0] = 9; table[2] = 1; table[4] = 2;   // leaf 2 -> room index 2
        unsigned* v = reinterpret_cast<unsigned*>(vars);
        v[0] = 0x77;
        v[11] = vars44;
        unsigned* room = reinterpret_cast<unsigned*>(arena + 0x2000 + 120 * 2);
        room[0] = flags;
        room[8] = stamp_equal ? 0x77u : 0x11u;
        calls = 0; room_result = result;
        int r = RenderLeaf__6VIZoneiR15VISceneRendVars(zone, 2, vars);
        bool skip_equal = stamp_equal == 1;
        bool skip_flag = !skip_equal && !(flags & 1) && vars44 != 0;
        bool renders = !skip_equal && !skip_flag;
        int want = renders && result < 0 ? -1 : 0;
        if (r != want) ok = false;
        if (calls != (renders ? 1 : 0)) ok = false;
        if (renders && (last_zone != zone || last_room != 2 || last_vars != vars)) ok = false;
        if (room[8] != 0x77u) ok = false;              // stamp written unless it already matched (then unchanged = 0x77)
        if (skip_equal == false && room[8] != 0x77u) ok = false;
        if (room[0] != flags) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
