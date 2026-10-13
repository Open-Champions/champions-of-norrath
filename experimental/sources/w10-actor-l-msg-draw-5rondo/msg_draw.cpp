#include "../w10-actor-l-common/access.hpp"
// Observed msg_draw__5Rondo at 0x002329a0, 472 bytes. First the Creature draw routine runs with
// the object, the group words at +12 and +20 (group = word at object+36) and a final zero. Then
// a marker is drawn only when all of these hold: the global gameCam is nonzero and the word at
// camera+52 is 1; findTagString(object, "exclamation", 0) is nonzero (it is called twice, the
// second result going to scriptStart); and scriptStart returns exactly 1. The marker position is
// the object's Point3 at +16 with z raised by 72.0, or by 48.0 when findTagString(object,
// "exclamationShort", 0) is nonzero. It is transformed by worldViewMatrix with matTransform; the
// returned w divides the three result components (1.0 / w is multiplied in). The texture
// "exclaim.tex" in the lump "icons.lmp" is drawn with textureDrawColor at x = (int)(result x *
// 0.5) + 293, y = (int)result y + 166, argument 5 and a color whose top byte is
// (int)(isin(frameNm << 10) * 16.0 + 112.0) over the low bytes 0x808080. Nothing is returned.
// Unknown: what the marker means; the meaning of camera+52. Nothing here ran on the EE.
extern "C" {
void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, int extra);
char* findTagString__10GameObjectPci(void* object, char* tag, int index);
int scriptStart__FPc(char* script);
float matTransform__FP6Point3P8Matrix44T0(Point3* out, void* matrix, Point3* in);
char* lumpFindResource__FPcT0(char* lump, char* name);
float isin__Fi(int angle);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color);
extern unsigned char* gameCam;
extern int frameNm;
extern unsigned char worldViewMatrix[];

void msg_draw__5Rondo(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const group = w10l::from_word(w10l::word(o, 36));
    draw__8CreatureP12_modelHeaderP8_textureT1(self, w10l::from_word(w10l::word(group, 12)),
                                               w10l::from_word(w10l::word(group, 20)), 0);
    if (gameCam == nullptr) return;
    if ((w10l::word(gameCam, 52) ^ 1u) != 0) return;
    if (findTagString__10GameObjectPci(self, const_cast<char*>("exclamation"), 0) == nullptr) return;
    if (scriptStart__FPc(findTagString__10GameObjectPci(self, const_cast<char*>("exclamation"), 0)) != 1) return;
    Point3 position = {w10l::real(o, 16), w10l::real(o, 20), w10l::real(o, 24) + 72.0f};
    if (findTagString__10GameObjectPci(self, const_cast<char*>("exclamationShort"), 0) != nullptr)
        position.z = w10l::real(o, 24) + 48.0f;
    Point3 screen;
    const float w = matTransform__FP6Point3P8Matrix44T0(&screen, worldViewMatrix, &position);
    const float scale = 1.0f / w;
    screen.x = screen.x * scale;
    screen.y = screen.y * scale;
    screen.z = screen.z * scale;
    char* const texture = lumpFindResource__FPcT0(const_cast<char*>("icons.lmp"), const_cast<char*>("exclaim.tex"));
    const int x = static_cast<int>(screen.x * 0.5f);
    const int y = static_cast<int>(screen.y);
    const float wave = isin__Fi(frameNm << 10) * 16.0f + 112.0f;
    textureDrawColor__FP8_textureiiiUi(texture, x + 293, y + 166, 5,
                                       (static_cast<unsigned>(static_cast<int>(wave)) << 24) | 0x808080u);
}
}
