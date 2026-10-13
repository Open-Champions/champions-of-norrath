// Observed __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description
// at 0x001450e8, 1356 bytes: the MissileWeapon constructor. Arguments, in order: the object, an integer kind, a model
// header pointer, a texture pointer, the address of a Point3 P (the start), the address of a second Point3 V, a short
// held in an integer, a creature pointer, two sound header pointers, six integers and an item description pointer.
// The first eight travel in registers; the rest are on the stack from 176 bytes above the frame in the order given.
// Observed steps:
//  1. P and V are copied to locals. The GameObject constructor (0x001411f0) runs on the object with a copy of P, a
//     null resource map and a null argument list. Word +72 receives the address of _vt$10Projectile (0x0044d378);
//     word +76 is cleared, word +12 is ORed with 0x4081 and stored, float +28 becomes 12.0 and byte +80 is cleared.
//     A non-null creature is linked with listOnewayLink__FP8ListHeadT0Pv(object+76, creature, creature). Word +72
//     then becomes the address of _vt$13MissileWeapon (0x00449e78).
//  2. The third word of each of five 24-byte entries from +84 is cleared (words +92, +116, +140, +164, +188). The
//     embedded light at object+368 is filled and added with lightAdd: word +292 is zero; the 12 bytes of P go to
//     +416; light byte +9 is zero; halfwords at light +10, +12, +14 are 255, 155, 0 and at +18, +20, +22 the same;
//     halfwords at +16 and +24 are -32; floats at +28 and +36 are 168.0; floats at +32 and +40 are the single at
//     0x0047fcfc (bit pattern 0x38149b92, which is 1 / 168^2); word +44 is zero; byte +8 is zero and that store
//     happens before lightAdd runs.
//  3. Byte +40 is 72; bytes +254 and halfwords +252 and +250 are zero; word +208 is the kind. For kind 5 or 6 the
//     object's light halfwords at +380 and +388 become 155, at +382 and +390 become 255, and at +386 and +378 zero.
//     Kind 2, 1, 5 or 6 starts a sound sequence: lumpFindResource("fx.lmp", name) with name "spel_cas_explarrow.anm",
//     "spel_cas_firearrow.anm", "spel_cas_icearrow.anm" or "spel_cas_shkarrow.anm" respectively, passed to
//     soundPlaySequence__FP15AnimationHeaderP10GameObjectf with the object and 1.0. Kind 0 or 3 deactivates the light.
//  4. When the description is non-null and its word +0 is non-zero, four 8-byte entries from description+32 are
//     searched, for the keys 24, 22 and 21 in that order, by their halfword at entry+4; the first entry with the key
//     supplies its halfword at entry+6. If any of the three values is non-zero, word +364 is cleared and a trail is
//     not started. Otherwise, when the kind is zero and bit 0x100 of levelFlags is clear, P_StartTrail__Fiifiiiii is
//     called with 1, 122, the single at 0x0047fd00 (0.151), 0, 128, 128, 128, 32; its result goes to +364 and
//     P_ExtendTrail__FiG6Point3 is called with it and a copy of the object's position (+16).
//  5. The six integers go to +212, +216, +220, +224, +228 and +232. The 12 bytes of V go to +428. Byte +204 is zero
//     and byte +248 is one. Halfword +56 receives the short; words +444 and +448 receive the model and texture
//     pointers and word +296 the description pointer. Word +12 is read again, ORed with 0x4001 and stored; word +236
//     is frameNm + 300 and word +244 is zero (stored before the next call). objectAddToRunList__FP10GameObject runs
//     on the object, then byte +205 becomes one.
//  6. A non-null first sound header is played with soundPlaySound__FP10_vagHeaderP6Point3fff at the object position
//     (+16) with 1.0, 1.0 and 0.0, and its result goes to +240; with no header +240 is zero. The second sound header
//     goes to +440.
// Nothing is returned. What the integers, the kind values and the description entries mean is not established here.
// The GameObject construction and lightAdd go through the provider names other authored units use; the other callees
// have the executable's names. Fresh compilation is not an original-byte match.
extern "C" {
unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments);
void* listOnewayLink__FP8ListHeadT0Pv(void* head, void* object_head, void* value);
void norrath_light_add(void* light);
void lightDeactivate__FP5Light(void* light);
void* lumpFindResource__FPcT0(char* lump_name, char* resource_name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float gain);
int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h);
void P_ExtendTrail__FiG6Point3(int trail, const void* point);
void objectAddToRunList__FP10GameObject(void* object);
int soundPlaySound__FP10_vagHeaderP6Point3fff(void* header, void* position, float a, float b, float c);
extern unsigned char vt_10Projectile[] __asm__("_vt$10Projectile");
extern unsigned char vt_13MissileWeapon[] __asm__("_vt$13MissileWeapon");
extern int frameNm;
extern unsigned levelFlags;
}

namespace {
struct Point3Words {
    unsigned x, y, z;
};

unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

bool description_has_effect(const unsigned char* description) {
    static const short keys[3] = {24, 22, 21};
    for (int k = 0; k < 3; ++k) {
        short value = 0;
        for (int i = 0; i < 4; ++i) {
            if (*reinterpret_cast<const short*>(description + 36 + 8 * i) == keys[k]) {
                value = *reinterpret_cast<const short*>(description + 38 + 8 * i);
                break;
            }
        }
        if (value != 0) return true;
    }
    return false;
}

void play_effect_sequence(void* object, char* name) {
    static char lump[] = "fx.lmp";
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump, name), object, 1.0f);
}
}

