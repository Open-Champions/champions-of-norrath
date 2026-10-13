// Observed msg_draw__6Player at 0x00196fc8, 2220 bytes. R is the player record player + word(+960) * 23984,
// idx the word at +960 and pos the three floats at +16. Steps, in order:
//   1. When cutMode is non-zero, the byte at +1106 is zero and thereIsACutPlayer is non-zero: unless
//      engineCurrentLevelName equals "bronzeht", "gnomehub" or "nas" (compared in that order), the byte at
//      +1108 is set to 1 and the function returns.
//   2. When gamePause, gameControlPaused, DLG_Active__Fv(), netGetCameraCustomControl__Fi(idx) and the byte
//      hudNetWaiting are all zero and netPlayerIsLocal__Fi(idx) is non-zero: playerItemScan__6PlayerP3padi(
//      object, null, 1).
//   3. When netGetCameraCustomControl(idx) is non-zero and dot(pos - worldCameraPos, (floats +8, +20, +32 of
//      worldCameraMatrix)) < 84.0: byte +1108 = 1 and return. Otherwise byte +1108 = 0.
//   4. Return when frameNm - 4 < shopActiveFrame and not frameNm + 4 < shopActiveFrame; return when the byte
//      gameHidePlayers is non-zero or gamePlayersPresent is zero.
//   5. maybeDrawMultiPlayerMarkers__6Player(object). An ambient-offset array of four halfwords is zeroed
//      with memset(.., 0, 8). If bit 3 of the word at +12 is set it is passed unchanged to
//      lightSetAmbientOffset__FPs. Otherwise, when the float R[180] is above 0.1: with |R[180]| below 0.8
//      healColor[idx] is lowered by 3 and both healColor[idx] and healPulse[idx] are cleared if it went
//      negative; otherwise healColor[idx] is raised by 3 while below 60. Then healPulse[idx] gains 960,
//      element 0 of the array becomes healColor[idx] + (int)(isin(healPulse[idx]) * 24.0), clamped to zero when
//      negative, and the array is passed to lightSetAmbientOffset. (healColor, healPulse: four halfwords each.)
//   6. If any of the sixteen words at +896.. is non-zero, the four floats at +108.. are converted to
//      halfwords and passed to lightSetAmbientOffset. Then, when the halfword at +1188 (h) is non-zero, the
//      array {h*15/2, h*15/2, h*15, h*15} (the first two by arithmetic shift) goes to lightSetAmbientOffset.
//   7. With shipOn non-zero: a Matrix34 is built with matMakeTransYaw (pos + (0, 0, 5.8), yaw halfword +56)
//      and matRot(matrix, 0, 0); the model and texture are the lump resources "ship.vif" and "ship.tex" of
//      "ship.lmp"; modelDraw(model, texture, 1, pos, matrix, null, all ones, null, 0);
//      modelResetOptionalParams(). Otherwise draw__8CreatureP12_modelHeaderP8_textureT1(object, word +1332,
//      word +1344, null) gives a record; with a = |dot(pos - worldCameraPos, camera axis)| the size
//      argument is 5.0, or 5.0 - (360 - a) * 0.018055... (not below 1.0) when a < 360;
//      drawAttachments__6PlayerP11_drawRecordf(object, record, size).
//   8. lightResetOptionalParams__Fv(). When spellAlpha[idx] is positive: the attachment positions 0 and 1 of
//      the model (modelGetCharAttachmentPos, float 1.0) are each drawn as a P_FaceSprite with size 5.0,
//      8000 + isin(frameNm << 10) * 250, the three words of haloColor + idx * 16, spellAlpha[idx] and
//      haloTexture1.
//   9. With netNetworkMode non-zero, bit 22 of the word at +12 set, netPlayerIsLocal(idx) zero and
//      DLG_Active zero: the point pos + (0, 0, 84) is projected with worldViewMatrix (matTransform; x, y, z are
//      scaled by the reciprocal of the returned float), "icons.lmp"/"busy.tex" is looked up and drawn with
//      textureDrawColor at (int)(x * 0.5) + 293, (int)y + 166, 5 and colour 0x808080 | ((int)(isin(frameNm << 10) *
//      16.0 + 112.0) << 24).
// Inferred from names only; Unknown: the meaning of the halfwords at +1188, +896.. and of the ambient offsets
// beyond the callee names. Nothing here ran on the EE.
#include "../w11-actor-b-common/access.hpp"

