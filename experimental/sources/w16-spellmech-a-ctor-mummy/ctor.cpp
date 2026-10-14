// Observed __5MummyG6Point3sPPcPc at 0x00236b68, 1600 bytes in .text. Arguments: the object, a point, a type (short), an
// argument list pointer and a name string. Returns nothing observed.
// Steps, in order:
//  1. The Creature base constructor is called as (object, a copy of the point, the sign-extended type, mummyResourceMap, 10.0f,
//     the argument list). Word +72 = _vt$5Mummy.
//  2. Halfwords at +1068, +1076, +1084 and +1092 = -1. Bytes +1064 and +1065 = -1. The 16 bytes at +1048 .. +1063 = 0.
//  3. Byte +40 = 30. The doubleword at +160 gets the bits 0x0000000400010000 set. Float +168 = the image float 0x3f999999. Word +248 =
//     frameNm. Float +28 = 15.0f. Byte +172 = 16. Words +176, +192 and +236 = 0; halfword +572 = 0; words +1104, +1108, +1112 = 0.
//  4. The name is compared case-insensitively (strticmp, zero when equal) with literal names; word +960 is the kind:
//     "mummytemplesoldier" or "mummytso" give 2; "mummydesertshaman" or "mummydsh" give 1; "mummytempleshaman" or
//     "mummytsh" give 3; "mummyelitesoldier" or "mummyeso" give 4; "mummyeliteshaman" or "mummyesh" give 5; anything else 0.
//  5. Word +244 = 0, then by kind (eRand is the GameObject random step, called on the object):
//       0: word +244 = 1 << (eRand % 3, unsigned); when float(int eRand) * 2^-16 is below the image float 0x3dcccccc (about
//          0.1), word +208 = (eRand & 1) + 1.
//       1: word +244 = 8.    3: word +244 = 256.    5: word +244 = 2048.
//       2: word +244 = 1 << ((eRand & 3) + 4); when float(int eRand) * 2^-16 is below the image float 0x3dcccccc, word +208 =
//          (eRand & 1) + 1.
//       4: word +244 = 1 << ((eRand & 1) + 9); when float(int eRand) * 2^-16 is below the image float 0x3dcccccc, word +208 =
//          (eRand & 3) + 1.
//     Kinds 6 and above would leave +244 at 0.
//  6. The doubleword at +552 = modelGetActiveChunks(the header at (the model at +36)+12, word +244). setWeapon(object);
//     initType(object, 0); animInitState(object+288); setState(object, 0); objectAddToSlowRunList(object).
//  7. setAnimEventFlags(object, id, f1, a, f2, b, f3, c, f4, d) is called twelve times. Every call has a = 1 and f2, f3, f4, b, c,
//     d zero except where stated. By id: 3 gets f1 = 0.1f (image float 0x3dcccccc); 4 gets 0.15f (0x3e199999); 32 gets 0.2f
//     (0x3e4ccccc); 33 gets 1.5f; 34 gets 0.2f; 14 and 15 get 0.1f; 16 gets 0.1f with f2 = 0.5f and b = 1; 17 and 18 get 0.1f;
//     19 gets 0.1f with f2 = 0.5f and b = 1; 43 gets 0.2f with f2 = 0.8f (0x3f4ccccc) and b = 2. finishConstruction(object) is
//     called last.
// Inferred: the constructor of the mummy enemies with five kinds picked from the name. Unknown: the meanings of the fields, the
// weapon masks and the animation flags. Nothing here ran on the EE; checks are the host contract in this directory and a
// private paired run.
#if defined(NORRATH_HOST_ARENA)
// Host checks only: stored 32-bit addresses are arena offsets; the check supplies both conversions.
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
#endif
struct Point3 { float x, y, z; };
struct ListHead { void* first; unsigned unknown[3]; };

extern "C" {
extern unsigned char vt_Mummy[] asm("_vt$5Mummy");
extern unsigned char mummyResourceMap[] asm("mummyResourceMap");
extern int frameNm;
void* __8CreatureG6Point3sPQ210GameObject11resourceMapfPPc(void* object, Point3 point, short type, void* map, float scale, char** arguments);
int strticmp__FPCcT0(const char* left, const char* right);
unsigned eRand__10GameObject(void* object);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* header, int mask);
void setWeapon__5Mummy(void* object);
void initType__5Mummyb(void* object, int flag);
void animInitState__FP14AnimationState(void* state);
void setState__5Mummyi(void* object, int state);
void objectAddToSlowRunList__FP10GameObject(void* object);
void setAnimEventFlags__8Creatureififififi(void* object, int id, float a, int b, float c, int d, float e, int f, float g, int h);
void finishConstruction__8Creature(void* object);
}

namespace {
struct WordTriple { unsigned words[3]; };

#if defined(NORRATH_HOST_ARENA)
inline unsigned address_word(const void* p) { return norrath_host_address(p); }
inline unsigned char* word_pointer(unsigned a) { return a == 0 ? nullptr : norrath_host_pointer(a); }
#else
inline unsigned address_word(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
inline unsigned char* word_pointer(unsigned a) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a)); }
#endif
inline unsigned load_word(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const unsigned*>(base + offset);
}
inline unsigned char* pointer_word(const unsigned char* base, unsigned offset) {
    return word_pointer(load_word(base, offset));
}
inline void store_word(unsigned char* base, unsigned offset, unsigned value) {
    *reinterpret_cast<unsigned*>(base + offset) = value;
}
inline void store_half(unsigned char* base, unsigned offset, unsigned short value) {
    *reinterpret_cast<unsigned short*>(base + offset) = value;
}
inline void store_float(unsigned char* base, unsigned offset, float value) {
    *reinterpret_cast<float*>(base + offset) = value;
}
inline float load_float(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const float*>(base + offset);
}
inline float float_bits(unsigned bits) {
    union { unsigned u; float f; } value;
    value.u = bits;
    return value.f;
}
static char name_temple_soldier[] = "mummytemplesoldier";
static char name_tso[] = "mummytso";
static char name_desert_shaman[] = "mummydesertshaman";
static char name_dsh[] = "mummydsh";
static char name_temple_shaman[] = "mummytempleshaman";
static char name_tsh[] = "mummytsh";
static char name_elite_soldier[] = "mummyelitesoldier";
static char name_eso[] = "mummyeso";
static char name_elite_shaman[] = "mummyeliteshaman";
static char name_esh[] = "mummyesh";
}

