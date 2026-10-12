// Observed _GLOBAL_$I$inventoryLoad__Fv (0x001b1fe8) and
// _GLOBAL_$D$inventoryLoad__Fv (0x001b2008), 32 bytes each, pinned
// SLUS_205.65, and the unit's local routine at 0x001b1528 (368 bytes) that both
// call with the second argument 0xffff (first argument 1 or 0).
// Observed, priority 0xffff and phase 1, in order:
//  - byte 5 of each of four 264-byte records at 0x007cc5c8 is set to 8;
//  - the constructor __11ParticleDefiiiiiiiifffiiiPFPv_v runs on the 80-byte
//    object at 0x007cc9e8 with ints 238, 210, 100, 180, 105, 63, 4, 0, floats
//    30.0f, 5.0f and the float word 0x3f733333, then ints 2, 1, 0 and a null
//    function pointer (the stack slots are eight bytes apart);
//  - the same constructor on the object at 0x007cca38 with ints 5, 128, 128, 402,
//    128, 128, 128, -68, floats 38.0f, 34.0f, 0.0f, then ints 2, 3, 0 and null.
// With phase 0 the four 264-byte records are destroyed in reverse order by
// _$_14AnimationState(record, 0).
// Inferred: the 80-byte objects are the unit's static particle definitions (ELF
// names torchParticle and heatParticle) and the 264-byte records its static
// AnimationState array (local ELF name animState). The constructor argument
// meanings are not established; the value 1 in the tenth integer slot is the
// phase value the compiler reused. Not executed on hardware.
extern "C" {
unsigned char norrath_inventory_anim_state_view[1056];
unsigned char torchParticle[80];
unsigned char heatParticle[80];
void _$_14AnimationState(void* object, int flags);
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* object, int a, int b, int c, int d, int e, int f, int g, int h,
                                         float x, float y, float z, int i, int j, int k, void (*callback)(void*));
}

namespace {
inline float bits(unsigned value) { union { unsigned u; float f; } c; c.u = value; return c.f; }
}

__attribute__((noinline)) static void staticInitInventory(int phase, int priority) {
    if (priority == 0xffff && phase == 1) {
        for (int i = 0; i < 4; ++i) norrath_inventory_anim_state_view[i * 264 + 5] = 8;
    }
    if (priority == 0xffff && phase == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(torchParticle, 238, 210, 100, 180, 105, 63, 4, 0,
                                            30.0f, 5.0f, bits(0x3f733333), 2, phase, 0, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(heatParticle, 5, 128, 128, 402, 128, 128, 128, -68,
                                            38.0f, 34.0f, 0.0f, 2, 3, 0, nullptr);
    }
    if (priority == 0xffff && phase == 0) {
        for (int i = 3; i >= 0; --i) _$_14AnimationState(norrath_inventory_anim_state_view + i * 264, 0);
    }
}

extern "C" void _GLOBAL_$I$inventoryLoad__Fv() { staticInitInventory(1, 0xffff); }
extern "C" void _GLOBAL_$D$inventoryLoad__Fv() { staticInitInventory(0, 0xffff); }
