// Observed __10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap at 0x00153fd0,
// 156 bytes. Arguments: the object, a three-float position (by value), an owner object (may be
// null) and a resource map. Steps, in order:
//  1. The GameObject constructor __10GameObjectG6Point3PQ210GameObject11resourceMapPPc runs on
//     the object with the position, the resource map and a null fourth argument.
//  2. The float at object+28 is set to 12.0, the word at +72 to the Projectile table
//     (_vt$10Projectile), the word at +12 gets the bits 0x4081 set, the word at +76 is cleared
//     and the byte at +80 is cleared.
//  3. When the owner is non-null, listOnewayLink__FP8ListHeadT0Pv is called with (object+76,
//     owner, owner).
// The object pointer is not returned in a register by this routine. Nothing here ran on the EE;
// what the fields mean is unknown.
struct Point3 { float x, y, z; };

extern "C" {
void game_object_construct(void* self, Point3 position, void* map, char** names)
    __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* other, void* item);
extern unsigned char vt_Projectile[] __asm__("_vt$10Projectile");
void construct(void* self, Point3 position, void* owner, void* map)
    __asm__("__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap");
}

extern "C" void construct(void* self, Point3 position, void* owner, void* map) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    game_object_construct(self, position, map, nullptr);
    *reinterpret_cast<float*>(o + 28) = 12.0f;
    *reinterpret_cast<unsigned*>(o + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Projectile));
    *reinterpret_cast<unsigned*>(o + 12) |= 0x4081u;
    *reinterpret_cast<unsigned*>(o + 76) = 0;
    o[80] = 0;
    if (owner) listOnewayLink__FP8ListHeadT0Pv(o + 76, owner, owner);
}
