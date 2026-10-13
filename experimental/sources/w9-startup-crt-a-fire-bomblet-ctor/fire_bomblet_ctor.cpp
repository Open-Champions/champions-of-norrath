// Observed __11FireBombletG6Point3sPcT1P6Point3i at 0x00289d60, 576 bytes: the FireBomblet constructor. Arguments:
// the object, the address of a Point3 P, a signed short heading, a marker string pointer, the address of a Point3 A,
// the address of a Point3 V (may be null) and an integer. P, A and V are passed by value in the source; the callee
// receives their addresses. In order:
//  1. P and A are copied to locals. The GameObject constructor (0x001411f0) runs on the object with the copy of A, the
//     resource map bombResMap (0x00438e98) and a null argument list.
//  2. Word +72 receives the address of _vt$11FireBomblet (0x00450620). The 12 bytes at +136 receive A. The embedded
//     light record at object+88 is filled: byte +9 zero; halfwords at light +10, +12, +14, +16 are 255, 128, 0,
//     -16 and at +18, +20, +22, +24 the same four; floats at +28 and +36 are 120.0; floats at +32 and +40 are the single
//     at 0x00482588 (bit pattern 0x3891a2b3, which is 1 / 120^2); word +44 is zero; byte +8 is zero and that store
//     happens before lightAdd__FP5Light runs on the light.
//  3. Byte +40 becomes 12, float +28 becomes 12.0, word +172 receives the marker pointer, and word +12 is stored as
//     16385 (replaced, not ORed). Then:
//     - V null: byte +155 is zero; float +84 is 16.0; float +76 is (P.x - object.x) * k1 * k2 and float +80 is
//       (P.y - object.y) * k1 * k2, where object.x and object.y are the words at +16 and +20 and k1, k2 are the
//       singles at 0x0048258c (bit pattern 0x3e88b49e, about 0.267) and 0x00482590 (0x3c888888, 1/60 as a single);
//       float +164 is A.z.
//     - V non-null: byte +155 is one; V is copied to +76..+87; float +164 is P.z - (object.z - P.z) * 3.0, with
//       object.z the float at +24.
//  4. Halfword +170 is 617; the 64-bit slot at +176 is all ones; halfword +56 is the heading minus 16384 (after
//     sign extension); word +148 is all ones; byte +157 is the low byte of the integer; bytes +152, +153, +154 and
//     +156 are zero; word +160 and halfword +168 are zero. The store to +156 happens before the final call,
//     objectAddToRunList__FP10GameObject, runs on the object.
// Nothing is returned. What the fields mean is not established here. The resource map and light provider names are
// the ones other authored units already use; the vtable name is the executable's. Fresh compilation is not an
// original-byte match.
extern "C" {
unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments);
void norrath_light_add(void* light);
void objectAddToRunList__FP10GameObject(void* object);
extern unsigned char norrath_actors_a_view_438e98[];
extern unsigned char vt_11FireBomblet[] __asm__("_vt$11FireBomblet");
}

namespace {
struct Point3Words {
    unsigned x, y, z;
};

float as_float(unsigned bits) { return __builtin_bit_cast(float, bits); }
unsigned as_bits(float value) { return __builtin_bit_cast(unsigned, value); }
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }
}

extern "C" void __11FireBombletG6Point3sPcT1P6Point3i(void* object, const void* position, short heading, char* marker,
                                                       const void* anchor, const void* velocity, int value) {
    unsigned* words = static_cast<unsigned*>(object);
    unsigned char* bytes = static_cast<unsigned char*>(object);
    const Point3Words p = *static_cast<const Point3Words*>(position);
    const Point3Words a = *static_cast<const Point3Words*>(anchor);
    const int angle = static_cast<int>(heading) - 16384;
    (void)norrath_game_object_construct(object, &a, norrath_actors_a_view_438e98, nullptr);
    words[18] = low32(vt_11FireBomblet);
    unsigned char* light = bytes + 88;
    light[9] = 0;
    words[34] = a.x;
    words[35] = a.y;
    words[36] = a.z;
    unsigned short* halves = reinterpret_cast<unsigned short*>(light);
    unsigned* light_words = reinterpret_cast<unsigned*>(light);
    halves[5] = 255;
    halves[6] = 128;
    halves[7] = 0;
    halves[8] = 0xfff0;
    halves[9] = 255;
    halves[10] = 128;
    halves[11] = 0;
    halves[12] = 0xfff0;
    light_words[7] = as_bits(120.0f);
    light_words[9] = as_bits(120.0f);
    light_words[8] = 0x3891a2b3u;
    light_words[10] = 0x3891a2b3u;
    light_words[11] = 0u;
    light[8] = 0;
    norrath_light_add(light);
    const float k1 = as_float(0x3e88b49eu);
    const float k2 = as_float(0x3c888888u);
    bytes[40] = 12;
    words[7] = as_bits(12.0f);
    words[43] = low32(marker);
    words[3] = 16385u;
    if (velocity == nullptr) {
        const float dx = as_float(p.x) - as_float(words[4]);
        const float dy = as_float(p.y) - as_float(words[5]);
        bytes[155] = 0;
        words[21] = as_bits(16.0f);
        words[41] = a.z;
        words[19] = as_bits(dx * k1 * k2);
        words[20] = as_bits(dy * k1 * k2);
    } else {
        bytes[155] = 1;
        const Point3Words v = *static_cast<const Point3Words*>(velocity);
        words[19] = v.x;
        words[20] = v.y;
        words[21] = v.z;
        words[41] = as_bits(as_float(p.z) - (as_float(words[6]) - as_float(p.z)) * 3.0f);
    }
    *reinterpret_cast<unsigned short*>(bytes + 170) = 617;
    words[44] = 0xffffffffu;
    words[45] = 0xffffffffu;
    *reinterpret_cast<unsigned short*>(bytes + 56) = static_cast<unsigned short>(angle);
    words[37] = 0xffffffffu;
    bytes[157] = static_cast<unsigned char>(value);
    bytes[152] = 0;
    bytes[153] = 0;
    words[40] = 0u;
    bytes[154] = 0;
    *reinterpret_cast<unsigned short*>(bytes + 168) = 0;
    bytes[156] = 0;
    objectAddToRunList__FP10GameObject(object);
}
