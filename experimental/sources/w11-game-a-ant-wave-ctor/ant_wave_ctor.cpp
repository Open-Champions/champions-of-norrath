// Observed __7AntWaveP10GameObjectG6Point3si at 0x00208be8, 228 bytes: the AntWave constructor.
// Arguments: the object memory (this), an owner object, a position (Point3 by reference; copied),
// a yaw (its low 16 bits, sign-extended again here as the image does with a shift pair) and an
// integer delay. In the image's order:
//   1. The Creature constructor __8CreatureG6Point3sPQ210GameObject11resourceMapfPPc runs on this
//      with the copied position, the yaw, a null resource map, the float 10000.0 and a null name list.
//   2. this+168 = 0.6 (bits 0x3f199999), this+176 = 100.0, this+28 = 5.0; this+72 receives the
//      AntWave table pointer (_vt$7AntWave).
//   3. this+960 = the word at this+808, this+964 = that word + delay.
//   4. this+28 = the float at owner+28 plus 10.0 (overwriting the 5.0 above).
//   5. The ListHead at this+968 is set to zero, then listOnewayLink__FP8ListHeadT0Pv(head, owner,
//      owner) links the owner into it; the flag word at this+12 gets bit 0x1 (stored in the delay
//      slot of that call, so before the link routine runs).
//   6. objectAddToRunList__FP10GameObject(this).
// Nothing is returned. Inferred: +960/+964 are a start and end frame (the word at +808 is read as
// the current time by the sibling classes; the AntQueen caller passes 600), and +28 is a height or
// radius sitting just above the owner's. Unknown: the meaning of +168/+176 and of the list.
// Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_AntWave[] __asm__("_vt$7AntWave");
void Creature_ctor(void* self, const float* position, short yaw, void* resource_map, float scale, char** names)
    __asm__("__8CreatureG6Point3sPQ210GameObject11resourceMapfPPc");
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
void objectAddToRunList__FP10GameObject(void* object);
}

namespace {
inline float constant(unsigned bits) { return __builtin_bit_cast(float, bits); }
inline unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }
}  // namespace

extern "C" void AntWave_ctor(void* object, void* owner_object, const float* position, int yaw, int delay)
    __asm__("__7AntWaveP10GameObjectG6Point3si");

extern "C" void AntWave_ctor(void* object, void* owner_object, const float* position, int yaw, int delay) {
    unsigned char* self = static_cast<unsigned char*>(object);
    unsigned char* owner = static_cast<unsigned char*>(owner_object);
    float where[3] = {position[0], position[1], position[2]};
    Creature_ctor(self, where, static_cast<short>(yaw), nullptr, constant(0x461c4000u), nullptr);
    *reinterpret_cast<float*>(self + 168) = constant(0x3f199999u);
    *reinterpret_cast<float*>(self + 176) = constant(0x42c80000u);
    *reinterpret_cast<float*>(self + 28) = constant(0x40a00000u);
    unsigned time = *reinterpret_cast<unsigned*>(self + 808);
    unsigned flags = *reinterpret_cast<unsigned*>(self + 12);
    *reinterpret_cast<unsigned*>(self + 72) = low32(vt_AntWave);
    *reinterpret_cast<unsigned*>(self + 960) = time;
    *reinterpret_cast<unsigned*>(self + 964) = time + static_cast<unsigned>(delay);
    *reinterpret_cast<float*>(self + 28) = *reinterpret_cast<float*>(owner + 28) + constant(0x41200000u);
    *reinterpret_cast<unsigned*>(self + 968) = 0;
    *reinterpret_cast<unsigned*>(self + 12) = flags | 1;
    listOnewayLink__FP8ListHeadT0Pv(self + 968, owner, owner);
    objectAddToRunList__FP10GameObject(self);
}
