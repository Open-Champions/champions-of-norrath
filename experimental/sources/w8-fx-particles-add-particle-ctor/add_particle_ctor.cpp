// Observed __11AddParticleG6Point3PPc at 0x00222a40, 368 bytes: the AddParticle
// constructor taking a Point3 (by address) and an argument list, with the object
// as first argument. The embedded light record starts at object+256 (the address
// passed to lightAdd__FP5Light, 0x0013d230).
//  1. The Point3 is copied to a local first. The GameObject constructor
//     (0x001411f0) runs with that local, a null resource map and the argument list.
//  2. Stores: +72 the address of _vt$11AddParticle (0x0044eb08); +76 0xffffffff;
//     +80, +84, +88 and +96 zero. The ParticleDef default constructor
//     (__11ParticleDef, 0x00180d08) runs on object+172. Then +252 is zero, the
//     bytes at +264 and +265 are zero and +320 is one. Word 12 is stored ORed
//     with 0x400f before particleInit__11AddParticlePPc (0x00222d20) runs on
//     (object, arguments); lightInit__11AddParticlePPc (0x002233c0) follows with
//     the same arguments.
//  3. If the word at +84 is then nonzero (read after both inits): r = float(int
//     word at +324). The local Point3 is copied to +304..+315. Light halfwords
//     are taken from +340, +344, +348 and +352 (signed): they go to +274, +276,
//     +278, +280 and again to +266, +268, +270, +272. r goes to +284 and +292.
//     1.0f / (r * r) goes to +296 and +288. Word +300 is zero. The byte at +264
//     is cleared in the call's delay slot and lightAdd runs on object+256.
// The return value is whatever the last call left in V0; callers here do not
// use it. Fresh compilation is not an original-byte match.
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

extern "C" void __11AddParticleG6Point3PPc(void* object, const void* point, char** arguments) {
    unsigned char* o = static_cast<unsigned char*>(object);
    unsigned char* light = o + 256;
    unsigned char local[12];
    for (unsigned n = 0u; n < 12u; ++n) local[n] = static_cast<const unsigned char*>(point)[n];
    (void)norrath_game_object_construct(object, local, nullptr, arguments);
    write_word(o, 72u, low32(vt_11AddParticle));
    write_word(o, 76u, 0xffffffffu);
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
