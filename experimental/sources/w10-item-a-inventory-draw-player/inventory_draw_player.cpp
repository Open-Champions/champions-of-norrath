// Observed inventoryDrawPlayer__FiiP14AnimationStatei at 0x001ad198, 1752 bytes
// (int owner, int mode, AnimationState* anim, int screen). R = player + owner *
// 23984; O = the word at R+21872 (the player's object). Does nothing when owner is
// 5 or more (unsigned) or O is null. Floats named by bit pattern are image
// constants whose exact bits are used.
//   1. pos = {32.0f, -36.0f, 403.0f}; animAdvanceAnimation__FP14AnimationStateP6Point3
//      P6Point2Ps(anim, &pos, 0, 0). With P = inventory[screen].word +52 (record
//      stride 116, base `inventory`), the int at R+23536 gains
//      (int)(netLocalPad[P].float28 * 608.0f) (record stride 88).
//   2. A 3x4 matrix is built into a local by matMakeYPRScale34_ReallySlow__FP8Matrix34
//      G6Point3sssf(matrix, &vec, yaw, -87, -55, scale), with yaw = the low 16 bits
//      of R+23536 minus a constant, by mode:
//        1: vec = {47.0f, 30.0f, -44.0f}, yaw - 16384, scale 1.19f;
//        2: vec = {-43.446f, 30.0f, -44.0f}, yaw - 11384, scale 1.19f;
//        0: owner != 0: vec = {19.0f, 0, 3.0f}, yaw - 14750, scale 0.47f;
//           owner == 0: vec = {-17.0f, 0, 3.0f}, yaw - 11254, scale 0.47f;
//        any other mode: no matrix is built (the local stays as it was).
//   3. modelDraw (the model word O+1332, the texture word O+1344, 4, &pos, matrix,
//      anim, the 64-bit value at O+552 passed as the flags argument, 0, 0) and
//      drawAttachments__6PlayerP11_drawRecordf(O, drawRecord, 1.5f).
//   4. eRandState is advanced with the linear generator x = x * 0x19660d +
//      0x3c6ef35f; r = (x >> 16) as 0..65535. With st = the signed byte R+23582 and
//      cls = the word R+80, animation one-shots (animAddOneShot__FP14AnimationState
//      P15AnimationHeaderffi(anim, header, 0.3f, 0.3f, 0)) are started:
//        st 1: when cls is 0 or 2, the byte animState+4 is 1 and r < 225: header
//              "dhs_idlec.anm" in the lump named by the first word of the structure
//              the word O+36 points to;
//        st 5: if cls == 0: t = charAnimTest; a nonzero t is incremented and stored
//              back; when t is 279 the 16 words R+21896..R+21956 are cleared (t is
//              not reset). Then if t != 0: when the word R+21944 is 0, animState+4 is
//              1 and r < 195, header "nw_idled.anm" in lump "fx.lmp"; if t == 0 and
//              worldWaterBumpSpeed != 0, with pad = netLocalPad[inventory[screen]
//              word +52] word +8: when both bits 0x400 and 0x200 are set,
//              charAnimTest = 1 and header "nw_idlep.anm" in "fx.lmp".
//              For any cls, after that: when the words R+21912, charAnimTest and
//              R+21908 are all 0, animState+4 is 1 and r < 195: "nw_idlec.anm" in the
//              lump named as for st 1.
//        any other st: none.
//   5. From "inventry.lmp": a model "stand.vif" and texture "stand.tex" are drawn:
//      vec = {492.0f, 1465.0f, -491.0f} (x = -494.0f when mode is 2); the matrix is
//      matMakeYPRScale34_ReallySlow(matrix, &vec, 0, 0, t, 14.6168f) with t = 1304
//      when mode is 2 and -820 otherwise, then matRot__FP8Matrix34is(matrix, 2,
//      (short)R+23536); modelDraw(model, texture, 4, &pos, matrix, 0, -1, 0, 0);
//      playerDrawWeaponEffects__6Playerf(O, 1.0f).
// Unknown: what the byte animState+4 is, the names of the player fields at R+80,
// R+21908..21944 and R+23582, charAnimTest and worldWaterBumpSpeed meanings, and
// the 8-byte load at O+552 (only the low word is used by the call).
extern "C" {
extern unsigned char player[];
extern unsigned char inventory[] __attribute__((aligned(4)));
extern unsigned char netLocalPad[];
extern unsigned char animState[];
extern int eRandState;
extern int charAnimTest;
extern int worldWaterBumpSpeed;
void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void*, void*, void*, void*);
void matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf(void*, const void*, short, short, short, float);
void matRot__FP8Matrix34is(void*, int, short);
void* modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void*, void*, int, const void*, void*, void*, unsigned long long, void*, int);
void drawAttachments__6PlayerP11_drawRecordf(void*, void*, float);
void playerDrawWeaponEffects__6Playerf(void*, float);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void*, float, float, int);
void* lumpFindResource__FPcT0(char*, char*);

static inline float bits(unsigned v) { return __builtin_bit_cast(float, v); }
static inline unsigned& word(unsigned char* base, int off) { return *reinterpret_cast<unsigned*>(base + off); }
#ifdef INVENTORY_DRAW_PLAYER_HOST_POINTER
// Host check only: maps a 32-bit word to a host pointer (the image has 32-bit pointers).
extern "C++" void* INVENTORY_DRAW_PLAYER_HOST_POINTER(unsigned);
static inline void* pointer(unsigned w) { return INVENTORY_DRAW_PLAYER_HOST_POINTER(w); }
#else
static inline void* pointer(unsigned w) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(w)); }
#endif

