// Observed msg_load__10DoorSecretPc at 0x21c068, 160 bytes. Reads one byte from the
// stream (signed) and stores it at object+88 (the byte msg_save__10DoorSecretPc
// writes). If it is nonzero, the door is made non-colliding and moved: it calls
// worldSetDoorCollision(world, door, 0) with the signed halfword at object+76 as the
// door index, then worldMoveDoor(world, door, point, 0) with the point (0, 0, -300.0)
// (the float word 0xc3960000), and clears bit 20 (0x100000) of the flag word at
// object+12. Returns 1, the bytes consumed.
// Unknown: the extra short argument of worldMoveDoor is passed as 0 here; what the
// callee does with it, and what the halfword at +76 names beyond "door index".

extern "C" {
struct Point3 { float x, y, z; };
void worldSetDoorCollision__FP12_worldHeaderii(void* world, int index, int enable);
int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* world, int index, Point3 point, short extra);
extern int world;
}

typedef unsigned Word __attribute__((may_alias));
typedef short Half __attribute__((may_alias));

// The world header pointer is read again at each call.
static inline void* world_header() { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(world)); }

extern "C" int msg_load__10DoorSecretPc(void* self, const char* in) {
    unsigned char* object = static_cast<unsigned char*>(self);
    signed char value = static_cast<signed char>(in[0]);
    object[88] = static_cast<unsigned char>(value);
    if (value != 0) {
        worldSetDoorCollision__FP12_worldHeaderii(world_header(), *reinterpret_cast<Half*>(object + 76), 0);
        Point3 point = {0.0f, 0.0f, -300.0f};
        worldMoveDoor__FP12_worldHeaderiG6Point3s(world_header(), *reinterpret_cast<Half*>(object + 76), point, 0);
        *reinterpret_cast<Word*>(object + 12) &= 0xffefffffu;
    }
    return 1;
}
