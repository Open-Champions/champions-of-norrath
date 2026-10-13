// Observed msg_draw__3NPC at 0x00231ac0, 1584 bytes. Argument: the object o. Nothing is returned. Names: pos =
// floats at o+16..o+24; name = the string at o+960; model = the word at +1056 and texture = the word at +1060
// (both resources, kept as locals that the image refreshes below); cam = worldCameraPos (three floats) and V =
// worldViewMatrix (sixteen floats). Strings and numbers are the image's.
//  1. delta = pos - cam. When (delta . (V[2], V[6], V[10])) is above 0.0 the function returns. It also returns when
//     playerSelectionRunning() is nonzero and playerSelectionNPCVisible(o) is zero.
//  2. limit = 240.0, or 300.0 when cutMode is nonzero. dist = |delta|. When dist <= limit, the 64-bit word at +552
//     becomes modelGetActiveChunks(word at +1056, word at +244). Otherwise (far):
//     a. When the halfword at +1066 is negative: model = lumpFindResource(name, sprintf("low%s", o+992)),
//        texture = lumpFindResource(name, sprintf("low%s", o+1024)), and +552 becomes
//        modelGetActiveChunks(model, +244) (the sprintf buffer is shared and filled before each lump call).
//     b. When gameCam is non-null with its word at +52 equal to 1, findTagString(o, "exclamation", 0) is non-null
//        and scriptStart(that string) returns 1: the point Q = (pos.x, pos.y, pos.z + 72.0) (+ 48.0 instead when
//        findTagString(o, "exclamationShort", 0) is non-null) is transformed by matTransform(&p, V, &Q), which
//        returns w; p is scaled by 1/w; with tex = lumpFindResource("icons.lmp", "exclaim.tex"), _textureDrawColor(tex,
//        (int)(p.x * 0.5) + 293, (int)p.y + 166, 5, ((int)(isin(frameNm << 10) * 16.0 + 112.0) << 24) | 0x808080).
//  3. When cutMode is zero and strcasecmp(o+1068, "firlow") is zero, model, texture and +552 are refreshed as in 2a
//     (buffer separate).
//  4. When name contains "select", +552 becomes playerSelectionGetNPCActiveChunks(o).
//  5. rec = draw__8Creature(o, model, texture, null).
//  6. Name "we_king.lmp": noise = (int)(ef1Noise((o address) * 0x19660d + (frameNm << 4) + 0x7efe41) * 500.0);
//     modelGetCharAttachmentPos(model, pos, halfword +56, o+288, 0, &out, &second, float at +224) fills out, then
//     P_FaceSprite(out, 5.0, (float)(noise + 4000), 84, 101, 46, 94, haloTexture2, 0, 2).
//  7. When name contains "select": playerSelectionNPCDraw(o, model, rec).
//  8. Name "superorc.lmp" (draws "cleever.vif"/"cleever.tex") and name "pelyshor.lmp" (draws "thehorn.vif"/
//     "thehorn.tex"): each copies the two names through sprintf into a buffer, looks each up with
//     lumpFindResource(name, buffer) and calls modelDrawAttachment(first, second, 0, model, rec, ~0, -1, 0).
// Inferred: this is the draw handler of NPC, from the symbol; the exclamation marker is a quest/dialog hint.
// Unknown: what +1066, +1068, gameCam+52 and the numbers in P_FaceSprite mean. Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
extern float worldCameraPos[3];
extern float worldViewMatrix[16];
extern signed char cutMode;
extern int frameNm;
extern void* gameCam;
extern void* haloTexture2;
int libc_sprintf(char* buffer, const char* format, ...) __asm__("sprintf");
int libc_strcmp(const char* a, const char* b) __asm__("strcmp");
int libc_strcasecmp(const char* a, const char* b) __asm__("strcasecmp");
char* libc_strstr(const char* a, const char* b) __asm__("strstr");
int playerSelectionRunning__Fv();
int playerSelectionNPCVisible__FP3NPC(void* npc);
unsigned long long playerSelectionGetNPCActiveChunks__FP3NPC(void* npc);
void playerSelectionNPCDraw__FP3NPCP12_modelHeaderP11_drawRecord(void* npc, void* model, void* record);
char* findTagString__10GameObjectPci(void* self, const char* tag, int index);
int scriptStart__FPc(char* script);
float matTransform__FP6Point3P8Matrix44T0(Point3* out, void* matrix, Point3* in);
void* lumpFindResource__FPcT0(char* lump, char* resource);
void _textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color);
float isin__Fi(int angle);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int bits);
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra);
float ef1Noise__Fi(int seed);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 position, short a, void* animation, int b, Point3* first, Point3* second, float f);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 position, float a, float b, int c0, int c1, int c2, int alpha, void* texture, short angle, int mode);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* a, void* b, int c, void* d, void* record, unsigned long long flags, int e, int f);

