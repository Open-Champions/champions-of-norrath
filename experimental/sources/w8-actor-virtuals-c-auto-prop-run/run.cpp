#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed msg_run__8AutoProp at 0x2207a8, 652 bytes.
// objectActiveProcessing(object, word at +280, byte at +40 plus 480) decides
// whether the object is active; its truth value is stored back at +280 and a
// false result ends the call. Active objects then:
//  - store at +192 the result of animAdvanceAnimation(object+152, object+16, 0, 0)
//    when the signed byte at +156 is nonzero, else store 0 there;
//  - add one to the word at +380 (before the name test below);
//  - end the call unless strticmp(object+76, "pipepistona") returns zero.
// For a name match, a point is built from the position at +16 moved by
// (48*icos(a), 48*isin(a)) with a the signed halfword at +56. (The image also
// raises a third component by 24.0, but that value is never read, so it is not
// reproduced.) Bit 0x1 of the
// word at +192 sets the word at +292 to 1; otherwise bit 0x2 clears it. When +292
// is zero the call ends. Otherwise every player slot below the word nmPlayers is
// visited: the player record array `player` has 23984-byte records and the object
// pointer sits at +21872 of a record. A null pointer or an object with bit 0x8 of
// its +12 flags is skipped. Within planar distance 18.0 of the point, the other
// object is hit by gameAttackResolver(player object, this object, s, s, 10000, 0,
// 0, 65, 0, 0) with s = gameDifficultyScale() * 0.3; its float at +756 becomes
// 100.0, and its halfword at +748 becomes this object's +56 halfword shifted by
// +16384 or -16384 depending on the sign of the 16-bit difference between
// iatan2(player.y - point.y, player.x - point.x) and that halfword.
// Inferred: the three flag meanings are unknown; the 0.3 is the first word of a
// small constant table at 0x48170c.
// Unknown: what the name test stands for, units of 48/18/100/16384, and the
// meaning of +192, +280, +292, +380, +748 and +756.
extern "C" int objectActiveProcessing__FP10GameObjectii(void* object, int a, int b);
extern "C" int animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* state, void* point, void* other, short* extra);
extern "C" int strticmp__FPCcT0(const char* left, const char* right);
extern "C" float icos__Fi(int angle);
extern "C" float isin__Fi(int angle);
extern "C" float gameDifficultyScale__Fv();
extern "C" int iatan2__Fff(float y, float x);
extern "C" void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
    void* victim, void* attacker, float a, float b, int c, int d, int e, int f, void* g, int h);
extern "C" unsigned char player[];
extern "C" unsigned char nmPlayers[4];

namespace {
__attribute__((always_inline)) inline float length(float dx, float dy) {
    const float sum = dx * dx + dy * dy;
#if defined(__mips__)
    float result;
    __asm__("sqrt.s %0, %1" : "=f"(result) : "f"(sum));
    return result;
#else
    return __builtin_sqrtf(sum);
#endif
}
}

extern "C" void msg_run__8AutoProp(void* self) {
    using namespace w8c;
    const Address base = address(self);
    const int active = objectActiveProcessing__FP10GameObjectii(
        self, signed_word(read_word(base + 280u)), static_cast<int>(read_u8(base + 40u)) + 480);
    write_word(base + 280u, active != 0 ? 1u : 0u);
    if (active == 0) return;
    unsigned char* const bytes_self = static_cast<unsigned char*>(self);
    if (read_u8(base + 156u) != 0u) {
        const int result = animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(
            bytes_self + 152, bytes_self + 16, nullptr, nullptr);
        write_word(base + 192u, static_cast<unsigned>(result));
    } else {
        write_word(base + 192u, 0u);
    }
    write_word(base + 380u, read_word(base + 380u) + 1u);
    static const char name[] = {'p', 'i', 'p', 'e', 'p', 'i', 's', 't', 'o', 'n', 'a', '\0'};
    if (strticmp__FPCcT0(reinterpret_cast<const char*>(bytes_self + 76), name) != 0) return;
    const int angle = read_i16(base + 56u);
    float point[2] = {read_single(base + 16u), read_single(base + 20u)};
    const float forward = icos__Fi(angle) * 48.0f;
    const float side = isin__Fi(angle) * 48.0f;
    point[0] = point[0] + forward;
    point[1] = point[1] + side;
    const unsigned phase = read_word(base + 192u);
    if ((phase & 1u) != 0u) {
        write_word(base + 292u, 1u);
    } else if ((phase & 2u) != 0u) {
        write_word(base + 292u, 0u);
    }
    if (read_word(base + 292u) == 0u) return;
    for (int i = 0; i < signed_word(read_word(address(nmPlayers))); ++i) {
        const Address slot = address(player) + 21872u + 23984u * static_cast<unsigned>(i);
        void* const other = pointer(read_word(slot));
        if (other == nullptr) continue;
        const Address other_base = address(other);
        if ((read_word(other_base + 12u) & 8u) != 0u) continue;
        if (!(length(point[0] - read_single(other_base + 16u), point[1] - read_single(other_base + 20u)) < 18.0f))
            continue;
        const float scale = gameDifficultyScale__Fv() * single_from_bits(0x3e999999u);
        gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
            pointer(read_word(slot)), self, scale, scale, 10000, 0, 0, 65, nullptr, 0);
        void* const target = pointer(read_word(slot));
        write_single(address(target) + 756u, 100.0f);
        const int heading = iatan2__Fff(read_single(address(target) + 20u) - point[1],
                                        read_single(address(target) + 16u) - point[0]);
        const unsigned facing = read_u16(base + 56u);
        const unsigned difference = (static_cast<unsigned>(heading) - facing) & 0xffffu;
        write_u16(address(pointer(read_word(slot))) + 748u,
                  difference >= 0x8000u ? facing - 16384u : facing + 16384u);
    }
}
