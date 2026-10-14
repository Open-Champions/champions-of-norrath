// Observed _GLOBAL_$I$shopActiveFrame (0x001bc5e0) and _GLOBAL_$D$shopActiveFrame
// (0x001bc600), 32 bytes each, pinned SLUS_205.65, and the unit's local routine at
// 0x001bbbf0 (368 bytes) that both call with the second argument 0xffff
// (first argument 1 or 0).
// Observed, priority 0xffff and phase 1, in order:
//  - __11ParticleDefiiiiiiiifffiiiPFPv_v runs on the 80-byte object at
//    0x007cca88 with ints 128, 96, 26, 152, 119, 55, 1, 47, the float words
//    0x40266666, 0x3ef0a3d7 and 0x3e0f5c28, then ints 1, 1, 4 and a null function
//    pointer;
//  - byte 5 of the 264-byte object at 0x007ccad8 is set to 8;
//  - two 4616-byte blocks at 0x007ccbe0 are visited; in each, 64 72-byte
//    records receive: bytes 32 and 33 = 0xff, halfwords at bytes 16..30 = 0 and
//    halfwords at bytes 36, 44, 52 and 60 = 0xffff (the same stores as the
//    item description initializer; the other bytes, including each block's
//    count word at +4608, are not written).
// With phase 0, _$_14AnimationState(0x007ccad8, 0) runs.
// Inferred: the objects are the unit's candleParticle, a single static
// AnimationState (local ELF name animState) and shopList; the block shape
// matches the 64-slot arrays used by the inventory code. The constructor
// argument meanings are not established. Not executed on hardware.
extern "C" {
unsigned char candleParticle[80];
unsigned char norrath_shop_anim_state_view[264];
unsigned char shopList[2 * 4616];
void _$_14AnimationState(void* object, int flags);
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* object, int a, int b, int c, int d, int e, int f, int g, int h,
                                         float x, float y, float z, int i, int j, int k, void (*callback)(void*));
}

namespace {
inline float bits(unsigned value) { union { unsigned u; float f; } c; c.u = value; return c.f; }
inline void initDescription(unsigned char* d) {
    for (int offset = 36; offset <= 60; offset += 8) {
        *reinterpret_cast<short*>(d + offset) = -1;
    }
    d[33] = 0xff;
    d[32] = 0xff;
    for (int offset = 16; offset < 32; offset += 2) *reinterpret_cast<short*>(d + offset) = 0;
}
}

__attribute__((noinline)) static void staticInitShop(int phase, int priority) {
    if (priority == 0xffff && phase == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(candleParticle, 128, 96, 26, 152, 119, 55, 1, 47,
                                            bits(0x40266666), bits(0x3ef0a3d7), bits(0x3e0f5c28), 1, phase, 4, nullptr);
        norrath_shop_anim_state_view[5] = 8;
        for (int block = 0; block < 2; ++block)
            for (int slot = 0; slot < 64; ++slot) initDescription(shopList + block * 4616 + slot * 72);
    }
    if (priority == 0xffff && phase == 0) _$_14AnimationState(norrath_shop_anim_state_view, 0);
}

extern "C" void _GLOBAL_$I$shopActiveFrame() { staticInitShop(1, 0xffff); }
extern "C" void _GLOBAL_$D$shopActiveFrame() { staticInitShop(0, 0xffff); }
