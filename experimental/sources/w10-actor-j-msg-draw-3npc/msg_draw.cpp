// Observed msg_draw__3NPC at 0x00231ac0, 1584 bytes. Offsets are bytes of the object; M is the matrix
// worldViewMatrix (0x0037ef30) and C the point worldCameraPos (0x0037ee80). "lump" is the text at +960,
// "model name" and "texture name" the texts at +992 and +1024, "name" the text at +1068. model is the
// word +1056 and texture the word +1060 (both can be replaced below).
//  1. Nothing is drawn (the call ends) when the dot product of (position +16 minus C) with (M[2], M[6],
//     M[10]) is above 0, or when playerSelectionRunning() is non-zero and playerSelectionNPCVisible(this)
//     is zero.
//  2. Let limit be 240, or 300 when the byte cutMode (0x00484318) is non-zero, and dist the distance
//     between the position and C. When dist is at most limit: the 64-bit modelGetActiveChunks(word +1056,
//     word +244) goes to +552. Otherwise: when the signed halfword +1066 is negative, model and texture are
//     looked up again by name (sprintf of "low%s" with the model name, then with the texture name, each
//     passed to lumpFindResource with lump) and the chunks are recomputed with the new model; then, when
//     gameCam (0x00484f50) is non-null and its word +52 is 1 and scriptStart(tag "exclamation") returns 1,
//     the position raised by 72 (48 when the tag "exclamationShort" also exists) is transformed with M,
//     divided by its w, and a small icon (resource "exclaim.tex" of "icons.lmp") is drawn with
//     _textureDrawColor at ((int)(x*0.5)+293, (int)y+166), size 5, colour 0x808080 plus an alpha byte
//     (int)(isin(frameNm << 10) * 16 + 112) << 24.
//  3. When cutMode is zero and name equals "firlow" (case-insensitive), model, texture and chunks are
//     looked up again as in 2.
//  4. When lump contains "select", the +552 chunks come from playerSelectionGetNPCActiveChunks(this).
//     draw(this, model, texture, 0) draws and returns a record.
//  5. When lump equals "we_king.lmp": ef1Noise(0x007efe41 + this*1664525 + (frameNm << 4)) * 500 is taken as
//     an integer n, modelGetCharAttachmentPos fills two points from model, the position, halfword +56,
//     float +224 and the animation state at +288, and P_FaceSprite(first point, 5.0, (float)(n + 4000), 84,
//     101, 46, 94, haloTexture2, 0, 2) draws a sprite.
//  6. When lump contains "select": playerSelectionNPCDraw(this, model, record).
//  7. When lump equals "superorc.lmp", the attachment ("cleever.vif" model, "cleever.tex" texture, both
//     from that lump) is drawn with modelDrawAttachment(attachment model, texture, 0, model, record, -1,
//     -1, 0); for "pelyshor.lmp" the same with "thehorn.vif" and "thehorn.tex".
// The meaning of the checks is unknown. Nothing here ran on the EE.
#include "../w10-actor-j-common/access.hpp"

extern "C" {
extern float worldCameraPos[];
extern float worldViewMatrix[];
extern signed char cutMode;
extern unsigned char* gameCam;
extern int frameNm;
extern void* haloTexture2;
int playerSelectionRunning__Fv();
int playerSelectionNPCVisible__FP3NPC(void* npc);
unsigned long long playerSelectionGetNPCActiveChunks__FP3NPC(void* npc);
void playerSelectionNPCDraw__FP3NPCP12_modelHeaderP11_drawRecord(void* npc, void* model, void* record);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int mask);
void* findTagString__10GameObjectPci(void* object, const char* tag, int index);
int scriptStart__FPc(const char* name);
float matTransform__FP6Point3P8Matrix44T0(Point3* out, void* matrix, Point3* in);
void* lumpFindResource__FPcT0(char* lump, char* name);
float isin__Fi(int angle);
void _textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int size, unsigned color);
float ef1Noise__Fi(int value);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 position, short a, void* animation, int b, Point3* first, Point3* second, float f);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 position, float a, float b, int c, int d, int e, int f, void* texture, short g, int h);
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* texture2);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* model, void* texture, int a, void* model2, void* record,
                                                                         unsigned long b, int c, int d);
int sprintf(char* to, const char* format, ...);
int strcmp(const char* a, const char* b);
int strcasecmp(const char* a, const char* b);
char* strstr(const char* haystack, const char* needle);
}

namespace {
__attribute__((always_inline)) inline float square_root(float value) {
#if defined(__mips__)
    float result;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(result) : "f"(value));
    return result;
#else
    return __builtin_sqrtf(value);
#endif
}

