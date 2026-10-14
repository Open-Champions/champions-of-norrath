#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "init.cpp"
#include <string.h>

static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
static int order[8], n;
static int ret_tree, ret_array, ret_room;
static void* seen[8];
static unsigned flags_at_tree;
extern "C" int Init__10VIZoneTreeii(void* tree, int a, int b) {
    order[n++] = 1; seen[0] = tree; flags_at_tree = W(0);
    if (a != 0 || b != 1) order[n++] = 99;
    return ret_tree;
}
extern "C" int Init__t7VIArray1Z10VIZoneRoomi(void* array, int count) {
    order[n++] = 2; seen[1] = array;
    if (count != 1) order[n++] = 98;
    return ret_array;
}
extern "C" int Init__10VIZoneRoomiiUiUiR6VIBBox(void* room, int a, int b, unsigned c, unsigned d, void* box) {
    order[n++] = 3; seen[2] = room; seen[3] = box;
    if (a || b || c || d) order[n++] = 97;
    return ret_room;
}

static int run(int rt, int ra, int rr) {
    memset(arena, 0, 512);
    W(80) = arena_base + 256; W(96) = arena_base + 320;
    W(256) = 0x11111111; W(260) = 0x22222222; W(320) = 0x10; W(324) = 0x33333333;
    ret_tree = rt; ret_array = ra; ret_room = rr; n = 0;
    return Init__6VIZone(arena);
}

int main() {
    if (run(0, 0, 0) != 0 || n != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3) return 1;
    if (seen[0] != arena + 68 || seen[1] != arena + 92 || seen[2] != arena + 320 || seen[3] != arena + 44) return 2;
    if (flags_at_tree != 1) return 3;                       // bit set before the first call
    if (W(0) != 1 || W(256) != 0 || W(260) != 0 || W(320) != 0x11 || W(324) != 0x33333333) return 4;
    if (run(-1, 0, 0) != -1 || n != 1 || W(0) != 1 || W(256) != 0x11111111) return 5;
    if (run(0, -2, 0) != -1 || n != 2 || W(256) != 0x11111111) return 6;
    if (run(5, 5, -3) != -1 || n != 3 || W(256) != 0 || W(320) != 0x10) return 7;
    if (run(5, 6, 7) != 0 || W(320) != 0x11) return 8;
    return 0;
}
