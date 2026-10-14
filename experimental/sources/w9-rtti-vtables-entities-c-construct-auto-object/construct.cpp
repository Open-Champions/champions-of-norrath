#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed __10AutoObjectPcG6Point3sPPc at 0x21e548, 1732 bytes, emitted here as
// norrath_construct_AutoObject (the name its AutoProp, AutoChest and
// AutoContainer callers import). Arguments: the object, a name string, a point
// (passed by address, copied to a 12-byte local first), a short type and the
// argument list. In order:
//  1. GameObject construction with the point copy, no resource map and the
//     argument list; then the table address 0x44ed78 (_vt$10AutoObject) goes
//     to +72, the byte at +157 becomes 1 and the halfword at +56 the type.
//  2. The name up to its first NUL, '-', ';' or ',' (nothing for an empty or
//     delimiter-first name) is copied to +76 with ASCII capitals lowered through
//     the _ctype_ upper-case bit.
//  3. The lump name at +109 is the "LumpName" string tag (found twice, the
//     second result copied) or else a copy of the +76 text; ".lmp" is appended.
//     A local copy of +76 plus ".vif" looks up the word at +144 and plus ".tex"
//     the word at +148 (lumpFindResource__FPcT0 with the +109 name first).
//  4. Singles 12.0 at +28 and 60.0 at +32. A "radius" string tag sets +28 to its
//     integer value and, when that value is zero, ORs 1 into the word at +12; a
//     "radius2" tag does the same. A nonzero "height" integer tag sets +32.
//  5. Bytes +40 and +41 are the unsigned byte conversions of the singles +28 and
//     +32. When the word at +144 is nonzero, modelGetBoundingBox(model, -1,
//     six int outputs) gives integers b0..b5; the byte at +40 becomes the
//     unsigned conversion of sqrt((b0-b3)^2 + (b1-b4)^2) * 0.5 and the byte at
//     +41 that of sqrt((b2-b5)^2) * 0.5 (the first two terms cancel), and when
//     either low byte, taken as a single, is below the single at +28 the byte
//     is replaced by the unsigned conversion of +28.
//  6. "drawradius" and "drawheight" integer tags, when their string tag exists,
//     set bytes +40 and +41 to min(value, 255) (signed compare).
//  7. animInitState on +152. When the byte cutMode is zero and an "initScript"
//     string tag exists, scriptStart__FPc is called with it. The byte at +108 is
//     whether a "multiwak" string tag exists. A nonzero "fullbright" integer
//     tag ORs 2 into the word at +208.
// The result left in V0 is the "fullbright" tag value, or the new word at +208
// when that tag is nonzero; callers discard it.
// Unknown: the names are as found in the image; what each tag does for gameplay
// beyond these field writes is not established.
extern "C" {
extern const unsigned char _ctype_[];
extern char cutMode;
extern unsigned char norrath_rtti_entities_a_view_44ed78[];
unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments);
char* findTagString__10GameObjectPci(void* object, char* tag, int occurrence);
int findTagInt__10GameObjectPci(void* object, char* tag, int occurrence);
char* strcpy(char* destination, const char* source);
char* strcat(char* destination, const char* source);
void* lumpFindResource__FPcT0(char* lump, char* name);
void modelGetBoundingBox__FP12_modelHeaderlPiN52(void* model, long selector, int* a, int* b, int* c, int* d, int* e, int* f);
void animInitState__FP14AnimationState(void* state);
void scriptStart__FPc(char* name);
}

namespace {
using namespace rtti_c;
__attribute__((always_inline)) inline float root(float value) {
#if defined(__mips__)
    float result;
    __asm__("sqrt.s %0,%1" : "=f"(result) : "f"(value));
    return result;
#else
    return __builtin_sqrtf(value);
#endif
}
__attribute__((always_inline)) inline unsigned char byte_of(float value) {
    return static_cast<unsigned char>(static_cast<unsigned>(value));
}
// Float tag stores shared by "radius" and "radius2".
__attribute__((always_inline)) inline void radius_tag(void* self, Address object, char* tag) {
    if (findTagString__10GameObjectPci(self, tag, 0) != nullptr) {
        const float value = static_cast<float>(findTagInt__10GameObjectPci(self, tag, 0));
        write_single(object + 28u, value);
        if (value == 0.0f) write_word(object + 12u, read_word(object + 12u) | 1u);
    }
}
__attribute__((always_inline)) inline void clamp_tag(void* self, Address object, char* tag, unsigned offset) {
    if (findTagString__10GameObjectPci(self, tag, 0) != nullptr) {
        const int value = findTagInt__10GameObjectPci(self, tag, 0);
        write_u8(object + offset, static_cast<unsigned>(255 < value ? 255 : value));
    }
}
}

