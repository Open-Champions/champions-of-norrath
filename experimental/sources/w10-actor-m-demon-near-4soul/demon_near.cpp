#include "../w10-actor-m-common/access.hpp"
// Observed demonNear__4Soul at 0x1f4d50, 268 bytes. Returns 1 when a Demon that does not have bit 3 (0x8) set in
// its word at +12 is within a 300.0f box around the object, otherwise 0. The box is the object's position
// (floats at +16, +20, +24) minus and plus 300.0f in every axis, passed to
// objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 128), which fills a list of object pointers
// ended by a null word (the list has 128 slots). The first entry null returns 0. For each listed object the function
// word at byte 4 of its table (word at +72, slot 0) is called; its result is compared with __tf5Demon()
// through __eq__C9type_infoRC9type_info. A nonzero comparison with the bit clear returns 1 at once; otherwise the
// next entry is tried. The image leaves the slot call's argument as stray register contents; the type-info getter
// does not use it and this body passes the object itself. Inferred: bit 3 marks an object that is not active.
// Unknown: what the bit means.
extern "C" {
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** list, int capacity);
void* __tf5Demon();
int __eq__C9type_infoRC9type_info(const void* a, const void* b);
}

extern "C" int demonNear__4Soul(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    void* list[128];
    const float radius = w10m::bits(0x43960000u);
    const float x = w10m::real(o, 16);
    const float y = w10m::real(o, 20);
    const float z = w10m::real(o, 24);
    Point3 low;
    low.x = x - radius;
    low.y = y - radius;
    low.z = z - radius;
    Point3 high;
    high.x = x + radius;
    high.y = y + radius;
    high.z = z + radius;
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 128);
    if (list[0] == nullptr) return 0;
    for (int i = 0; list[i] != nullptr; ++i) {
        unsigned char* other = static_cast<unsigned char*>(list[i]);
        const unsigned type_word = w10m::call_entry_plain(other, 0, self);
        const void* type = reinterpret_cast<const void*>(static_cast<__UINTPTR_TYPE__>(type_word));
        if (__eq__C9type_infoRC9type_info(type, __tf5Demon()) != 0 && (w10m::word(other, 12) & 8u) == 0u) return 1;
    }
    return 0;
}