void msg_draw__3NPC(void* self) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    char* const name = reinterpret_cast<char*>(o + 960);
    void* model = from_word(word(o, 1056));
    void* texture = from_word(word(o, 1060));
    Point3 delta;
    delta.x = real(o, 16) - worldCameraPos[0];
    delta.y = real(o, 20) - worldCameraPos[1];
    delta.z = real(o, 24) - worldCameraPos[2];
    const float dot = delta.x * worldViewMatrix[2] + delta.y * worldViewMatrix[6] + delta.z * worldViewMatrix[10];
    if (0.0f < dot) return;
    if (playerSelectionRunning__Fv() != 0 && playerSelectionNPCVisible__FP3NPC(self) == 0) return;
    float limit = bits(0x43700000u);
    if (cutMode != 0) limit = bits(0x43960000u);
    const float dx = real(o, 16) - worldCameraPos[0];
    const float dy = real(o, 20) - worldCameraPos[1];
    const float dz = real(o, 24) - worldCameraPos[2];
    const float distance = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    char buffer[128];
    if (limit < distance) {
        if (half(o, 1066) < 0) {
            libc_sprintf(buffer, "low%s", reinterpret_cast<char*>(o + 992));
            model = lumpFindResource__FPcT0(name, buffer);
            libc_sprintf(buffer, "low%s", reinterpret_cast<char*>(o + 1024));
            texture = lumpFindResource__FPcT0(name, buffer);
            dword(o, 552) = modelGetActiveChunks__FP12_modelHeaderi(model, sword(o, 244));
        }
        if (gameCam != nullptr && word(static_cast<unsigned char*>(gameCam), 52) == 1u && findTagString__10GameObjectPci(self, "exclamation", 0) != nullptr &&
            scriptStart__FPc(findTagString__10GameObjectPci(self, "exclamation", 0)) == 1) {
            Point3 q;
            q.x = real(o, 16);
            q.y = real(o, 20);
            q.z = real(o, 24) + bits(0x42900000u);
            if (findTagString__10GameObjectPci(self, "exclamationShort", 0) != nullptr) q.z = real(o, 24) + bits(0x42400000u);
            Point3 p;
            const float w = matTransform__FP6Point3P8Matrix44T0(&p, worldViewMatrix, &q);
            const float inverse = 1.0f / w;
            p.x = p.x * inverse;
            p.y = p.y * inverse;
            p.z = p.z * inverse;
            void* const icon = lumpFindResource__FPcT0(const_cast<char*>("icons.lmp"), const_cast<char*>("exclaim.tex"));
            const int x = static_cast<int>(p.x * bits(0x3f000000u));
            const int y = static_cast<int>(p.y);
            const float pulse = isin__Fi(frameNm << 10);
            _textureDrawColor__FP8_textureiiiUi(icon, x + 293, y + 166, 5, (static_cast<unsigned>(static_cast<int>(pulse * bits(0x41800000u) + bits(0x42e00000u))) << 24) | 0x808080u);
        }
    } else {
        dword(o, 552) = modelGetActiveChunks__FP12_modelHeaderi(from_word(word(o, 1056)), sword(o, 244));
    }
    if (cutMode == 0 && libc_strcasecmp(reinterpret_cast<char*>(o + 1068), "firlow") == 0) {
        libc_sprintf(buffer, "low%s", reinterpret_cast<char*>(o + 992));
        model = lumpFindResource__FPcT0(name, buffer);
        libc_sprintf(buffer, "low%s", reinterpret_cast<char*>(o + 1024));
        texture = lumpFindResource__FPcT0(name, buffer);
        dword(o, 552) = modelGetActiveChunks__FP12_modelHeaderi(model, sword(o, 244));
    }
    if (libc_strstr(name, "select") != nullptr) dword(o, 552) = playerSelectionGetNPCActiveChunks__FP3NPC(self);
    void* const record = draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    if (libc_strcmp(name, "we_king.lmp") == 0) {
        const int noise = static_cast<int>(ef1Noise__Fi(static_cast<int>(to_word(self) * 0x19660du + static_cast<unsigned>(frameNm << 4) + 0x7efe41u)) * bits(0x43fa0000u));
        Point3 out, second;
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(model, *reinterpret_cast<Point3*>(o + 16), half(o, 56), o + 288, 0, &out, &second, real(o, 224));
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(out, bits(0x40a00000u), static_cast<float>(noise + 4000), 84, 101, 46, 94, haloTexture2, 0, 2);
    }
    if (libc_strstr(name, "select") != nullptr) playerSelectionNPCDraw__FP3NPCP12_modelHeaderP11_drawRecord(self, model, record);
    if (libc_strcmp(name, "superorc.lmp") == 0) {
        libc_sprintf(buffer, "cleever.vif");
        void* const first = lumpFindResource__FPcT0(name, buffer);
        libc_sprintf(buffer, "cleever.tex");
        void* const second = lumpFindResource__FPcT0(name, buffer);
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(first, second, 0, model, record, ~0ull, -1, 0);
    }
    if (libc_strcmp(name, "pelyshor.lmp") == 0) {
        libc_sprintf(buffer, "thehorn.vif");
        void* const first = lumpFindResource__FPcT0(name, buffer);
        libc_sprintf(buffer, "thehorn.tex");
        void* const second = lumpFindResource__FPcT0(name, buffer);
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(first, second, 0, model, record, ~0ull, -1, 0);
    }
}
}
