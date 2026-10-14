// Observed msg_load__9DoorSwingPc at 0x212d08, 216 bytes. Reads from the stream a
// halfword stored at object+80, a halfword whose low byte is stored at object+88, and a
// float stored at object+84 (the three fields msg_save__9DoorSwingPc writes, in
// that order). If the first halfword is nonzero, it clears bit 20 (0x100000) of the flag
// word at object+12, calls worldSetDoorCollision(world, door, 0) with the signed
// halfword at object+76 as the door index, sets bit 0 of the flag word and adds the
// object to the run list. In every case it then calls worldMoveDoor(world, door,
// point, step) with the point (0, 0, 0) and step the low 16 bits of the unsigned
// halfword at +80 times the signed byte at +88. Returns 8, the bytes consumed.
// Unknown: what the three saved fields mean beyond that, and what worldMoveDoor does
// with its extra short argument.

extern "C" {
struct Point3 { float x, y, z; };
void worldSetDoorCollision__FP12_worldHeaderii(void* world, int index, int enable);
int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* world, int index, Point3 point, short extra);
void objectAddToRunList__FP10GameObject(void* object);
extern int world;
}

typedef unsigned Word __attribute__((may_alias));
typedef short Half __attribute__((may_alias));
typedef float Single __attribute__((may_alias));

// The world header pointer is read again at each call.
static inline void* world_header() { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(world)); }

extern "C" int msg_load__9DoorSwingPc(void* self, const char* in) {
    unsigned char* object = static_cast<unsigned char*>(self);
    short moved = *reinterpret_cast<const Half*>(in);
    *reinterpret_cast<Half*>(object + 80) = moved;
    short second = *reinterpret_cast<const Half*>(in + 2);
    object[88] = static_cast<unsigned char>(second);
    *reinterpret_cast<Single*>(object + 84) = *reinterpret_cast<const Single*>(in + 4);
    if (moved != 0) {
        *reinterpret_cast<Word*>(object + 12) &= 0xffefffffu;
        worldSetDoorCollision__FP12_worldHeaderii(world_header(), *reinterpret_cast<Half*>(object + 76), 0);
        *reinterpret_cast<Word*>(object + 12) |= 1;
        objectAddToRunList__FP10GameObject(self);
    }
    Point3 origin = {0.0f, 0.0f, 0.0f};
    int step = static_cast<int>(*reinterpret_cast<unsigned short*>(object + 80)) * static_cast<signed char>(object[88]);
    worldMoveDoor__FP12_worldHeaderiG6Point3s(world_header(), *reinterpret_cast<Half*>(object + 76), origin, static_cast<short>(step));
    return 8;
}
