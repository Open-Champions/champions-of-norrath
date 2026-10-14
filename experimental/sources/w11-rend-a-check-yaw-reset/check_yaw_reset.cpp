#include "../w11-rend-a-common/rend.hpp"
// Observed checkYawReset__6Camera at 0x0026ed68, 296 bytes. Camera words used: signed byte +352,
// word +100, word +56 (a rotation direction), word +64 (a player/pad index n), halfword +104, halfword
// +138 and signed byte +353. Globals: levelFlags, the 88-byte records of pad (word +12 and floats +20
// and +24) and the 23984-byte records of player (pointer +21872, whose target holds an unsigned
// halfword at +56).
// Nothing happens when levelFlags has bit 0x02000000, or the byte at +352, the word at +100 or the
// word at +56 is nonzero, or pad[n] word +12 lacks bit 0x200, or the squared length of pad[n]'s
// floats at +20 and +24 is not below 0x1.0624dcp-5 (0.032, truncated; read from the image).
// Otherwise a target yaw is stored at +138: 16384 minus the halfword at +56 of player[n]'s pointer
// when that pointer is nonzero, else -8192. The image also tests the signed byte at +353 when the
// pointer is zero, but both of its paths end in -8192, so it has no effect. Then the word at +56 is
// set to fastestRotDirection__FssPi((short)+104, (short)+138, 0).
// Nothing is returned.
// Inferred: when the stick is centred the camera yaw is re-aimed behind the player and the rotation
// direction for the turn is cached. Unknown: the units of the angles and the roles of the guards.
// Nothing here was executed on the EE.
extern "C" {
extern int levelFlags;
extern unsigned char pad[];
extern unsigned char player[];
int fastestRotDirection__FssPi(short a, short b, int* out);
void checkYawReset__6Camera(unsigned char* camera) {
    using namespace rend;
    if ((levelFlags & 0x2000000) != 0) return;
    if (at<i8>(camera, 352) != 0) return;
    if (at<i32>(camera, 100) != 0) return;
    if (at<i32>(camera, 56) != 0) return;
    const int n = at<i32>(camera, 64);
    unsigned char* p = pad + n * 88;
    if ((at<i32>(p, 12) & 0x200) == 0) return;
    const float a = at<f32>(p, 20);
    const float b = at<f32>(p, 24);
    if (!(a * a + b * b < 0x1.0624dcp-5f)) return;
    unsigned char* target = ptr(player + n * 23984, 21872);
    at<i16>(camera, 138) = target != nullptr ? static_cast<short>(16384 - at<u16>(target, 56)) : -8192;
    at<i32>(camera, 56) = fastestRotDirection__FssPi(at<i16>(camera, 104), at<i16>(camera, 138), 0);
}
}
