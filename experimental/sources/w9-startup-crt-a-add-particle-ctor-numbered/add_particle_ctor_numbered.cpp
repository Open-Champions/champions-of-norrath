// Observed __11AddParticleG6Point3PPci at 0x00222bb0, 368 bytes: the AddParticle constructor taking a Point3 (by
// address), an argument list and an integer, with the object as first argument. It is the same body as the
// constructor without the integer (__11AddParticleG6Point3PPc at 0x00222a40, authored in
// w8-fx-particles-add-particle-ctor) except for one store: the word at +76 receives the integer instead of
// 0xffffffff. Order of the observed steps:
//  1. The Point3 is copied to a local. The GameObject constructor runs with that local, a null resource map and the
//     argument list.
//  2. Stores: +72 the address of _vt$11AddParticle; +76 the integer argument; +80, +84, +88 and +96 zero. The
//     ParticleDef default constructor (__11ParticleDef, authored in w9-startup-crt-a-particle-def-default-ctor)
//     runs on object+172. Then +252 is zero, the bytes at +264 and +265 are zero and +320 is one. Word 12 is stored
//     ORed with 0x400f before particleInit__11AddParticlePPc runs on (object, arguments); lightInit__11AddParticlePPc
//     follows with the same arguments.
//  3. If the word at +84 is then nonzero (read after both inits): r = float(int word at +324). The local Point3 is
//     copied to +304..+315. Light halfwords are taken from +340, +344, +348 and +352 (signed): they go to +274, +276,
//     +278, +280 and again to +266, +268, +270, +272. r goes to +284 and +292. 1.0f / (r * r) goes to +296 and +288.
//     Word +300 is zero. The byte at +264 is cleared in the call's delay slot and lightAdd runs on object+256.
// The AutoPropLP constructor passes its loop index 0..3, one per light; what this constructor's later code does with
// the stored word is not observed here. Nothing is returned (the original leaves the last call's V0; callers here
// do not use it). The GameObject constructor and lightAdd are reached through the provider names the sibling
// constructor already uses. Fresh compilation is not an original-byte match.
extern "C" {
unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments);
void __11ParticleDef(void* definition);
void particleInit__11AddParticlePPc(void* object, char** arguments);
void lightInit__11AddParticlePPc(void* object, char** arguments);
void norrath_light_add(void* light);
extern unsigned char vt_11AddParticle[] __asm__("_vt$11AddParticle");
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
void write_half(unsigned char* base, unsigned offset, unsigned value) {
    base[offset] = static_cast<unsigned char>(value);
    base[offset + 1] = static_cast<unsigned char>(value >> 8);
}
void write_float(unsigned char* base, unsigned offset, float value) {
    union { float f; unsigned u; } bits;
    bits.f = value;
    write_word(base, offset, bits.u);
}
float read_int_as_float(const unsigned char* base, unsigned offset) {
    return static_cast<float>(static_cast<int>(read_word(base, offset)));
}
unsigned read_half(const unsigned char* base, unsigned offset) {
    return unsigned(base[offset]) | (unsigned(base[offset + 1]) << 8);
}
}

extern "C" void __11AddParticleG6Point3PPci(void* object, const void* point, char** arguments, int number) {
    unsigned char* o = static_cast<unsigned char*>(object);
    unsigned char* light = o + 256;
    unsigned char local[12];
    for (unsigned n = 0u; n < 12u; ++n) local[n] = static_cast<const unsigned char*>(point)[n];
    (void)norrath_game_object_construct(object, local, nullptr, arguments);
    write_word(o, 72u, low32(vt_11AddParticle));
    write_word(o, 76u, static_cast<unsigned>(number));
    write_word(o, 80u, 0u);
    write_word(o, 84u, 0u);
    write_word(o, 88u, 0u);
    write_word(o, 96u, 0u);
    __11ParticleDef(o + 172);
    write_word(o, 252u, 0u);
    light[8] = 0u;
    light[9] = 0u;
    write_word(o, 320u, 1u);
    write_word(o, 12u, read_word(o, 12u) | 0x400fu);
    particleInit__11AddParticlePPc(object, arguments);
    lightInit__11AddParticlePPc(object, arguments);
    if (read_word(o, 84u) == 0u) return;
    const float radius = read_int_as_float(o, 324u);
    const float inverse = 1.0f / (radius * radius);
    const unsigned h0 = read_half(o, 340u);
    const unsigned h1 = read_half(o, 344u);
    const unsigned h2 = read_half(o, 348u);
    const unsigned h3 = read_half(o, 352u);
    for (unsigned n = 0u; n < 12u; ++n) o[304u + n] = local[n];
    write_half(light, 18u, h0);
    write_half(light, 20u, h1);
    write_half(light, 22u, h2);
    write_half(light, 24u, h3);
    write_float(light, 28u, radius);
    write_float(light, 36u, radius);
    write_word(light, 44u, 0u);
    write_float(light, 40u, inverse);
    write_float(light, 32u, inverse);
    write_half(light, 10u, h0);
    write_half(light, 12u, h1);
    write_half(light, 14u, h2);
    write_half(light, 16u, h3);
    light[8] = 0u;
    norrath_light_add(light);
}
