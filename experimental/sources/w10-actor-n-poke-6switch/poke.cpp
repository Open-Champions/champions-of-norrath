// Observed msg_poke__6Switchi at 0x0021be80, 108 bytes. Clears bit 20 (0x100000) of the flag
// word at object+12, calls worldMoveDoor(world, door, point, 0) with the signed halfword at
// object+76 as the door index and the point (8, 0, 0), then calls objectDoMyActions(object, 1).
// The integer argument is not read. The world pointer is read at the call. Returns nothing.
// Unknown: what the point means to worldMoveDoor and why the door is moved by 8 units on x.
// Nothing here ran on the EE.
extern "C" {
struct Point3 { float x, y, z; };
int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* world, int index, Point3 point, short extra);
void objectDoMyActions__FP10GameObjecti(void* object, int on);
extern int world;
}

typedef unsigned Word __attribute__((may_alias));
typedef short Half __attribute__((may_alias));

extern "C" void msg_poke__6Switchi(void* self, int) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<Word*>(object + 12) &= 0xffefffffu;
    Point3 offset = {8.0f, 0.0f, 0.0f};
    worldMoveDoor__FP12_worldHeaderiG6Point3s(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(world)), *reinterpret_cast<Half*>(object + 76), offset, 0);
    objectDoMyActions__FP10GameObjecti(self, 1);
}
