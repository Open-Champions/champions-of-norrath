#include "../w11-actor-c-common/c.hpp"
// Observed msg_collision__6SparksP10GameObject at 0x00217028, 544 bytes. Arguments: the sparks object s and
// another object o. Let t be the word at (word at s+76) + 12 (zero when the word at s+76 is zero). Nothing
// happens when the word at s+84 is zero, when t is zero, or when the virtual slot at byte 160 of t's table
// (called with t adjusted and no other argument) returns zero. Otherwise:
//   1. A 188-byte block is allocated; if that succeeds a LightEffect is constructed in it at the three floats
//      at s+16 with the floats 255, 200, 100, -32, 0, 0, 0, 0, 180, 120, 1.0, 1.0, no object and 0.
//   2. The sound sequence "spel_ew_shockfire_hit.anm" from the lump "fx.lmp" is played on s at gain 1.0.
//   3. An item description is built on the stack: the bytes at 32 and 33 are -1, the eight halfwords at 16..31 are 0
//      and the four 8-byte entries at 36 each get -1 in their first halfword. The first entry whose first
//      halfword is -1 or 31 gets 31, 100 at +2, the low halfword of the word at s+172 at +4 and the low halfword
//      of the word at s+168 at +6.
//   4. gameAttackResolver(o, t, 0.0, 0.0, 10000000, word(s+172), word(s+168), 0xc060, description, false) runs,
//      then the word at s+84 is cleared.
// Inferred: this is the sparks' hit: once, on the first object it touches, it flashes a light, plays the shock-fire hit
// sound and applies a fixed attack of shock type whose two values come from the sparks themselves. The
// meaning of 0xc060 and the description fields is not established. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
void* blockAlloc__Fi(int size);
void lightEffect_construct(void* self, const Point3* position, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7,
                           float f8, float f9, float f10, float f11, void* object, int flags)
    __asm__("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void* lumpFindResource__FPcT0(const char* lump, const char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* animation, void* object, float gain);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);

void msg_collision__6SparksP10GameObject(void* self, void* other) {
    using namespace w11c;
    unsigned char* target = nullptr;
    if (word(self, 76) != 0u) target = link(link(self, 76), 12);
    if (word(self, 84) == 0u) return;
    if (target == nullptr) return;
    if (call_slot(target, 160) == nullptr) return;
    void* block = blockAlloc__Fi(188);
    if (block != nullptr) {
        const Point3 position = *reinterpret_cast<const Point3*>(bytes(self) + 16);
        lightEffect_construct(block, &position, 255.0f, 200.0f, 100.0f, -32.0f, 0.0f, 0.0f, 0.0f, 0.0f, 180.0f, 120.0f, 1.0f, 1.0f, nullptr, 0);
    }
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", "spel_ew_shockfire_hit.anm"), self, 1.0f);
    unsigned char description[80];
    for (int entry = 3; entry >= 0; --entry) shalf(description, 36 + 8 * entry) = -1;
    sbyte(description, 33) = -1;
    sbyte(description, 32) = -1;
    for (int i = 0; i < 8; ++i) shalf(description, 16 + 2 * i) = 0;
    const unsigned word172 = word(self, 172);
    const unsigned word168 = word(self, 168);
    for (int entry = 0; entry < 4; ++entry) {
        unsigned char* e = description + 36 + 8 * entry;
        if (shalf(e, 0) == -1 || shalf(e, 0) == 31) {
            shalf(e, 0) = 31;
            shalf(e, 2) = 100;
            shalf(e, 4) = static_cast<short>(word172);
            shalf(e, 6) = static_cast<short>(word168);
            break;
        }
    }
    gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(other, target, 0.0f, 0.0f, 10000000, static_cast<int>(word172),
                                                                    static_cast<int>(word168), 0xc060, description, false);
    word(self, 84) = 0u;
}
}