void inventoryDrawPlayer__FiiP14AnimationStatei(int owner, int mode, void* anim, int screen) {
    float pos[3] = {32.0f, -36.0f, 403.0f};
    float vec[3];
    unsigned char matrix[48];
    if (static_cast<unsigned>(owner) >= 5u) return;
    unsigned char* record = player + owner * 23984;
    if (word(record, 21872) == 0) return;
    unsigned char* object = static_cast<unsigned char*>(pointer(word(record, 21872)));
    animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(anim, pos, 0, 0);
    int pad = *reinterpret_cast<int*>(inventory + screen * 116 + 52);
    *reinterpret_cast<int*>(record + 23536) += static_cast<int>(*reinterpret_cast<float*>(netLocalPad + pad * 88 + 28) * 608.0f);
    unsigned long long modelFlags = *reinterpret_cast<unsigned long long*>(object + 552);
    int angle = *reinterpret_cast<int*>(record + 23536);
    if (mode == 1) {
        vec[0] = 47.0f; vec[1] = 30.0f; vec[2] = -44.0f;
        matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf(matrix, vec, static_cast<short>((angle & 0xffff) - 16384), -87, -55, bits(0x3f9851da));
    } else if (mode == 2) {
        vec[0] = bits(0xc22dc8b4); vec[1] = 30.0f; vec[2] = -44.0f;
        matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf(matrix, vec, static_cast<short>((angle & 0xffff) - 11384), -87, -55, bits(0x3f9851da));
    } else if (mode == 0) {
        if (owner == 0) {
            vec[0] = -17.0f; vec[1] = 0.0f; vec[2] = 3.0f;
            matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf(matrix, vec, static_cast<short>((*reinterpret_cast<unsigned short*>(record + 23536)) - 11254), -87, -55, bits(0x3ef0a3f8));
        } else {
            vec[0] = 19.0f; vec[1] = 0.0f; vec[2] = 3.0f;
            matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf(matrix, vec, static_cast<short>((*reinterpret_cast<unsigned short*>(record + 23536)) - 14750), -87, -55, bits(0x3ef0a3d7));
        }
    }
    void* drawRecord = modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        pointer(word(object, 1332)), pointer(word(object, 1344)), 4, pos, matrix, anim, modelFlags, 0, 0);
    drawAttachments__6PlayerP11_drawRecordf(object, drawRecord, 1.5f);
    eRandState = eRandState * 0x19660d + 0x3c6ef35f;
    int r = static_cast<int>(static_cast<unsigned>(eRandState) >> 16);
    r = r % 65536;
    int st = static_cast<signed char>(record[23582]);
    if (st == 1) {
        unsigned cls = word(record, 80);
        if ((cls == 0 || cls == 2) && static_cast<signed char>(animState[4]) == 1 && r < 225) {
            char* lump = static_cast<char*>(pointer(word(static_cast<unsigned char*>(pointer(word(object, 36))), 0)));
            void* header = lumpFindResource__FPcT0(lump, const_cast<char*>("dhs_idlec.anm"));
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim, header, bits(0x3e999999), bits(0x3e999999), 0);
        }
    } else if (st == 5) {
        if (word(record, 80) == 0) {
            int t = charAnimTest;
            if (t != 0) {
                t = t + 1;
                charAnimTest = t;
            }
            if (t == 279) {
                for (int k = 15; k >= 0; k--) word(record, 21956 - 4 * (15 - k)) = 0;
            }
            if (t != 0) {
                if (word(record, 21944) == 0 && static_cast<signed char>(animState[4]) == 1 && r < 195) {
                    void* header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("nw_idled.anm"));
                    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim, header, bits(0x3e999999), bits(0x3e999999), 0);
                }
            } else if (worldWaterBumpSpeed != 0) {
                unsigned flags = word(netLocalPad + *reinterpret_cast<int*>(inventory + screen * 116 + 52) * 88, 8);
                if ((flags & 0x400) != 0 && (flags & 0x200) != 0) {
                    charAnimTest = 1;
                    void* header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("nw_idlep.anm"));
                    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim, header, bits(0x3e999999), bits(0x3e999999), 0);
                }
            }
        }
        if (word(record, 21912) == 0 && charAnimTest == 0 && word(record, 21908) == 0 &&
            static_cast<signed char>(animState[4]) == 1 && r < 195) {
            char* lump = static_cast<char*>(pointer(word(static_cast<unsigned char*>(pointer(word(object, 36))), 0)));
            void* header = lumpFindResource__FPcT0(lump, const_cast<char*>("nw_idlec.anm"));
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim, header, bits(0x3e999999), bits(0x3e999999), 0);
        }
    }
    void* model = lumpFindResource__FPcT0(const_cast<char*>("inventry.lmp"), const_cast<char*>("stand.vif"));
    void* texture = lumpFindResource__FPcT0(const_cast<char*>("inventry.lmp"), const_cast<char*>("stand.tex"));
    vec[0] = 492.0f;
    vec[1] = 1465.0f;
    vec[2] = -491.0f;
    short pitch = -820;
    if (mode == 2) {
        vec[0] = -494.0f;
        pitch = 1304;
    }
    matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf(matrix, vec, 0, 0, pitch, bits(0x4169de50));
    matRot__FP8Matrix34is(matrix, 2, *reinterpret_cast<short*>(record + 23536));
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(model, texture, 4, pos, matrix, 0, 0xffffffffffffffffull, 0, 0);
    playerDrawWeaponEffects__6Playerf(object, 1.0f);
}
}