extern "C" {
extern signed char cutMode;
extern signed char thereIsACutPlayer;
extern char engineCurrentLevelName[];
extern int gamePause;
extern int gameControlPaused;
extern signed char hudNetWaiting;
extern int frameNm;
extern int shopActiveFrame;
extern signed char gameHidePlayers;
extern int gamePlayersPresent;
extern int shipOn;
extern signed char netNetworkMode;
extern int spellAlpha[];
extern unsigned char haloColor[];
extern unsigned short healColor[];
extern unsigned short healPulse[];
extern void* haloTexture1;
extern unsigned char worldCameraPos[];
extern unsigned char worldCameraMatrix[];
extern unsigned char worldViewMatrix[];
extern unsigned char player[];
int compare_text(const char* a, const char* b) __asm__("strcmp");
void* clear_bytes(void* destination, int value, unsigned count) __asm__("memset");
int DLG_Active__Fv();
int netGetCameraCustomControl__Fi(int index);
int netPlayerIsLocal__Fi(int index);
void playerItemScan__6PlayerP3padi(void* self, void* pad, int flag);
void maybeDrawMultiPlayerMarkers__6Player(void* self);
void lightSetAmbientOffset__FPs(short* offsets);
void lightResetOptionalParams__Fv();
void modelResetOptionalParams__Fv();
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, Point3 position, short yaw);
void matRot__FP8Matrix34is(void* matrix, int axis, short angle);
char* lumpFindResource__FPcT0(char* lump, char* name);
int modelDraw(void* model, void* texture, int mode, Point3 point, void* matrix, void* animation, unsigned long long flags,
              void* record, int last) __asm__("modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi");
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra);
void drawAttachments__6PlayerP11_drawRecordf(void* self, void* record, float size);
float isin__Fi(int angle);
void modelGetCharAttachmentPos(void* model, Point3 position, short angle, void* animation, int which, Point3* out, Point3* out2, float scale)
    __asm__("modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f");
void P_FaceSprite(const Point3* position, float size, float depth, int r, int g, int b, int a, void* texture, short flags, int mode)
    __asm__("P_FaceSprite__FG6Point3ffiiiiP8_texturesi");
float matTransform__FP6Point3P8Matrix44T0(Point3* out, void* matrix, Point3* in);
void textureDrawColor(void* texture, int x, int y, int z, unsigned color) __asm__("textureDrawColor__FP8_textureiiiUi");
}

namespace {
inline float dot3(const Point3& a, float x, float y, float z) { return a.x * x + a.y * y + a.z * z; }
}

