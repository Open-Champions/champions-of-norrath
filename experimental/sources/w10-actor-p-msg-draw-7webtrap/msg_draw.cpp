// Observed msg_draw__7WebTrap at 0x0020e8e8, 744 bytes. Draws two web splays and then two
// face-sprites. Names for the object fields are not established; offsets are used.
//  Points: P = (+16), Q = (+84), A = (+108), B = (+120). Let H be the word at +136 and K the word at
//  offset 12 of the block H points to (K is used as an object: its flag word at +12 and its table
//  at +72 are read).
//  - Attached form, when H is nonzero and bit 5 (0x20) of the word at K+12 is set: for each of the
//    two splays the table entry at byte offset 128 of K's table is called on K (adjusted by the
//    entry's halfword) with no extra argument; its result is the model header. Then
//    DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState is called with
//    (P, signed byte +132, A, 0, word +152, 10, float +156, K, model, K+288) for the first splay and
//    with (Q, signed byte +132, B, 0, word +152, 10, float +156, K, model, K+288) for the second.
//    K is read again from H before each splay, as is the table call.
//  - Otherwise the same two calls are made with (P, ..., A, ...) and (Q, ..., B, ...) and the last
//    three arguments (object, model, animation state) all null.
//  Then, in both forms, P_FaceSprite__FG6Point3ffiiiiP8_texturesi is called twice with a copy of
//  P and then of Q, floats 0.0 and 9000.0, ints 255, 255, 255, the word at +144, the word at +140
//  as the texture, a short and 1. The short is the low half of the object's own address for P and
//  that address plus 16384 for Q.
// Unknown: what the fields are (the +140 word is used as a texture pointer, the +144 word as an
// integer); why the sprite shorts derive from the address; what table entry 128 is (it returns
// the value used as a model header); the 9000.0 is read as a literal. Nothing here ran on the EE.
#include "../w10-actor-p-common/access.hpp"

extern "C" {
void DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState(
    Point3 from, int a, Point3 to, int b, int c, int d, float e, void* object, void* model, void* animation);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 position, float first, float second, int red, int green, int blue, int alpha,
                                              void* texture, short tag, int flag);
}

extern "C" void msg_draw__7WebTrap(void* self) {
    using namespace w10p;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const Point3& p = *reinterpret_cast<const Point3*>(o + 16);
    const Point3& q = *reinterpret_cast<const Point3*>(o + 84);
    const Point3& a = *reinterpret_cast<const Point3*>(o + 108);
    const Point3& b = *reinterpret_cast<const Point3*>(o + 120);
    const unsigned held = word(o, 136);
    if (held != 0u && (word(from_word(word(from_word(held), 12)), 12) & 0x20u) != 0u) {
        unsigned char* k = from_word(word(from_word(word(o, 136)), 12));
        void* model = from_word(call_entry(k, 128));
        k = from_word(word(from_word(word(o, 136)), 12));
        DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState(
            p, static_cast<signed char>(o[132]), a, 0, sword(o, 152), 10, real(o, 156), k, model, k + 288);
        k = from_word(word(from_word(word(o, 136)), 12));
        model = from_word(call_entry(k, 128));
        k = from_word(word(from_word(word(o, 136)), 12));
        DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState(
            q, static_cast<signed char>(o[132]), b, 0, sword(o, 152), 10, real(o, 156), k, model, k + 288);
    } else {
        DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState(
            p, static_cast<signed char>(o[132]), a, 0, sword(o, 152), 10, real(o, 156), nullptr, nullptr, nullptr);
        DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState(
            q, static_cast<signed char>(o[132]), b, 0, sword(o, 152), 10, real(o, 156), nullptr, nullptr, nullptr);
    }
    P_FaceSprite__FG6Point3ffiiiiP8_texturesi(p, 0.0f, 9000.0f, 255, 255, 255, sword(o, 144), from_word(word(o, 140)),
                                              static_cast<short>(to_word(self)), 1);
    P_FaceSprite__FG6Point3ffiiiiP8_texturesi(q, 0.0f, 9000.0f, 255, 255, 255, sword(o, 144), from_word(word(o, 140)),
                                              static_cast<short>(to_word(self) + 16384u), 1);
}
