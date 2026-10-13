// Host contract for getHairAndSkin__Fi. Not an EE plan input.
#include "get_hair_and_skin.cpp"
#include <cstdio>
#include <cstring>
#include <string>
Target targets[2];
const char* npcLumpName = "npc";
static std::string found[64];
static int foundCount;
static std::string trace;
extern "C" {
int sprintf(char* out, const char* f, ...);
}
static bool has(const char* s) {
    for (int i = 0; i < foundCount; ++i) if (found[i] == s) return true;
    return false;
}
static void add(const char* s) { found[foundCount++] = s; }
extern "C" void* lumpFindResource__FPcT0(const char* lump, char* text) {
    if (std::strcmp(lump, "npc") != 0) return 0;
    trace += std::string(text) + ";";
    return has(text) ? (void*)(0x1000 + std::strlen(text)) : 0;
}
extern "C" int getVifAndTex__FPcT0PP12_modelHeaderPP8_texture(const char* n, const char* what, void** model, void** tex) {
    trace += std::string("vif:") + n + "," + what + ";";
    *model = (void*)0x11; *tex = (void*)0x22;
    return 0;
}
int main() {
    Target& t = targets[1];
    std::memset(&t, 0, sizeof t);
    t.name = "orc"; t.modelName = "mdl"; t.kind = 4; t.flag = 1;
    add("orc_bodyv1.tex"); add("orc_bodyv2.tex");
    add("orc_hair1a.vif"); add("orc_hair1a.tex"); add("orc_hair1b.tex");
    add("orc_hair2a.vif"); add("orc_hair2a.tex");
    add("orc_hair3a.vif");                       // no texture: stops with j = 0
    add("orc_beard1a.vif"); add("orc_beard1a.tex");
    add("mdl.vif"); add("mdl.tex");
    getHairAndSkin__Fi(1);
    if (t.bodyTextureCount != 2) return 1;
    if (t.hairCount != 3) return 2;                 // two styles plus one for the flag
    if (t.hairVariants != 1) return 3;
    if (!t.hairModel[0] || !t.hairModel[1] || !t.hairModel[2] || t.hairModel[3]) return 4;
    if (!t.hairTexture[0][1] || t.hairTexture[0][2] || t.hairTexture[1][1]) return 5;
    if (t.beardCountPlusOne != 2) return 6;
    if (!t.beardModel[0] || t.beardModel[1] || !t.beardTexture[0][0]) return 7;
    if (t.armorModel[2] != (void*)0x11 || t.armorTexture[0] != (void*)0x22) return 8;
    if (trace.find("vif:orc,cloth1body11;vif:orc,cloth1leggings11;vif:orc,cloth1boots11;mdl.vif;mdl.tex;") == std::string::npos) return 9;
    if (!t.modelVif || !t.modelTex) return 10;
    if (std::strcmp(npcLumpName, "npc") != 0) return 11;
    // other kind and no flag
    trace.clear(); t.kind = 0; t.flag = 0;
    getHairAndSkin__Fi(1);
    if (t.hairCount != 2) return 12;
    if (trace.find("vif:orc,lethbody11;vif:orc,lethleggings11;vif:orc,lethboots11;") == std::string::npos) return 13;
    return 0;
}