extern "C" void msg_draw__6Player(void* self) {
    using namespace w11b;
    unsigned char* o = static_cast<unsigned char*>(self);
    const int idx = sword(o, 960);
    unsigned char* record = player + idx * 23984;
    if (cutMode != 0 && *reinterpret_cast<signed char*>(o + 1106) == 0 && thereIsACutPlayer != 0) {
        if (compare_text(engineCurrentLevelName, "bronzeht") != 0 && compare_text(engineCurrentLevelName, "gnomehub") != 0 &&
            compare_text(engineCurrentLevelName, "nas") != 0) {
            *(o + 1108) = 1;
            return;
        }
    }
    if (gamePause == 0 && gameControlPaused == 0 && DLG_Active__Fv() == 0 && netGetCameraCustomControl__Fi(sword(o, 960)) == 0 &&
        hudNetWaiting == 0 && netPlayerIsLocal__Fi(sword(o, 960)) != 0)
        playerItemScan__6PlayerP3padi(o, nullptr, 1);
    if (netGetCameraCustomControl__Fi(sword(o, 960)) != 0) {
        const float dx = real(o, 16) - *reinterpret_cast<float*>(worldCameraPos);
        const float dy = real(o, 20) - *reinterpret_cast<float*>(worldCameraPos + 4);
        const float dz = real(o, 24) - *reinterpret_cast<float*>(worldCameraPos + 8);
        const float dot = dx * *reinterpret_cast<float*>(worldCameraMatrix + 8) + dy * *reinterpret_cast<float*>(worldCameraMatrix + 20) +
                          dz * *reinterpret_cast<float*>(worldCameraMatrix + 32);
        if (dot < 84.0f) {
            *(o + 1108) = 1;
            return;
        }
    }
    *(o + 1108) = 0;
    if (frameNm - 4 < shopActiveFrame && !(frameNm + 4 < shopActiveFrame)) return;
    if (gameHidePlayers != 0) return;
    if (gamePlayersPresent == 0) return;
    maybeDrawMultiPlayerMarkers__6Player(o);
    short ambient[4];
    clear_bytes(ambient, 0, 8);
    if ((word(o, 12) & 8u) != 0) {
        ambient[0] = 0;
        lightSetAmbientOffset__FPs(ambient);
    } else if (real(record, 180) > bits(0x3dcccccc)) {
        const float level = real(record, 180);
        const int index = sword(o, 960);
        if (__builtin_fabsf(level) < bits(0x3f4ccccc)) {
            healColor[index] = static_cast<unsigned short>(healColor[index] - 3);
            if (static_cast<short>(healColor[index]) < 0) {
                healColor[index] = 0;
                healPulse[index] = 0;
            }
        } else if (static_cast<short>(healColor[index]) < 60) {
            healColor[index] = static_cast<unsigned short>(healColor[index] + 3);
        }
        healPulse[index] = static_cast<unsigned short>(healPulse[index] + 960);
        const int sine = static_cast<int>(isin__Fi(static_cast<short>(healPulse[index])) * 24.0f);
        const unsigned short value = static_cast<unsigned short>(healColor[index] + sine);
        ambient[0] = static_cast<short>(value);
        if (static_cast<short>(value) < 0) ambient[0] = 0;
        lightSetAmbientOffset__FPs(ambient);
    }
    for (int i = 0; i < 16; ++i) {
        if (word(o, 896 + 4 * i) != 0) {
            short offsets[4];
            for (int k = 0; k < 4; ++k) offsets[k] = static_cast<short>(static_cast<int>(real(o, 108 + 4 * k)));
            lightSetAmbientOffset__FPs(offsets);
            break;
        }
    }
    const int h = half(o, 1188);
    if (h != 0) {
        const int v = h * 15;
        ambient[3] = static_cast<short>(v);
        ambient[2] = static_cast<short>(v);
        ambient[1] = static_cast<short>(static_cast<short>(v) >> 1);
        ambient[0] = static_cast<short>(static_cast<short>(v) >> 1);
        lightSetAmbientOffset__FPs(ambient);
    }
    if (shipOn != 0) {
        unsigned char matrix[48] __attribute__((aligned(16)));
        Point3 at;
        at.x = real(o, 16) + 0.0f;
        at.y = real(o, 20);
        at.z = real(o, 24) + bits(0x40b99999);
        matMakeTransYaw__FP8Matrix34G6Point3s(matrix, at, half(o, 56));
        matRot__FP8Matrix34is(matrix, 0, 0);
        void* model = lumpFindResource__FPcT0(const_cast<char*>("ship.lmp"), const_cast<char*>("ship.vif"));
        void* texture = lumpFindResource__FPcT0(const_cast<char*>("ship.lmp"), const_cast<char*>("ship.tex"));
        modelDraw(model, texture, 1, *reinterpret_cast<Point3*>(o + 16), matrix, nullptr, ~0ull, nullptr, 0);
        modelResetOptionalParams__Fv();
    } else {
        void* drawn = draw__8CreatureP12_modelHeaderP8_textureT1(o, from_word(word(o, 1332)), from_word(word(o, 1344)), nullptr);
        const float dx = real(o, 16) - *reinterpret_cast<float*>(worldCameraPos);
        const float dy = real(o, 20) - *reinterpret_cast<float*>(worldCameraPos + 4);
        const float dz = real(o, 24) - *reinterpret_cast<float*>(worldCameraPos + 8);
        float a = dx * *reinterpret_cast<float*>(worldCameraMatrix + 8) + dy * *reinterpret_cast<float*>(worldCameraMatrix + 20) +
                  dz * *reinterpret_cast<float*>(worldCameraMatrix + 32);
        a = __builtin_fabsf(a);
        float size = 5.0f;
        if (a < 360.0f) {
            size = 5.0f - (360.0f - a) * bits(0x3c93e93e);
            if (size < 1.0f) size = 1.0f;
        }
        drawAttachments__6PlayerP11_drawRecordf(o, drawn, size);
    }
    lightResetOptionalParams__Fv();
    if (spellAlpha[sword(o, 960)] > 0) {
        Point3 first;
        Point3 second;
        modelGetCharAttachmentPos(from_word(word(o, 1332)), *reinterpret_cast<Point3*>(o + 16), half(o, 56), o + 288, 0, &first, nullptr, 1.0f);
        modelGetCharAttachmentPos(from_word(word(o, 1332)), *reinterpret_cast<Point3*>(o + 16), half(o, 56), o + 288, 1, &second, nullptr, 1.0f);
        unsigned char* halo = haloColor + sword(o, 960) * 16;
        P_FaceSprite(&first, 5.0f, isin__Fi(frameNm << 10) * 250.0f + 8000.0f, sword(halo, 0), sword(halo, 4), sword(halo, 8),
                     spellAlpha[sword(o, 960)], haloTexture1, 0, 2);
        P_FaceSprite(&second, 5.0f, isin__Fi(frameNm << 10) * 250.0f + 8000.0f, sword(halo, 0), sword(halo, 4), sword(halo, 8),
                     spellAlpha[sword(o, 960)], haloTexture1, 0, 2);
    }
    if (netNetworkMode == 0) return;
    if ((word(o, 12) & 0x400000u) == 0) return;
    if (netPlayerIsLocal__Fi(sword(o, 960)) != 0) return;
    if (DLG_Active__Fv() != 0) return;
    Point3 raised;
    raised.x = real(o, 16);
    raised.y = real(o, 20);
    raised.z = real(o, 24) + 84.0f;
    Point3 view;
    const float w = matTransform__FP6Point3P8Matrix44T0(&view, worldViewMatrix, &raised);
    const float reciprocal = 1.0f / w;
    view.x = view.x * reciprocal;
    view.y = view.y * reciprocal;
    view.z = view.z * reciprocal;
    void* icon = lumpFindResource__FPcT0(const_cast<char*>("icons.lmp"), const_cast<char*>("busy.tex"));
    const int x = static_cast<int>(view.x * 0.5f);
    const int y = static_cast<int>(view.y);
    const float pulse = isin__Fi(frameNm << 10) * 16.0f + 112.0f;
    textureDrawColor(icon, x + 293, y + 166, 5, (static_cast<unsigned>(static_cast<int>(pulse)) << 24) | 0x808080u);
}
