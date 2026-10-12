#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_draw__14GnomeNavigator at 0x201df8, 472 bytes.
// draw__8Creature gets the object, the words at +12 and +20 of the group at
// object+36, and a null fourth argument. The method then returns unless the
// word gameCam (0x484f50) is non-null and the word at gameCam+52 is 1, and
// findTagString for the tag "exclamation" (occurrence 0) is non-null. The tag
// is looked up a second time and its result passed to scriptStart, which must
// return 1 to continue. A point {x, y, z + 72.0} is built from the singles at
// +16, +20 and +24; when findTagString for "exclamationShort" is non-null the
// z becomes z + 48.0. matTransform of that point by worldViewMatrix
// (0x37ef30) writes a result point and returns a single w; the first two result
// components are multiplied by 1.0 / w (the reference also scales the third;
// nothing reads it afterwards). lumpFindResource("icons.lmp",
// "exclaim.tex") returns the texture. The x component times 0.5, truncated,
// plus 293, and the y component truncated, plus 166, are the screen position.
// The alpha is the truncation of isin(frameNm << 10) * 16.0 + 112.0 shifted
// left 24 and ORed with 0x808080. textureDrawColor gets the texture, x, y, 5
// and that colour.
// Observed: the tag strings are at 0x46f340 and 0x46f350, the lump and
// resource names at 0x46f368 and 0x46f378. Inferred: matTransform writes its
// first argument from its third. Unknown: what the marker means, and the
// roles of w, gameCam+52 and the scriptStart return.
extern "C" void* gameCam;
extern "C" int frameNm;
extern "C" unsigned char worldViewMatrix[];
extern "C" void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra);
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence);
extern "C" int scriptStart__FPc(char* name);
extern "C" float matTransform__FP6Point3P8Matrix44T0(void* result, void* matrix, const void* point);
extern "C" char* lumpFindResource__FPcT0(char* lump_name, char* resource_name);
extern "C" float isin__Fi(int angle);
extern "C" void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color);

extern "C" void msg_draw__14GnomeNavigator(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    static char exclamation[] = {'e', 'x', 'c', 'l', 'a', 'm', 'a', 't', 'i', 'o', 'n', '\0'};
    static char exclamation_short[] = {'e', 'x', 'c', 'l', 'a', 'm', 'a', 't', 'i', 'o', 'n', 'S', 'h', 'o', 'r', 't', '\0'};
    static char lump_name[] = {'i', 'c', 'o', 'n', 's', '.', 'l', 'm', 'p', '\0'};
    static char resource_name[] = {'e', 'x', 'c', 'l', 'a', 'i', 'm', '.', 't', 'e', 'x', '\0'};
    const Address group = address(pointer(read_word(base + 36u)));
    draw__8CreatureP12_modelHeaderP8_textureT1(self, pointer(read_word(group + 12u)), pointer(read_word(group + 20u)), nullptr);
    if (gameCam == nullptr) return;
    if (read_word(address(gameCam) + 52u) != 1u) return;
    if (norrath_actor_find_tag_string(self, exclamation, 0) == nullptr) return;
    if (scriptStart__FPc(norrath_actor_find_tag_string(self, exclamation, 0)) != 1) return;
    float point[3];
    float result[3];
    point[0] = read_single(base + 16u);
    point[1] = read_single(base + 20u);
    point[2] = read_single(base + 24u) + 72.0f;
    if (norrath_actor_find_tag_string(self, exclamation_short, 0) != nullptr) point[2] = read_single(base + 24u) + 48.0f;
    const float w = matTransform__FP6Point3P8Matrix44T0(result, worldViewMatrix, point);
    const float scale = 1.0f / w;
    result[0] = result[0] * scale;
    result[1] = result[1] * scale;
    void* const texture = lumpFindResource__FPcT0(lump_name, resource_name);
    const int y = static_cast<int>(result[1]) + 166;
    const int x = static_cast<int>(result[0] * 0.5f) + 293;
    const float level = isin__Fi(static_cast<int>(static_cast<unsigned>(frameNm) << 10)) * 16.0f + 112.0f;
    textureDrawColor__FP8_textureiiiUi(texture, x, y, 5, (static_cast<unsigned>(static_cast<int>(level)) << 24) | 0x808080u);
}
