// Observed worldDeactivateUntakenDoors__FP12_worldHeader at 0x0017d298, 124 bytes. The world
// header holds a door count at +80 and a door array pointer at +84; records are 28 bytes.
// For each door index in order, when bit 0 of the halfword at record+6 is clear it calls
// worldSetDoorCollision__FP12_worldHeaderii(world, index, 0). The count is reloaded from the
// header after each call and the array pointer is reloaded each pass, so a callback that
// changes either is honoured. Returns nothing.
// Inferred: bit 0 of that halfword marks a door as taken. Unknown: the rest of the record.
struct _worldHeader;
extern "C" void worldSetDoorCollision__FP12_worldHeaderii(_worldHeader*, int, int);

extern "C" void worldDeactivateUntakenDoors__FP12_worldHeader(_worldHeader* world) {
    unsigned char* header = reinterpret_cast<unsigned char*>(world);
    int count = *reinterpret_cast<int*>(header + 80);
    for (int i = 0; i < count; ++i) {
        unsigned char* door = *reinterpret_cast<unsigned char**>(header + 84) + i * 28;
        if (*reinterpret_cast<unsigned short*>(door + 6) & 1) continue;
        worldSetDoorCollision__FP12_worldHeaderii(world, i, 0);
        count = *reinterpret_cast<int*>(header + 80);
    }
}
