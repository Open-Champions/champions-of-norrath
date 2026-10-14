#include <cstring>
extern "C" void worldSetDoorCollision__FP12_worldHeaderii(void* world, int index, int enable);

alignas(8) unsigned char arena[512];
unsigned char* norrath_door_arena = arena;

static unsigned short half(int off) { return *reinterpret_cast<unsigned short*>(arena + off); }

int main() {
    alignas(8) unsigned char header[96];
    std::memset(header, 0, sizeof header);
    std::memset(arena, 0, sizeof arena);
    *reinterpret_cast<unsigned*>(header + 84) = 32u;   // doors at arena+32
    *reinterpret_cast<unsigned*>(header + 32) = 256u;  // items at arena+256
    // door 1 -> item 2; door 0 -> item -1 (negative slot is addressed literally)
    *reinterpret_cast<short*>(arena + 32 + 28 + 24) = 2;
    *reinterpret_cast<short*>(arena + 32 + 24) = 1;
    const int item2 = 256 + 2 * 36 + 16;
    const int item1 = 256 + 1 * 36 + 16;
    *reinterpret_cast<unsigned short*>(arena + item2) = 0xffff;
    *reinterpret_cast<unsigned short*>(arena + item1) = 0x0000;

    worldSetDoorCollision__FP12_worldHeaderii(header, -1, 1);
    if (half(item2) != 0xffff || half(item1) != 0) return 1;
    worldSetDoorCollision__FP12_worldHeaderii(header, 1, 5);
    if (half(item2) != 0xfffd) return 2;
    if (half(item1) != 0) return 3;
    worldSetDoorCollision__FP12_worldHeaderii(header, 1, 0);
    if (half(item2) != 0xffff) return 4;
    worldSetDoorCollision__FP12_worldHeaderii(header, 0, 0);
    if (half(item1) != 2) return 5;
    worldSetDoorCollision__FP12_worldHeaderii(header, 0, -1);
    if (half(item1) != 0) return 6;
    return 0;
}