// Looks up the model and texture named by the object's two texts through the "low%s" format.
void reload(unsigned char* o, void*& model, void*& texture) {
    using namespace w10j;
    char buffer[96];
    char* const lump = reinterpret_cast<char*>(o + 960);
    sprintf(buffer, "low%s", reinterpret_cast<char*>(o + 992));
    model = lumpFindResource__FPcT0(lump, buffer);
    sprintf(buffer, "low%s", reinterpret_cast<char*>(o + 1024));
    texture = lumpFindResource__FPcT0(lump, buffer);
    dword(o, 552) = modelGetActiveChunks__FP12_modelHeaderi(model, static_cast<int>(word(o, 244)));
}

void draw_attachment(unsigned char* o, const char* modelText, const char* textureText, void* model, void* record) {
    char buffer[96];
    char* const lump = reinterpret_cast<char*>(o + 960);
    sprintf(buffer, modelText);
    void* const attachmentModel = lumpFindResource__FPcT0(lump, buffer);
    sprintf(buffer, textureText);
    void* const attachmentTexture = lumpFindResource__FPcT0(lump, buffer);
    modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(attachmentModel, attachmentTexture, 0, model, record,
                                                                         static_cast<unsigned long>(-1), -1, 0);
}
}  // namespace

extern "C" void msg_draw__3NPC(void* self) {
    using namespace w10j;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const float dx = real(o, 16) - worldCameraPos[0];
    const float dy = real(o, 20) - worldCameraPos[1];
    const float dz = real(o, 24) - worldCameraPos[2];
    void* model = from_word(word(o, 1056));
    void* texture = from_word(word(o, 1060));
    if (0.0f < dx * worldViewMatrix[2] + dy * worldViewMatrix[6] + dz * worldViewMatrix[10]) return;
    if (playerSelectionRunning__Fv() != 0 && playerSelectionNPCVisible__FP3NPC(self) == 0) return;
    const float limit = cutMode != 0 ? 300.0f : 240.0f;
    const float ex = real(o, 16) - worldCameraPos[0];
    const float ey = real(o, 20) - worldCameraPos[1];
    const float ez = real(o, 24) - worldCameraPos[2];
    char* const lump = reinterpret_cast<char*>(o + 960);
    if (!(limit < square_root(ex * ex + ey * ey + ez * ez))) {
        dword(o, 552) = modelGetActiveChunks__FP12_modelHeaderi(from_word(word(o, 1056)), static_cast<int>(word(o, 244)));
    } else {
        if (half(o, 1066) < 0) reload(o, model, texture);
        if (gameCam != nullptr && word(gameCam, 52) == 1u) {
            const char* const tag = "exclamation";
            if (findTagString__10GameObjectPci(self, tag, 0) != nullptr && scriptStart__FPc(static_cast<const char*>(findTagString__10GameObjectPci(self, tag, 0))) == 1) {
                Point3 raised = {real(o, 16), real(o, 20), real(o, 24) + 72.0f};
                if (findTagString__10GameObjectPci(self, "exclamationShort", 0) != nullptr) raised.z = real(o, 24) + 48.0f;
                Point3 view;
                const float w = matTransform__FP6Point3P8Matrix44T0(&view, worldViewMatrix, &raised);
                const float inverse = 1.0f / w;
                view.x = view.x * inverse;
                view.y = view.y * inverse;
                view.z = view.z * inverse;
                void* const icon = lumpFindResource__FPcT0(const_cast<char*>("icons.lmp"), const_cast<char*>("exclaim.tex"));
                const int x = static_cast<int>(view.x * 0.5f);
                const int y = static_cast<int>(view.y);
                const float pulse = isin__Fi(frameNm << 10);
                const int alpha = static_cast<int>(pulse * 16.0f + 112.0f);
                _textureDrawColor__FP8_textureiiiUi(icon, x + 293, y + 166, 5, (static_cast<unsigned>(alpha) << 24) | 0x808080u);
            }
        }
    }
    if (cutMode == 0 && strcasecmp(reinterpret_cast<const char*>(o + 1068), "firlow") == 0) reload(o, model, texture);
    if (strstr(lump, "select") != nullptr) dword(o, 552) = playerSelectionGetNPCActiveChunks__FP3NPC(self);
    void* const record = draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    if (strcmp(lump, "we_king.lmp") == 0) {
        const int noise = static_cast<int>(ef1Noise__Fi(static_cast<int>(0x7efe41u + to_word(self) * 1664525u + (static_cast<unsigned>(frameNm) << 4))) * 500.0f);
        Point3 first, second;
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
            model, *reinterpret_cast<Point3*>(o + 16), half(o, 56), o + 288, 0, &first, &second, real(o, 224));
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(first, 5.0f, static_cast<float>(noise + 4000), 84, 101, 46, 94, haloTexture2, 0, 2);
    }
    if (strstr(lump, "select") != nullptr) playerSelectionNPCDraw__FP3NPCP12_modelHeaderP11_drawRecord(self, model, record);
    if (strcmp(lump, "superorc.lmp") == 0) draw_attachment(o, "cleever.vif", "cleever.tex", model, record);
    if (strcmp(lump, "pelyshor.lmp") == 0) draw_attachment(o, "thehorn.vif", "thehorn.tex", model, record);
}
