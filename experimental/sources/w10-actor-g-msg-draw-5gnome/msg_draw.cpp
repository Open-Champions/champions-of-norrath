// Observed msg_draw__5Gnome at 0x002013d0, 472 bytes.
// Draws through the Creature draw routine with the word at m+12 as the model and the word at m+20
// as the texture (m is the pointer stored at object+36), fourth argument zero. Then, when the
// camera pointed to by the global gameCam is non-null and its word at +52 equals 1:
//  - the tag text "exclamation" is looked up with findTagString__10GameObjectPci(object, text, 0);
//    a null result stops. It is looked up again and the result goes to scriptStart__FPc, which must
//    return 1 or the function stops;
//  - a world position is built from the object's floats at +16, +20 and +24 + 72.0, with +24 +
//    48.0 instead when the tag text "exclamationShort" is found on the object;
//  - matTransform__FP6Point3P8Matrix44T0(out, worldViewMatrix, position) transforms it and returns
//    a float w; out.x, out.y and out.z are each multiplied by 1.0 / w;
//  - the resource "exclaim.tex" in the lump "icons.lmp" is looked up with
//    lumpFindResource__FPcT0 (lump first);
//  - textureDrawColor__FP8_textureiiiUi(texture, (int)(out.x * 0.5) + 293, (int)out.y + 166, 5,
//    color) draws it, where color = (int)(isin__Fi(frameNm << 10) * 16.0 + 112.0) << 24 | 0x808080.
// Inferred: an alert marker over the creature. Unknown: units of the screen coordinates and what
// +52 of the camera means. gameCam, frameNm and worldViewMatrix are unresolved imports. Nothing
// here ran on the EE.
#include "../w10-actor-g-common/words.hpp"

struct Point3 { float x, y, z; };

extern "C" {
extern unsigned gameCam;
extern int frameNm;
extern unsigned char worldViewMatrix[];
int draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* second);
char* findTagString__10GameObjectPci(void* self, char* tag, int occurrence);
int scriptStart__FPc(char* script);
float matTransform__FP6Point3P8Matrix44T0(Point3* out, void* matrix, Point3* in);
void* lumpFindResource__FPcT0(char* lump, char* resource);
float isin__Fi(int angle);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color);
}

extern "C" void msg_draw__5Gnome(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* m = w10g::from_word(w10g::word_at(self, 36));
    draw__8CreatureP12_modelHeaderP8_textureT1(self, w10g::from_word(*reinterpret_cast<unsigned*>(m + 12)),
                                               w10g::from_word(*reinterpret_cast<unsigned*>(m + 20)), nullptr);
    if (gameCam == 0) return;
    if (*reinterpret_cast<unsigned*>(w10g::from_word(gameCam) + 52) != 1u) return;
    char tag[] = "exclamation";
    char shortTag[] = "exclamationShort";
    if (findTagString__10GameObjectPci(self, tag, 0) == nullptr) return;
    if (scriptStart__FPc(findTagString__10GameObjectPci(self, tag, 0)) != 1) return;
    Point3 position;
    position.x = *reinterpret_cast<float*>(o + 16);
    position.y = *reinterpret_cast<float*>(o + 20);
    position.z = *reinterpret_cast<float*>(o + 24) + 72.0f;
    if (findTagString__10GameObjectPci(self, shortTag, 0) != nullptr) position.z = *reinterpret_cast<float*>(o + 24) + 48.0f;
    Point3 out;
    const float w = matTransform__FP6Point3P8Matrix44T0(&out, worldViewMatrix, &position);
    const float scale = 1.0f / w;
    out.x = out.x * scale;
    out.y = out.y * scale;
    out.z = out.z * scale;
    char lump[] = "icons.lmp";
    char resource[] = "exclaim.tex";
    void* texture = lumpFindResource__FPcT0(lump, resource);
    const int x = static_cast<int>(out.x * 0.5f);
    const int y = static_cast<int>(out.y);
    const float wave = isin__Fi(frameNm << 10);
    const int alpha = static_cast<int>(wave * 16.0f + 112.0f);
    textureDrawColor__FP8_textureiiiUi(texture, x + 293, y + 166, 5, (static_cast<unsigned>(alpha) << 24) | 0x808080u);
}