extern "C" unsigned norrath_construct_AutoObject(void* self, char* name, const void* point, short type, char** arguments) {
    const Address object = address(self);
    unsigned char copy[12];
    for (unsigned i = 0; i < 12u; ++i) copy[i] = static_cast<unsigned char>(read_u8(address(point) + i));
    norrath_game_object_construct(self, copy, nullptr, arguments);
    write_word(object + 72u, address_bits(norrath_rtti_entities_a_view_44ed78));
    write_u8(object + 157u, 1u);
    write_u16(object + 56u, static_cast<unsigned>(type));
    char* const base = reinterpret_cast<char*>(bytes(object + 76u));
    {
        const char* source = name;
        char* out = base;
        if (*source != '\0' && *source != '-' && *source != ';' && *source != ',') {
            do {
                const int c = *source++;
                *out++ = static_cast<char>((_ctype_[1 + c] & 1) != 0 ? c + 32 : c);
            } while (*source != '\0' && *source != '-' && *source != ';' && *source != ',');
        }
        *out = '\0';
    }
    char* const lump = reinterpret_cast<char*>(bytes(object + 109u));
    if (findTagString__10GameObjectPci(self, const_cast<char*>("LumpName"), 0) != nullptr) {
        strcpy(lump, findTagString__10GameObjectPci(self, const_cast<char*>("LumpName"), 0));
    } else {
        strcpy(lump, base);
    }
    strcat(lump, ".lmp");
    char local[48];
    strcpy(local, base);
    strcat(local, ".vif");
    write_word(object + 144u, address_bits(lumpFindResource__FPcT0(lump, local)));
    strcpy(local, base);
    strcat(local, ".tex");
    write_word(object + 148u, address_bits(lumpFindResource__FPcT0(lump, local)));
    write_single(object + 28u, single_from_bits(0x41400000u));
    write_single(object + 32u, single_from_bits(0x42700000u));
    radius_tag(self, object, const_cast<char*>("radius"));
    radius_tag(self, object, const_cast<char*>("radius2"));
    if (findTagInt__10GameObjectPci(self, const_cast<char*>("height"), 0) != 0) {
        write_single(object + 32u, static_cast<float>(findTagInt__10GameObjectPci(self, const_cast<char*>("height"), 0)));
    }
    write_u8(object + 40u, byte_of(read_single(object + 28u)));
    write_u8(object + 41u, byte_of(read_single(object + 32u)));
    if (read_word(object + 144u) != 0u) {
        int box[6];
        modelGetBoundingBox__FP12_modelHeaderlPiN52(pointer(read_word(object + 144u)), -1, &box[0], &box[1], &box[2], &box[3], &box[4], &box[5]);
        const float limit = read_single(object + 28u);
        const float half = single_from_bits(0x3f000000u);
        const float dx = static_cast<float>(box[0]) - static_cast<float>(box[3]);
        const float dy = static_cast<float>(box[1]) - static_cast<float>(box[4]);
        const unsigned wide = static_cast<unsigned>(root(dx * dx + dy * dy) * half);
        write_u8(object + 40u, wide);
        if (static_cast<float>(wide & 0xffu) < limit) write_u8(object + 40u, byte_of(limit));
        const float dz = static_cast<float>(box[2]) - static_cast<float>(box[5]);
        const unsigned tall = static_cast<unsigned>(root(0.0f * 0.0f + 0.0f * 0.0f + dz * dz) * half);
        write_u8(object + 41u, tall);
        if (static_cast<float>(tall & 0xffu) < limit) write_u8(object + 41u, byte_of(limit));
    }
    clamp_tag(self, object, const_cast<char*>("drawradius"), 40u);
    clamp_tag(self, object, const_cast<char*>("drawheight"), 41u);
    animInitState__FP14AnimationState(bytes(object + 152u));
    if (cutMode == 0) {
        char* script = findTagString__10GameObjectPci(self, const_cast<char*>("initScript"), 0);
        if (script != nullptr) scriptStart__FPc(script);
    }
    write_u8(object + 108u, findTagString__10GameObjectPci(self, const_cast<char*>("multiwak"), 0) != nullptr ? 1u : 0u);
    unsigned result = static_cast<unsigned>(findTagInt__10GameObjectPci(self, const_cast<char*>("fullbright"), 0));
    if (result != 0u) {
        result = read_word(object + 208u) | 2u;
        write_word(object + 208u, result);
    }
    return result;
}
