// Observed msg_run__11LightEffect at 0x001d45a0, 488 bytes, taking the object. Field offsets are decimal
// byte offsets in the object; floats are 32-bit.
// 1. Five floats are advanced by a per-step float: the colour-like quad at +140, +144, +148, +152 gains the
//    quad at +156, +160, +164, +168 (componentwise), and the float at +172 gains the float at +176. The
//    sums are stored back.
// 2. The first three sums at +140, +144, +148 are clamped and stored again: to the range -255.0 to 255.0
//    when the word at +184 is nonzero, to 0.0 to 255.0 when it is zero. The +152 sum is not clamped.
// 3. The embedded light record at +76 is filled from those values: the four halfwords at +94, +96, +98,
//    +100 receive the truncated integers of the floats at +140, +144, +148, +152 (the +152 one unclamped),
//    and +86, +88, +90, +92 receive the same four values again. The float at +172 goes to +104 and to
//    +112 of the object, and 1.0f / (that float squared) goes to +108 and +116.
// 4. The pointer at +136 may name an owner. If it is nonzero and the pointer at owner+12 is nonzero (call
//    it the anchor), the three floats at +124 are set to anchor+16 plus 0.0f, anchor+20 unchanged, and
//    anchor+24 plus 36.0f. With no owner or no anchor +124 is left alone.
// 5. If the signed word frameNm (0x004844a4) is below the signed word at +180, the function returns.
//    Otherwise the first slot of the object's virtual table is
//    called with the object and the value 3. The slot is the 8-byte-offset entry of the table whose
//    address is the word at +72: a halfword adjustment added to the object pointer, then (at +4) the
//    function. Inferred only: +180 is the last frame of the effect and the slot is the destructor with
//    both low flag bits set. Unknown: the exact meaning of the 3.
// Nothing is returned. NaN handling in the clamps is not established; the clamps use plain compares.
// The object holds 32-bit addresses. The host check defines NORRATH_HOST_LIGHTEFFECT, which turns a stored
// address into an offset inside a test arena and routes the virtual call to a recorder.
#if defined(NORRATH_HOST_LIGHTEFFECT)
extern unsigned char* norrath_light_effect_arena;
extern "C" void norrath_light_effect_call(unsigned function, void* object, int flags);
#endif

extern "C" int frameNm;

namespace {
inline unsigned word(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const unsigned*>(base + offset); }
inline unsigned char* address(unsigned bits) {
#if defined(NORRATH_HOST_LIGHTEFFECT)
    return norrath_light_effect_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
}

extern "C" void msg_run__11LightEffect(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    float* f = reinterpret_cast<float*>(object);
    short* h = reinterpret_cast<short*>(object);
    f[35] = f[35] + f[39];
    f[36] = f[36] + f[40];
    f[37] = f[37] + f[41];
    f[38] = f[38] + f[42];
    f[43] = f[43] + f[44];
    float low = word(object, 184) != 0 ? -255.0f : 0.0f;
    for (int i = 35; i < 38; ++i) {
        float value = f[i] < low ? low : f[i];
        f[i] = value > 255.0f ? 255.0f : value;
    }
    h[47] = static_cast<short>(static_cast<int>(f[35]));
    h[48] = static_cast<short>(static_cast<int>(f[36]));
    h[43] = h[47];
    h[49] = static_cast<short>(static_cast<int>(f[37]));
    h[50] = static_cast<short>(static_cast<int>(f[38]));
    h[44] = h[48];
    h[45] = h[49];
    h[46] = h[50];
    float size = f[43];
    float inverse = 1.0f / (size * size);
    f[26] = size;
    f[29] = inverse;
    f[28] = size;
    f[27] = inverse;
    unsigned ownerBits = word(object, 136);
    unsigned anchorBits = ownerBits != 0 ? word(address(ownerBits), 12) : 0;
    if (anchorBits != 0) {
        const float* position = reinterpret_cast<const float*>(address(anchorBits) + 16);
        float x = position[0] + 0.0f;
        float y = position[1];
        float z = position[2] + 36.0f;
        f[31] = x;
        f[32] = y;
        f[33] = z;
    }
    if (!(frameNm < static_cast<int>(word(object, 180)))) {
        const unsigned char* slot = address(word(object, 72)) + 8;
        short adjustment = *reinterpret_cast<const short*>(slot);
        unsigned function = word(slot, 4);
#if defined(NORRATH_HOST_LIGHTEFFECT)
        norrath_light_effect_call(function, object + adjustment, 3);
#else
        reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(function))(object + adjustment, 3);
#endif
    }
}
