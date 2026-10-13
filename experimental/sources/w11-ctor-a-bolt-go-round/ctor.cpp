// Observed __11BoltGoRoundG6Point3siP15AnimationHeader at 0x1e92e0, 580 bytes. Arguments: the
// object, a position (by value), a short (not read), an int N and an AnimationHeader pointer
// (may be null). Returns nothing useful. Steps, in order:
//  1. The GameObject constructor runs with (object, position, null map, null names).
//  2. The float at +24 is raised by 24.0. Word +72 = _vt$11BoltGoRound, bits 0xb are set in
//     the word at +12, word +80 = frameNm + N, word +84 = 0 and word +152 = -1.
//  3. When the header is non-null, soundPlaySequence(header, object, 1.0) runs and its result
//     replaces the word at +152.
//  4. objectAddToRunList(object); halfword +76 = 0.
//  5. For i from 0 while i < nmPlayersAlive (re-read after each lightning call), the player
//     record is playersAlive[i] * 23984 bytes into `player`; the object pointer at record+21872
//     gives a position at +16. If the squared distance from the three floats at object+16 is
//     below 250000.0 (bits 0x48742400): that position is copied to object+104+12*count, where
//     count is the halfword at +76; P_StartLightning__FG6Point3T0iiffUi(copy of object+16, copy of
//     the position stored at object+104+12*i, 200, 10, 8.0, 16.5, 0x5e808080) runs and its result
//     goes to the word at object+88+4*count; count is incremented.
// Inferred: this is a lightning effect that arcs to players within 500 units, because of the
// call's name and the distance test. Unknown: the roles of N and +80, of the lightning
// arguments, and the storage limit of the arrays at +88 and +104 (the loop is bounded by
// nmPlayersAlive). Nothing here ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_BoltGoRound[] __asm__("_vt$11BoltGoRound");
void __11BoltGoRoundG6Point3siP15AnimationHeader(void* self, Point3 position, short unused, int n, void* header);
}

extern "C" void __11BoltGoRoundG6Point3siP15AnimationHeader(void* self, Point3 position, short unused, int n, void* header) {
    using namespace w11;
    (void)unused;
    game_object_construct(self, position, nullptr, nullptr);
    const float raised = f32(self, 24) + 24.0f;
    u32(self, 12) |= 0xbu;
    u32(self, 72) = word(vt_BoltGoRound);
    i32(self, 80) = frameNm + n;
    f32(self, 24) = raised;
    i32(self, 152) = -1;
    u32(self, 84) = 0;
    if (header) i32(self, 152) = soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, self, 1.0f);
    objectAddToRunList__FP10GameObject(self);
    i16(self, 76) = 0;
    for (int i = 0; i < nmPlayersAlive; ++i) {
        unsigned char* const record = player + playersAlive[i] * 23984;
        unsigned char* const other = static_cast<unsigned char*>(pointer(*reinterpret_cast<unsigned*>(record + 21872)));
        const float dx = f32(other, 16) - f32(self, 16);
        const float dy = f32(other, 20) - f32(self, 20);
        const float dz = f32(other, 24) - f32(self, 24);
        if (dx * dx + dy * dy + dz * dz < __builtin_bit_cast(float, 0x48742400u)) {
            const int count = i16(self, 76);
            unsigned char* const slot = static_cast<unsigned char*>(self) + 104 + count * 12;
            f32(slot, 0) = f32(other, 16);
            f32(slot, 4) = f32(other, 20);
            f32(slot, 8) = f32(other, 24);
            const unsigned char* const target = static_cast<unsigned char*>(self) + 104 + i * 12;
            const int handle = P_StartLightning__FG6Point3T0iiffUi(
                Point3{f32(self, 16), f32(self, 20), f32(self, 24)},
                Point3{*reinterpret_cast<const float*>(target), *reinterpret_cast<const float*>(target + 4), *reinterpret_cast<const float*>(target + 8)},
                200, 10, 8.0f, 16.5f, 0x5e808080u);
            i32(self, 88 + i16(self, 76) * 4) = handle;
            i16(self, 76) = static_cast<short>(i16(self, 76) + 1);
        }
    }
}
