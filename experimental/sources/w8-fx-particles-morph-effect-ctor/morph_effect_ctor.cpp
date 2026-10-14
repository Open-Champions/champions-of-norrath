// Observed __19ParticleMorphEffectP8CreatureT1 at 0x00155f08, 148 bytes. Three
// arguments: the object, and two creature pointers.
//  1. The GameObject constructor (0x001411f0) runs with a Point3 of three zero
//     floats built in the frame, a null resource map and a null argument list.
//  2. Object word 12 is read after that call and stored ORed with 0x421f.
//  3. Stores: object+72 takes the address of _vt$19ParticleMorphEffect
//     (0x0044d528), +76 takes frameNm (0x004844a4) plus 150, +80 takes frameNm,
//     +84 takes the first creature and +88 the second. frameNm is read once.
//  4. objectAddToRunList__FP10GameObject (0x00143830) is called with the object
//     and its result is the function's result in V0.
// The vtable and field meanings beyond these offsets are not established here.
// Fresh compilation is not an original-byte match.
extern "C" {
unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments);
unsigned norrath_actor_add_run_list(void* object);
extern unsigned frameNm;
extern unsigned char vt_19ParticleMorphEffect[] __asm__("_vt$19ParticleMorphEffect");
}

namespace {
using Address = __UINTPTR_TYPE__;
unsigned char* bytes(void* object) {
    return static_cast<unsigned char*>(object);
}
unsigned read_word(unsigned char* base, unsigned offset) {
    return unsigned(base[offset]) | (unsigned(base[offset + 1]) << 8) | (unsigned(base[offset + 2]) << 16) |
           (unsigned(base[offset + 3]) << 24);
}
void write_word(unsigned char* base, unsigned offset, unsigned value) {
    base[offset] = static_cast<unsigned char>(value);
    base[offset + 1] = static_cast<unsigned char>(value >> 8);
    base[offset + 2] = static_cast<unsigned char>(value >> 16);
    base[offset + 3] = static_cast<unsigned char>(value >> 24);
}
unsigned low32(const void* p) {
    return static_cast<unsigned>(reinterpret_cast<Address>(p));
}
}

extern "C" unsigned __19ParticleMorphEffectP8CreatureT1(void* object, void* first, void* second) {
    const float zero[3] = {0.0f, 0.0f, 0.0f};
    (void)norrath_game_object_construct(object, zero, nullptr, nullptr);
    unsigned char* o = bytes(object);
    const unsigned frame = frameNm;
    const unsigned flags = read_word(o, 12u);
    write_word(o, 72u, low32(vt_19ParticleMorphEffect));
    write_word(o, 76u, frame + 150u);
    write_word(o, 12u, flags | 0x421fu);
    write_word(o, 84u, low32(first));
    write_word(o, 88u, low32(second));
    write_word(o, 80u, frame);
    return norrath_actor_add_run_list(object);
}