extern "C" void __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
    void* object, int kind, void* model, void* texture, const void* position, const void* velocity, int angle,
    void* creature, void* sound_a, void* sound_b, int a, int b, int c, int d, int e, int f, void* description_pointer) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    unsigned* words = static_cast<unsigned*>(object);
    unsigned char* description = static_cast<unsigned char*>(description_pointer);
    const Point3Words p = *static_cast<const Point3Words*>(position);
    const Point3Words v = *static_cast<const Point3Words*>(velocity);
    (void)norrath_game_object_construct(object, &p, nullptr, nullptr);
    words[18] = low32(vt_10Projectile);
    words[19] = 0u;
    words[3] = words[3] | 0x4081u;
    words[7] = 0x41400000u;
    bytes[80] = 0;
    if (creature != nullptr) listOnewayLink__FP8ListHeadT0Pv(bytes + 76, creature, creature);
    words[18] = low32(vt_13MissileWeapon);
    for (int entry = 0; entry < 5; ++entry) words[23 + 6 * entry] = 0u;
    unsigned char* light = bytes + 368;
    unsigned short* halves = reinterpret_cast<unsigned short*>(light);
    unsigned* light_words = reinterpret_cast<unsigned*>(light);
    words[73] = 0u;
    light[9] = 0;
    words[104] = p.x;
    words[105] = p.y;
    words[106] = p.z;
    halves[12] = 0xffe0;
    halves[8] = 0xffe0;
    light_words[9] = 0x43280000u;
    light_words[10] = 0x38149b92u;
    light_words[7] = 0x43280000u;
    light_words[8] = 0x38149b92u;
    light_words[11] = 0u;
    halves[5] = 255;
    halves[6] = 155;
    halves[7] = 0;
    halves[9] = 255;
    halves[10] = 155;
    halves[11] = 0;
    light[8] = 0;
    norrath_light_add(light);
    bytes[40] = 72;
    bytes[254] = 0;
    *reinterpret_cast<unsigned short*>(bytes + 252) = 0;
    *reinterpret_cast<unsigned short*>(bytes + 250) = 0;
    words[52] = static_cast<unsigned>(kind);
    if (static_cast<unsigned>(kind - 5) < 2u) {
        *reinterpret_cast<unsigned short*>(bytes + 380) = 155;
        *reinterpret_cast<unsigned short*>(bytes + 382) = 255;
        *reinterpret_cast<unsigned short*>(bytes + 386) = 0;
        *reinterpret_cast<unsigned short*>(bytes + 378) = 0;
        *reinterpret_cast<unsigned short*>(bytes + 388) = 155;
        *reinterpret_cast<unsigned short*>(bytes + 390) = 255;
    }
    if (words[52] == 2u) {
        play_effect_sequence(object, const_cast<char*>("spel_cas_explarrow.anm"));
    } else if (words[52] == 1u) {
        play_effect_sequence(object, const_cast<char*>("spel_cas_firearrow.anm"));
    } else if (words[52] == 5u) {
        play_effect_sequence(object, const_cast<char*>("spel_cas_icearrow.anm"));
    } else if (words[52] == 6u) {
        play_effect_sequence(object, const_cast<char*>("spel_cas_shkarrow.anm"));
    }
    if (words[52] == 0u || words[52] == 3u) lightDeactivate__FP5Light(light);
    bool found = false;
    if (description != nullptr && *reinterpret_cast<unsigned*>(description) != 0u) {
        if (description_has_effect(description)) {
            words[91] = 0u;
            found = true;
        }
    }
    if (!found && words[52] == 0u && (levelFlags & 0x100u) == 0u) {
        const int trail = P_StartTrail__Fiifiiiii(1, 122, __builtin_bit_cast(float, 0x3e1a9fbeu), 0, 128, 128, 128, 32);
        words[91] = static_cast<unsigned>(trail);
        const Point3Words here = {words[4], words[5], words[6]};
        P_ExtendTrail__FiG6Point3(trail, &here);
    }
    words[53] = static_cast<unsigned>(a);
    words[54] = static_cast<unsigned>(b);
    words[55] = static_cast<unsigned>(c);
    words[56] = static_cast<unsigned>(d);
    words[57] = static_cast<unsigned>(e);
    words[58] = static_cast<unsigned>(f);
    words[107] = v.x;
    words[108] = v.y;
    words[109] = v.z;
    bytes[204] = 0;
    bytes[248] = 1;
    *reinterpret_cast<unsigned short*>(bytes + 56) = static_cast<unsigned short>(angle);
    words[111] = low32(model);
    words[112] = low32(texture);
    words[74] = low32(description);
    words[3] = words[3] | 0x4001u;
    words[59] = static_cast<unsigned>(frameNm) + 300u;
    words[61] = 0u;
    objectAddToRunList__FP10GameObject(object);
    bytes[205] = 1;
    if (sound_a != nullptr)
        words[60] = static_cast<unsigned>(soundPlaySound__FP10_vagHeaderP6Point3fff(sound_a, bytes + 16, 1.0f, 1.0f, 0.0f));
    else
        words[60] = 0u;
    words[110] = low32(sound_b);
}
