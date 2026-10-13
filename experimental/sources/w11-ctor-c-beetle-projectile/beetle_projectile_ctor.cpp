// Observed __16beetleProjectileG6Point3T1iP8CreatureiiiiiP15AnimationHeader at 0x00212520,
// 244 bytes. Arguments (object first): positions P and Q, an int N, a Creature pointer C, five
// ints I1..I5 and an AnimationHeader pointer H (I4, I5 and H arrive on the stack). In the
// order of the image:
//  - the MissileWeapon constructor runs with: int 0, null model header, null texture, copies of
//    P and Q, N, C, two null sound-header words, I1, I2, I3, I4, I5, the int 4 and a null item
//    description pointer;
//  - the table pointer (_vt$16beetleProjectile) goes to +72 and the word at +452 is cleared;
//  - when C is not null, listOnewayLink__FP8ListHeadT0Pv(object + 452, C, C) runs;
//  - H goes to +456.
// Nothing is returned. Inferred: +452 is the head of a one-way list that C joins (it is the head
// argument of the link call), and the base is a missile/projectile weapon object.
// Unknown: the meaning of N and of I1..I5, and what H is used for later. Nothing here ran on
// the EE.
struct Point3 { float x, y, z; };

extern "C" {
unsigned __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
    void* self, int zero, void* modelHeader, void* texture, Point3 p, Point3 q, int n, void* creature,
    void* vagA, void* vagB, int i1, int i2, int i3, int i4, int i5, int four, void* description);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
extern unsigned char vt_beetleProjectile[] __asm__("_vt$16beetleProjectile");
}

extern "C" void __16beetleProjectileG6Point3T1iP8CreatureiiiiiP15AnimationHeader(
    void* self, Point3 p, Point3 q, int n, void* creature, int i1, int i2, int i3, int i4, int i5, void* header) {
    unsigned char* object = static_cast<unsigned char*>(self);
    __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
        self, 0, nullptr, nullptr, p, q, n, creature, nullptr, nullptr, i1, i2, i3, i4, i5, 4, nullptr);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_beetleProjectile));
    *reinterpret_cast<unsigned*>(object + 452) = 0u;
    if (creature) listOnewayLink__FP8ListHeadT0Pv(object + 452, creature, creature);
    *reinterpret_cast<unsigned*>(object + 456) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(header));
}
