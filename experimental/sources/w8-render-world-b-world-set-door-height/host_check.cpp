#include <cstring>
extern "C" void worldSetDoorHeight__FP12_worldHeaderif(void* world, int index, float height);

alignas(8) unsigned char arena[512];
unsigned char* norrath_door_arena = arena;
static int word(int off) { return *reinterpret_cast<int*>(arena + off); }

int main() {
    alignas(8) unsigned char header[96];
    std::memset(header, 0, sizeof header);
    std::memset(arena, 0x11, sizeof arena);
    *reinterpret_cast<unsigned*>(header + 84) = 32u;
    *reinterpret_cast<unsigned*>(header + 32) = 256u;
    *reinterpret_cast<short*>(arena + 32 + 2 * 28 + 24) = 3;
    *reinterpret_cast<short*>(arena + 32 + 0 * 28 + 24) = -1;
    const int item3 = 256 + 3 * 36 + 28;
    const int before = word(item3);
    worldSetDoorHeight__FP12_worldHeaderif(header, -4, 9.0f);
    if (word(item3) != before) return 1;
    worldSetDoorHeight__FP12_worldHeaderif(header, 2, 12.75f);
    if (word(item3) != 12) return 2;
    worldSetDoorHeight__FP12_worldHeaderif(header, 2, -3.9f);
    if (word(item3) != -3) return 3;
    // Slot -1 addresses the record 36 bytes before the table start.
    worldSetDoorHeight__FP12_worldHeaderif(header, 0, 100.0f);
    if (word(256 - 36 + 28) != 100) return 4;
    if (word(item3) != -3) return 5;
    return 0;
}