extern "C" void __5MummyG6Point3sPPcPc(void* self, Point3 point, int type_word, char** arguments, char* name) {
    // The type argument is a short; the image re-extends its low 16 bits from the register, so it is taken as a word here.
    const short type = static_cast<short>(type_word);
    unsigned char* o = static_cast<unsigned char*>(self);
    __8CreatureG6Point3sPQ210GameObject11resourceMapfPPc(self, point, type, mummyResourceMap, 10.0f, arguments);
    store_word(o, 72, address_word(vt_Mummy));
    for (unsigned i = 0; i < 4; ++i) store_half(o, 1068 + 8 * i, 0xffff);
    o[1065] = 0xff;
    o[1064] = 0xff;
    for (unsigned i = 0; i < 8; ++i) store_half(o, 1048 + 2 * i, 0);
    o[40] = 30;
    unsigned long long flags = *reinterpret_cast<unsigned long long*>(o + 160);
    flags |= 0x0000000400010000ull;
    store_float(o, 168, float_bits(0x3f999999u));
    store_word(o, 248, static_cast<unsigned>(frameNm));
    store_float(o, 28, 15.0f);
    o[172] = 16;
    *reinterpret_cast<unsigned long long*>(o + 160) = flags;
    store_word(o, 176, 0);
    store_word(o, 192, 0);
    store_word(o, 236, 0);
    store_half(o, 572, 0);
    store_word(o, 1104, 0);
    store_word(o, 1108, 0);
    store_word(o, 1112, 0);
    unsigned kind;
    if (strticmp__FPCcT0(name, name_temple_soldier) == 0 || strticmp__FPCcT0(name, name_tso) == 0) kind = 2;
    else if (strticmp__FPCcT0(name, name_desert_shaman) == 0 || strticmp__FPCcT0(name, name_dsh) == 0) kind = 1;
    else if (strticmp__FPCcT0(name, name_temple_shaman) == 0 || strticmp__FPCcT0(name, name_tsh) == 0) kind = 3;
    else if (strticmp__FPCcT0(name, name_elite_soldier) == 0 || strticmp__FPCcT0(name, name_eso) == 0) kind = 4;
    else if (strticmp__FPCcT0(name, name_elite_shaman) == 0 || strticmp__FPCcT0(name, name_esh) == 0) kind = 5;
    else kind = 0;
    store_word(o, 960, kind);
    const unsigned kind_now = load_word(o, 960);
    store_word(o, 244, 0);
    const float scale = float_bits(0x37800000u);
    const float threshold = float_bits(0x3dccccccu);
    switch (kind_now) {
    case 0: {
        const unsigned roll = eRand__10GameObject(o);
        store_word(o, 244, 1u << (roll % 3u));
        if (static_cast<float>(static_cast<int>(eRand__10GameObject(o))) * scale < threshold)
            store_word(o, 208, (eRand__10GameObject(o) & 1u) + 1u);
        break;
    }
    case 1:
        store_word(o, 244, 8);
        break;
    case 2: {
        const unsigned roll = eRand__10GameObject(o);
        store_word(o, 244, 1u << ((roll & 3u) + 4u));
        if (static_cast<float>(static_cast<int>(eRand__10GameObject(o))) * scale < threshold)
            store_word(o, 208, (eRand__10GameObject(o) & 1u) + 1u);
        break;
    }
    case 3:
        store_word(o, 244, 256);
        break;
    case 4: {
        const unsigned roll = eRand__10GameObject(o);
        store_word(o, 244, 1u << ((roll & 1u) + 9u));
        if (static_cast<float>(static_cast<int>(eRand__10GameObject(o))) * scale < threshold)
            store_word(o, 208, (eRand__10GameObject(o) & 3u) + 1u);
        break;
    }
    case 5:
        store_word(o, 244, 2048);
        break;
    default:
        break;
    }
    *reinterpret_cast<unsigned long long*>(o + 552) =
        modelGetActiveChunks__FP12_modelHeaderi(pointer_word(pointer_word(o, 36), 12), static_cast<int>(load_word(o, 244)));
    setWeapon__5Mummy(o);
    initType__5Mummyb(o, 0);
    animInitState__FP14AnimationState(o + 288);
    setState__5Mummyi(o, 0);
    objectAddToSlowRunList__FP10GameObject(o);
    const float tenth = float_bits(0x3dccccccu), fifth = float_bits(0x3e4cccccu);
    setAnimEventFlags__8Creatureififififi(o, 3, tenth, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(o, 4, float_bits(0x3e199999u), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(o, 32, fifth, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(o, 33, 1.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(o, 34, fifth, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(o, 14, tenth, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(o, 15, tenth, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(o, 16, tenth, 1, 0.5f, 1, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(o, 17, tenth, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(o, 18, tenth, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(o, 19, tenth, 1, 0.5f, 1, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(o, 43, fifth, 1, float_bits(0x3f4cccccu), 2, 0.0f, 0, 0.0f, 0);
    finishConstruction__8Creature(o);
}
