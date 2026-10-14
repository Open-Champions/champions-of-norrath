// Observed __6PlayerG6Point3sii at 0x00194930, 2208 bytes: the Player
// constructor. Arguments: the object, a position (12 bytes), a short, the player
// index (0..3) and a flag. The player record is player[index], an array of
// 23984-byte records at 0x00408600 (symbol "player"). In the order of the image:
//  1. Creature's constructor (0x00108e58) runs on the object with the position,
//     the short, the resource pointer typeToResource[word at record+84], 13.0f
//     and null.
//  2. _vt$6Player (0x0044c620) goes to object+72; five words at object+976,
//     +1000, +1024, +1048 and +1072 are cleared; the word at +1192 is cleared.
//  3. The light at object+1208 gets byte +9 = 0, halves 188 at +10, +12, +14,
//     +18, +20, +22, halves -64 at +16 and +24, 300.0f at +28 and +36, the float
//     view[0] at +32 and +40, zero at the word +44, byte +8 = 0, and
//     lightAdd (0x0013d230) runs on it. The position is copied to object+1256.
//     Bytes +1276 and +1277 and the word +1396 are cleared; byte +1361 = 1.
//  4. selectLumpName (0x00484548) is set to the string at 0x00464ab8 when
//     nmPlayers >= 2 or coreForceSmallCharacters is nonzero, otherwise to the
//     string at 0x004644c0. A nonzero flag sets thereIsACutPlayer (0x0048459c)
//     to 1. Byte object+1106 = flag, byte +40 = 48, word +1176 = -1.
//  5. When levelFlags (0x004844c0) & 0x24800000 is zero the light is turned off
//     (lightDeactivate 0x0013eb28). Otherwise floats 252.0 and view[1] go to
//     object+1236 and +1240, and if levelFlags & 0x800000 is set the four signed
//     halves at object+1218..+1224 are halved toward zero.
//  6. Resource lookups, all via lumpFindResource(selectLumpName, text): with the
//     record bytes c0 = record[32], c1 = record[33], c2 = record[34] and
//     c3 = record[35], the race prefix p = racePrefix[word at record+84]:
//       c0 == -1: object+1336 and +1352 become 0; else sprintf formats
//         (p, c0+1) -> +1336 and (p, c0+1, c1+'a') -> +1352;
//       c2 == -1: object+1340 and +1348 become 0; else (p, c2+1) -> +1340 and
//         (p, c2+1, c1+'a') -> +1348;
//       always: (p) -> +1332 and (p, c3+1) -> +1344.
//     Six distinct format strings are used (0x464ac8, 0x464ad8, 0x464ae8,
//     0x464af8, 0x464b10, 0x464b20).
//  7. object+12 |= 0x1004040 and object+960 = index. When byte +1106 is zero,
//     scePrintf prints the string at 0x00464b30 and the record's word at +21872
//     becomes the object.
//  8. Float view[2] goes to object+168 and 12.0f to +28. Words are cleared at
//     object+88..+104 (5 words), +108..+120 (4), +124..+136 (4) and +896..+956
//     (16). view[3] goes to +1120, byte +1105 = 1, half +1186 = index*2700 +
//     3600, the bytes +964, +1130, +1131, +1136, +1137, +1138, +1140, +1141,
//     +1142, +1152, +1107, halves +1182 and +1184 and words +1100, +1132, +1144,
//     +1096 are cleared, word +1400 = 2. animInitState (0x001637a8) runs on
//     object+288 and resetPlayerAnimations (0x001a1c98) on the index.
//  9. 1.0f goes to object+304 and +312 and the half 4613 to +324. Then, with x =
//     the signed byte at record+23582: if levelFlags & 0x10000000 is set, the
//     animation table row is 0 and object+296 = word at (word at object+36)+36;
//     otherwise row = 13*x and object+296 =
//     lumpFindResource(word at (word at (record word +21872)+36), s) where s is the string at
//     0x464b48/58/68/78/88 for x = 1/2/3/4/7 and 0x464b98 for any other x.
// 10. With table = word at object+36, entry = table + row*8: object+360 and +392
//     get entry[84] and entry[92], +328 gets entry[76], halves +356, +388 and
//     +420 get 4613, byte +292 = 4, words +336, +344, +368, +376, +400, +408 are
//     cleared, and objectAddToRunList runs.
// 11. For six table entries of 104 bytes each, with a = entry[76] and b =
//     entry[92]: a[24] = a[32] = 1, b[24] = b[32] = 4, floats 0.75 and 0.25 go to
//     a+28 and a+20, view[5] and view[4] to b+28 and b+20.
// 12. object+24 = worldFindHeight(world, position at object+16, float at
//     object+28); playerRecomputeAbilityModifiers runs; memset clears 64 bytes at
//     record+23584 and 64 at record+23648; record byte +23968 = 0;
//     enumerateAlivePlayers runs.
// Inferred: the names of the records and tables come from image symbols; offsets
// have no known field names. Unknown: what most fields and the strings mean. The
// original keeps the table base and reloads the table pointer between the stores
// in step 11; those reloads are kept.
struct Point3 { float x, y, z; };

