// Observed __11LightEffectG6Point3ffffffffffffP10GameObjecti at 0x001e5278,
// 648 bytes: the LightEffect constructor. Arguments: the object, a Point3 by
// address, twelve floats (f1..f12; the first eight arrive in float registers, the
// rest in 8-byte stack slots), an owner object pointer and an integer. The light
// record starts at object+76 (the address passed to lightAdd__FP5Light).
//  1. The Point3 is copied to a local; the GameObject constructor (0x001411f0)
//     runs with that local, a null resource map and a null argument list.
//  2. The vtable address of _vt$11LightEffect (0x00449a70) goes to +72 and the
//     byte at +85 is cleared. The local Point3 is copied to +124..+135. With
//     r = f9 and c = 1.0f / (r * r): light word +40 and +32 take c, +44 takes
//     f11, +28 and +36 take r. The signed low halves of the truncated f2, f1, f3
//     and f4 go to light halfwords +10,+12,+14,+16 and again to +18,+20,+22,+24
//     (that is, f1 truncated goes to +10 and +18, f2 to +12 and +20, f3 to +14
//     and +22, f4 to +16 and +24). The byte at +8 is cleared in the delay slot
//     of the lightAdd call.
//  3. After lightAdd: the word at +136 is zeroed, word 12 is stored ORed with 3 and,
//     when the owner is nonzero, listOnewayLink__FP8ListHeadT0Pv runs with
//     (object+136, owner, owner).
//  4. n = truncated (f12 * 60.0f), at least 1 (a value of zero or less becomes 1).
//     k = 1.0f / float(n). Stores: +140 f1, +144 f2, +148 f3, +152 f4, +172 f9,
//     +180 frameNm + n, +156 (f5 - f1) * k, +160 (f6 - f2) * k, +164 (f7 - f3) * k,
//     +168 (f8 - f4) * k, +176 (f10 - f9) * k. The +168 store sits in the delay slot
//     of the objectAddToRunList call, so it lands before that call runs.
//  5. After that call, +184 takes the integer argument.
// Fresh compilation is not an original-byte match. Floats convert by truncation.
extern "C" {
unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments);
void norrath_light_add(void* light);
void* norrath_list_oneway_link(void* head, void* object_head, void* value);
unsigned norrath_actor_add_run_list(void* object);
extern unsigned frameNm;
extern unsigned char vt_11LightEffect[] __asm__("_vt$11LightEffect");
}

namespace {
using Address = __UINTPTR_TYPE__;
unsigned low32(const void* p) { return static_cast<unsigned>(reinterpret_cast<Address>(p)); }
unsigned read_word(const unsigned char* base, unsigned offset) {
    return unsigned(base[offset]) | (unsigned(base[offset + 1]) << 8) | (unsigned(base[offset + 2]) << 16) |
           (unsigned(base[offset + 3]) << 24);
}
void write_word(unsigned char* base, unsigned offset, unsigned value) {
    base[offset] = static_cast<unsigned char>(value);
    base[offset + 1] = static_cast<unsigned char>(value >> 8);
    base[offset + 2] = static_cast<unsigned char>(value >> 16);
    base[offset + 3] = static_cast<unsigned char>(value >> 24);
}
void write_half(unsigned char* base, unsigned offset, int value) {
    base[offset] = static_cast<unsigned char>(value);
    base[offset + 1] = static_cast<unsigned char>(value >> 8);
}
void write_float(unsigned char* base, unsigned offset, float value) {
    union { float f; unsigned u; } bits;
    bits.f = value;
    write_word(base, offset, bits.u);
}
}

extern "C" void __11LightEffectG6Point3ffffffffffffP10GameObjecti(
    void* object, const void* point, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8,
    float f9, float f10, float f11, float f12, void* owner, int value) {
    unsigned char* o = static_cast<unsigned char*>(object);
    unsigned char* light = o + 76;
    unsigned char local[12];
    for (unsigned n = 0u; n < 12u; ++n) local[n] = static_cast<const unsigned char*>(point)[n];
    (void)norrath_game_object_construct(object, local, nullptr, nullptr);
    const int h1 = static_cast<int>(f1);
    const int h2 = static_cast<int>(f2);
    const int h3 = static_cast<int>(f3);
    const int h4 = static_cast<int>(f4);
    const float inverse = 1.0f / (f9 * f9);
    write_word(o, 72u, low32(vt_11LightEffect));
    light[9] = 0u;
    for (unsigned n = 0u; n < 12u; ++n) o[124u + n] = local[n];
    write_float(light, 40u, inverse);
    write_float(light, 44u, f11);
    write_half(light, 18u, h1);
    write_half(light, 20u, h2);
    write_half(light, 22u, h3);
    write_half(light, 24u, h4);
    write_float(light, 28u, f9);
    write_float(light, 36u, f9);
    write_float(light, 32u, inverse);
    write_half(light, 10u, h1);
    write_half(light, 12u, h2);
    write_half(light, 14u, h3);
    write_half(light, 16u, h4);
    light[8] = 0u;
    norrath_light_add(light);
    write_word(o, 136u, 0u);
    write_word(o, 12u, read_word(o, 12u) | 3u);
    if (owner != nullptr) norrath_list_oneway_link(o + 136, owner, owner);
    int frames = static_cast<int>(f12 * 60.0f);
    if (frames <= 0) frames = 1;
    write_float(o, 140u, f1);
    write_float(o, 144u, f2);
    write_float(o, 148u, f3);
    write_float(o, 152u, f4);
    write_float(o, 172u, f9);
    write_word(o, 180u, frameNm + static_cast<unsigned>(frames));
    const float step = 1.0f / static_cast<float>(frames);
    write_float(o, 176u, (f10 - f9) * step);
    write_float(o, 156u, (f5 - f1) * step);
    write_float(o, 160u, (f6 - f2) * step);
    write_float(o, 164u, (f7 - f3) * step);
    write_float(o, 168u, (f8 - f4) * step);
    (void)norrath_actor_add_run_list(object);
    write_word(o, 184u, static_cast<unsigned>(value));
}
