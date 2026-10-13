#include "../level-anim-load-common/mem.hpp"
// Observed animGetBoneMatrix__FP14AnimationStateiP8Matrix34 at 0x001627a0, 320 bytes.
// When the word at state+0 (the evaluated result record) is zero the routine first calls
// evaluateAnimation__FP14AnimationStateb(state, 0) and rereads it; if it is still zero the
// record at identityResult (0x006da1c0) is used instead. The signed halfword at record+8 is a
// bone count; the bone index is clamped to 0 first and then to count-1 (signed compares).
// Bone records are 24 bytes from record+0, each holding three groups of four signed
// halfwords at +16, +24, +32. For group j (0..2) and element e (0..2) the float at
// out[e*3 + j] is the halfword at +16+8*j+2*e times 1/4096; the halfwords at +22, +30, +38
// give out[9], out[10], out[11] as halfword times 1/64. Rotation floats are stored first.
// Nothing is returned.
// Inferred: the 3x3 part is a fixed-point 4.12 rotation and the last three values a
// translation in 1/64 units, written as a 12-float Matrix34. Unknown: the record bytes
// +0..+15 of each bone and the meaning of the identityResult contents.
extern "C" {
extern unsigned char identityResult[];
void evaluateAnimation__FP14AnimationStateb(unsigned char* state, bool flag);
void animGetBoneMatrix__FP14AnimationStateiP8Matrix34(unsigned char* state, int bone, float* out) {
    using namespace norrath_lal;
    if (load_word(state) == 0) evaluateAnimation__FP14AnimationStateb(state, false);
    unsigned char* result = load_pointer(state);
    if (result == nullptr) result = identityResult;
    const int last = load_signed_half(result + 8) - 1;
    if (bone < 0) bone = 0;
    if (bone > last) bone = last;
    const unsigned char* entry = result + bone * 24;
    for (int e = 0; e != 3; ++e)
        for (int j = 0; j != 3; ++j)
            out[e * 3 + j] = static_cast<float>(load_signed_half(entry + 16 + 8 * j + 2 * e)) * (1.0f / 4096.0f);
    out[9] = static_cast<float>(load_signed_half(entry + 22)) * (1.0f / 64.0f);
    out[10] = static_cast<float>(load_signed_half(entry + 30)) * (1.0f / 64.0f);
    out[11] = static_cast<float>(load_signed_half(entry + 38)) * (1.0f / 64.0f);
}
}