extern "C" {
unsigned __8CreatureG6Point3sPQ210GameObject11resourceMapfPPc(void* self, Point3 position, short type, void* resource,
                                                             float scale, char** arguments);
void lightAdd__FP5Light(void* light);
void lightDeactivate__FP5Light(void* light);
int sprintf(char* buffer, const char* format, ...);
void* lumpFindResource__FPcT0(char* lump, char* name);
int scePrintf(const char* format, ...);
void animInitState__FP14AnimationState(void* state);
void resetPlayerAnimations__Fi(int index);
unsigned objectAddToRunList__FP10GameObject(void* object);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 position, float height);
void playerRecomputeAbilityModifiers__Fv();
void* memset(void* destination, int value, unsigned count);
void enumerateAlivePlayers__Fv();

extern unsigned char player[4 * 23984];
extern char* racePrefix[10];
extern void* typeToResource[10];
extern char* selectLumpName;
extern unsigned levelFlags;
extern int nmPlayers;
extern char thereIsACutPlayer;
extern char coreForceSmallCharacters;
extern void* world;
extern unsigned char _vt$6Player[];
extern float norrath_w9_startup_crt_b_view_4801f8[6];
extern char norrath_w9_startup_crt_b_view_464ab8[];
extern char norrath_w9_startup_crt_b_view_4644c0[];
extern char norrath_w9_startup_crt_b_view_464ac8[];
extern char norrath_w9_startup_crt_b_view_464ad8[];
extern char norrath_w9_startup_crt_b_view_464ae8[];
extern char norrath_w9_startup_crt_b_view_464af8[];
extern char norrath_w9_startup_crt_b_view_464b10[];
extern char norrath_w9_startup_crt_b_view_464b20[];
extern char norrath_w9_startup_crt_b_view_464b30[];
extern char norrath_w9_startup_crt_b_view_464b48[];
extern char norrath_w9_startup_crt_b_view_464b58[];
extern char norrath_w9_startup_crt_b_view_464b68[];
extern char norrath_w9_startup_crt_b_view_464b78[];
extern char norrath_w9_startup_crt_b_view_464b88[];
extern char norrath_w9_startup_crt_b_view_464b98[];
}

#if defined(NORRATH_HOST_CHECK)
// The host check keeps 32-bit handles in the pointer-sized object fields.
extern "C" void* norrath_host_pointer(unsigned handle);
extern "C" unsigned norrath_host_handle(const void* pointer);
#endif

namespace {
template <class T> T& at(unsigned char* base, unsigned offset) { return *reinterpret_cast<T*>(base + offset); }
#if defined(NORRATH_HOST_CHECK)
unsigned address_bits(const void* pointer) { return norrath_host_handle(pointer); }
unsigned char* pointer_from(unsigned handle) { return static_cast<unsigned char*>(norrath_host_pointer(handle)); }
#else
unsigned address_bits(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }
unsigned char* pointer_from(unsigned handle) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(handle)); }
#endif
unsigned char* load_pointer(unsigned char* base, unsigned offset) { return pointer_from(at<unsigned>(base, offset)); }
void store_pointer(unsigned char* base, unsigned offset, const void* pointer) { at<unsigned>(base, offset) = address_bits(pointer); }
float bits_float(unsigned bits) { return __builtin_bit_cast(float, bits); }
}

