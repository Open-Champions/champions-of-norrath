// Host contract for worldDeactivateUntakenDoors__FP12_worldHeader. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct _worldHeader;
static int calls[16], n;
extern "C" void worldSetDoorCollision__FP12_worldHeaderii(_worldHeader*, int i, int v) {
    calls[n++] = i * 10 + v;
}
extern "C" void worldDeactivateUntakenDoors__FP12_worldHeader(_worldHeader*);
alignas(8) static unsigned char world[128];
alignas(8) static unsigned char doors[5 * 28];
int main() {
    std::memset(world, 0, sizeof world);
    *reinterpret_cast<int*>(world + 80) = 5;
    *reinterpret_cast<unsigned char**>(world + 84) = doors;
    const unsigned short flags[5] = {0, 1, 0xfffe, 0x3, 0x8000};
    for (int i = 0; i != 5; ++i) *reinterpret_cast<unsigned short*>(doors + 28 * i + 6) = flags[i];
    worldDeactivateUntakenDoors__FP12_worldHeader(reinterpret_cast<_worldHeader*>(world));
    if (n != 3 || calls[0] != 0 || calls[1] != 20 || calls[2] != 40) return 1;
    n = 0;
    *reinterpret_cast<int*>(world + 80) = 0;
    worldDeactivateUntakenDoors__FP12_worldHeader(reinterpret_cast<_worldHeader*>(world));
    if (n != 0) return 2;
    std::printf("deactivate untaken doors contract passed\n");
    return 0;
}
