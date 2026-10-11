// Observed setPassableState__8GothTrapb at 0x225cc0, extent 300.
// The bool argument is not read. Every call reloads `world` (0x4844a0) and the
// door index at object+396. worldSetDoorCollision always receives flag 1.
// Word 376 selects the rest: 0 moves the door to the zero point and sets the
// height from object+400; 1 moves to {0, 0, -350} and subtracts 350 from that
// height; 2 moves to the same point and adds 256. The zero arm reloads word
// 376 before the 1-or-2 test. Other values stop after the collision call.
// Low V0 is the last callee's low V0. Door helpers stay imports.
extern "C" int worldSetDoorCollision__FP12_worldHeaderii(void* world, int index, int flag);
extern "C" int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* world, int index, const void* point, short extra);
extern "C" int worldSetDoorHeight__FP12_worldHeaderif(void* world, int index, float height);
extern "C" unsigned char norrath_actors_b_view_4844a0[];

namespace {
using Address = __UINTPTR_TYPE__;
Address address(const void* p) { return reinterpret_cast<Address>(p); }
unsigned char* bytes(Address a) { return reinterpret_cast<unsigned char*>(a); }
void* pointer(Address a) { return reinterpret_cast<void*>(a); }
unsigned read_word(Address a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
float bits_to_float(unsigned bits) {
    float value;
    unsigned char* p = reinterpret_cast<unsigned char*>(&value);
    p[0] = static_cast<unsigned char>(bits);
    p[1] = static_cast<unsigned char>(bits >> 8);
    p[2] = static_cast<unsigned char>(bits >> 16);
    p[3] = static_cast<unsigned char>(bits >> 24);
    return value;
}
float read_float(Address a) { return bits_to_float(read_word(a)); }
int signed_word(unsigned value) {
    return value <= 0x7fffffffu ? static_cast<int>(value) : -1 - static_cast<int>(0xffffffffu - value);
}
void* world() { return pointer(read_word(address(norrath_actors_b_view_4844a0))); }
int door_index(Address object) { return signed_word(read_word(object + 396u)); }
}

extern "C" unsigned norrath_goth_trap_set_passable_state(void* object, int enabled) {
    (void)enabled;
    const Address a = address(object);
    int result = worldSetDoorCollision__FP12_worldHeaderii(world(), door_index(a), 1);
    unsigned state = read_word(a + 376u);
    if (state == 0u) {
        const float point[3] = {0.0f, 0.0f, 0.0f};
        result = worldMoveDoor__FP12_worldHeaderiG6Point3s(world(), door_index(a), point, 0);
        result = worldSetDoorHeight__FP12_worldHeaderif(world(), door_index(a), read_float(a + 400u));
        state = read_word(a + 376u);
    }
    if (state == 1u) {
        const float point[3] = {0.0f, 0.0f, bits_to_float(0xc3af0000u)};
        result = worldMoveDoor__FP12_worldHeaderiG6Point3s(world(), door_index(a), point, 0);
        const float height = read_float(a + 400u) - bits_to_float(0x43af0000u);
        result = worldSetDoorHeight__FP12_worldHeaderif(world(), door_index(a), height);
    } else if (state == 2u) {
        const float point[3] = {0.0f, 0.0f, bits_to_float(0xc3af0000u)};
        result = worldMoveDoor__FP12_worldHeaderiG6Point3s(world(), door_index(a), point, 0);
        const float height = read_float(a + 400u) + bits_to_float(0x43800000u);
        result = worldSetDoorHeight__FP12_worldHeaderif(world(), door_index(a), height);
    }
    return static_cast<unsigned>(result);
}