// The image sign-extends the short argument itself before passing it on, so the
// parameter is taken as an int here and narrowed explicitly.
extern "C" void __6PlayerG6Point3sii(void* self, Point3 position, int type_bits, int index, int flag) {
    const short type = static_cast<short>(type_bits);
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* record = player + index * 23984;
    const Point3 kept = position;
    char buffer[128];

    __8CreatureG6Point3sPQ210GameObject11resourceMapfPPc(self, position, type, typeToResource[at<int>(record, 84)],
                                                          bits_float(0x41500000u), nullptr);
    at<unsigned>(object, 72) = address_bits(_vt$6Player);
    for (int i = 0; i < 5; ++i) at<unsigned>(object, 976 + 24 * i) = 0;
    at<unsigned>(object, 1192) = 0;

    unsigned char* light = object + 1208;
    const float lamp = norrath_w9_startup_crt_b_view_4801f8[0];
    light[9] = 0;
    __builtin_memcpy(object + 1256, &kept, 12);
    at<short>(light, 22) = 188;
    at<short>(light, 10) = 188;
    at<short>(light, 12) = 188;
    at<short>(light, 14) = 188;
    at<short>(light, 18) = 188;
    at<short>(light, 20) = 188;
    at<float>(light, 36) = bits_float(0x43960000u);
    at<float>(light, 40) = lamp;
    at<short>(light, 24) = -64;
    at<float>(light, 28) = bits_float(0x43960000u);
    at<float>(light, 32) = lamp;
    at<unsigned>(light, 44) = 0;
    at<short>(light, 16) = -64;
    light[8] = 0;
    lightAdd__FP5Light(light);

    object[1277] = 0;
    object[1276] = 0;
    object[1361] = 1;
    at<unsigned>(object, 1396) = 0;
    selectLumpName = (nmPlayers >= 2 || coreForceSmallCharacters) ? norrath_w9_startup_crt_b_view_464ab8
                                                                    : norrath_w9_startup_crt_b_view_4644c0;
    if (flag) thereIsACutPlayer = 1;
    object[1106] = static_cast<unsigned char>(flag);
    object[40] = 48;
    at<int>(object, 1176) = -1;

    if ((levelFlags & 0x24800000u) == 0) {
        lightDeactivate__FP5Light(light);
    } else {
        at<float>(object, 1236) = bits_float(0x437c0000u);
        at<float>(object, 1240) = norrath_w9_startup_crt_b_view_4801f8[1];
        // The four halves are the light's colour halves written above (188, 188,
        // 188 and -64), all even, so the rounding below never changes a value.
        if (levelFlags & 0x800000u) {
            for (int offset = 1218; offset <= 1224; offset += 2) {
                const int value = at<short>(object, offset);
                at<short>(object, offset) = static_cast<short>((value + (value < 0 ? 1 : 0)) >> 1);
            }
        }
    }

    const signed char* codes = reinterpret_cast<const signed char*>(record + 32);
    char** prefix = &racePrefix[at<int>(record, 84)];
    if (codes[0] == -1) {
        at<unsigned>(object, 1336) = 0;
        at<unsigned>(object, 1352) = 0;
    } else {
        sprintf(buffer, norrath_w9_startup_crt_b_view_464ac8, *prefix, codes[0] + 1);
        store_pointer(object, 1336, lumpFindResource__FPcT0(selectLumpName, buffer));
        sprintf(buffer, norrath_w9_startup_crt_b_view_464ad8, *prefix, codes[0] + 1, codes[1] + 97);
        store_pointer(object, 1352, lumpFindResource__FPcT0(selectLumpName, buffer));
    }
    if (codes[2] == -1) {
        at<unsigned>(object, 1340) = 0;
        at<unsigned>(object, 1348) = 0;
    } else {
        sprintf(buffer, norrath_w9_startup_crt_b_view_464ae8, *prefix, codes[2] + 1);
        store_pointer(object, 1340, lumpFindResource__FPcT0(selectLumpName, buffer));
        sprintf(buffer, norrath_w9_startup_crt_b_view_464af8, *prefix, codes[2] + 1, codes[1] + 97);
        store_pointer(object, 1348, lumpFindResource__FPcT0(selectLumpName, buffer));
    }
    sprintf(buffer, norrath_w9_startup_crt_b_view_464b10, *prefix);
    store_pointer(object, 1332, lumpFindResource__FPcT0(selectLumpName, buffer));
    sprintf(buffer, norrath_w9_startup_crt_b_view_464b20, *prefix, codes[3] + 1);
    store_pointer(object, 1344, lumpFindResource__FPcT0(selectLumpName, buffer));

    at<unsigned>(object, 12) |= 0x1004040u;
    at<int>(object, 960) = index;
    if (object[1106] == 0) {
        scePrintf(norrath_w9_startup_crt_b_view_464b30);
        store_pointer(player + at<int>(object, 960) * 23984, 21872, self);
    }

    at<float>(object, 168) = norrath_w9_startup_crt_b_view_4801f8[2];
    at<float>(object, 28) = bits_float(0x41400000u);
    for (int i = 0; i < 5; ++i) at<unsigned>(object, 104 - 4 * i) = 0;
    for (int i = 0; i < 4; ++i) {
        at<unsigned>(object, 108 + 4 * i) = 0;
        at<unsigned>(object, 124 + 4 * i) = 0;
    }
    for (int i = 0; i < 16; ++i) at<unsigned>(object, 956 - 4 * i) = 0;

    const int slot = at<int>(object, 960);
    at<float>(object, 1120) = norrath_w9_startup_crt_b_view_4801f8[3];
    object[1105] = 1;
    unsigned char* slot_record = player + slot * 23984;
    object[1131] = 0;
    at<short>(object, 1186) = static_cast<short>(slot * 2700 + 3600);
    at<unsigned>(object, 1132) = 0;
    object[964] = 0;
    object[1136] = 0;
    object[1137] = 0;
    object[1138] = 0;
    object[1140] = 0;
    object[1142] = 0;
    at<unsigned>(object, 1100) = 0;
    object[1152] = 0;
    at<short>(object, 1182) = 0;
    object[1107] = 0;
    at<short>(object, 1184) = 0;
    object[1130] = 0;
    object[1141] = 0;
    at<unsigned>(object, 1144) = 0;
    at<int>(object, 1400) = 2;
    at<unsigned>(object, 1096) = 0;
    const int kind = static_cast<signed char>(slot_record[23582]);
    int row = kind * 13;
    animInitState__FP14AnimationState(object + 288);
    resetPlayerAnimations__Fi(at<int>(object, 960));

    at<float>(object, 312) = bits_float(0x3f800000u);
    at<short>(object, 324) = 4613;
    at<float>(object, 304) = bits_float(0x3f800000u);
    unsigned char* table;
    if (levelFlags & 0x10000000u) {
        table = load_pointer(object, 36);
        row = 0;
        store_pointer(object, 296, load_pointer(table, 36));
    } else {
        unsigned char* current = player + at<int>(object, 960) * 23984;
        const int code = static_cast<signed char>(current[23582]);
        const char* name;
        if (code == 1) name = norrath_w9_startup_crt_b_view_464b48;
        else if (code == 2) name = norrath_w9_startup_crt_b_view_464b58;
        else if (code == 3) name = norrath_w9_startup_crt_b_view_464b68;
        else if (code == 4) name = norrath_w9_startup_crt_b_view_464b78;
        else if (code == 7) name = norrath_w9_startup_crt_b_view_464b88;
        else name = norrath_w9_startup_crt_b_view_464b98;
        unsigned char* owner = load_pointer(current, 21872);
        void* result = lumpFindResource__FPcT0(reinterpret_cast<char*>(load_pointer(load_pointer(owner, 36), 0)),
                                               const_cast<char*>(name));
        table = load_pointer(object, 36);
        store_pointer(object, 296, result);
    }
    unsigned char* entry = table + row * 8;
    unsigned entry_76 = at<unsigned>(entry, 76);
    unsigned entry_84 = at<unsigned>(entry, 84);
    unsigned entry_92 = at<unsigned>(entry, 92);
    at<unsigned>(object, 360) = entry_84;
    at<short>(object, 420) = 4613;
    at<unsigned>(object, 392) = entry_92;
    object[292] = 4;
    at<short>(object, 356) = 4613;
    at<short>(object, 388) = 4613;
    at<unsigned>(object, 328) = entry_76;
    at<unsigned>(object, 336) = 0;
    at<unsigned>(object, 344) = 0;
    at<unsigned>(object, 368) = 0;
    at<unsigned>(object, 376) = 0;
    at<unsigned>(object, 400) = 0;
    at<unsigned>(object, 408) = 0;
    objectAddToRunList__FP10GameObject(self);

    for (int k = 0; k < 6; ++k) {
        unsigned char* a = load_pointer(load_pointer(object, 36) + 104 * k, 76);
        a[24] = 1;
        a[32] = 1;
        unsigned char* b = load_pointer(load_pointer(object, 36) + 104 * k, 92);
        b[24] = 4;
        b[32] = 4;
        unsigned char* entry_k = load_pointer(object, 36) + 104 * k;
        unsigned char* pa = load_pointer(entry_k, 76);
        unsigned char* pb = load_pointer(entry_k, 92);
        at<float>(pa, 28) = bits_float(0x3f400000u);
        at<float>(pa, 20) = bits_float(0x3e800000u);
        at<float>(pb, 28) = norrath_w9_startup_crt_b_view_4801f8[5];
        at<float>(pb, 20) = norrath_w9_startup_crt_b_view_4801f8[4];
    }

    Point3 here;
    __builtin_memcpy(&here, object + 16, 12);
    at<float>(object, 24) = worldFindHeight__FP12_worldHeaderG6Point3f(world, here, at<float>(object, 28));
    playerRecomputeAbilityModifiers__Fv();
    for (int i = 0; i < 2; ++i)
        memset(player + at<int>(object, 960) * 23984 + 23584 + 64 * i, 0, 64);
    unsigned char* last = player + at<int>(object, 960) * 23984;
    last[23968] = 0;
    enumerateAlivePlayers__Fv();
}
