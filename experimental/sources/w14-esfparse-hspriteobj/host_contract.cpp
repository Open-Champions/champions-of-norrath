#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_hsprite_obj.cpp"

static std::string tr; static unsigned id_in;
static int find_ret, find_type, find_handle, create_ret, add_ret, hinit_ret, end_rets[12], end_calls, begin_rets[12], begin_calls, count_in, version, surf_ret, pal_ret, array_ret, hier_ret, map_ret, share_map_ret, trig_ret, attach_ret, anim_ret, share_anim_ret, addplay_ret, float_calls;
static unsigned short tags[12]; static HSpriteBox box_seen; static void* sphere_seen; static int shared_map, shared_anim, play_anim; static void* play_sprite; static void* play_scene; static void* hier_sprite;
alignas(16) static unsigned char sprite_mem[512];
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tags[begin_calls]; *second = 0; tr += "B;"; return begin_rets[begin_calls++]; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = 1.0f + float_calls++; tr += "F;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_rets[end_calls++]; }
extern "C" int NumSubObjects__9VIObjFile(void*) { tr += "N;"; return count_in; }
extern "C" int ObjectVersion__9VIObjFile(void*) { tr += "V;"; return version; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int CreateSprite__7VIScene12VISpriteTypei(void* s, int t, int a) { if (s != reinterpret_cast<void*>(0x20) || t != 4 || a != 0x99) tr += "BADCREATE"; tr += "C;"; return create_ret; }
extern "C" void* Sprite__7VIScenei(void*, int) { tr += "Sp;"; return sprite_mem; }
extern "C" int Init__9VIHSprite(void* s) { if (s != sprite_mem) tr += "BADHINIT"; tr += "HI;"; return hinit_ret; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) {
    if (r != sprite_mem || id != id_in || t != 5 || h != create_ret) tr += "BADADD"; tr += "A;"; return add_ret; }
extern "C" void Init__8VISphereRC6VIBBox(void* s, const HSpriteBox* b) { sphere_seen = s; box_seen = *b; tr += "Init;"; }
extern "C" int ParseSurfaceObj__10VIESFParse(void*) { tr += "PS;"; return surf_ret; }
extern "C" int ParseMaterialPalObj__10VIESFParse(void*) { tr += "PP;"; return pal_ret; }
extern "C" int ParseHSpriteSpriteArray__10VIESFParse(void*) { tr += "Arr;"; return array_ret; }
extern "C" int ParseHSpriteHierarchy__10VIESFParseR9VIHSprite(void*, void* s) { hier_sprite = s; tr += "Hier;"; return hier_ret; }
extern "C" int ParseRefMap__10VIESFParse(void*) { tr += "Map;"; return map_ret; }
extern "C" int ShareRefMap__7VIScenei(void* sc, int m) { if (sc != reinterpret_cast<void*>(0x20)) tr += "BADSCENE"; shared_map = m; tr += "SM;"; return share_map_ret; }
extern "C" int ParseHSpriteTriggers__10VIESFParseR9VIHSprite(void*, void*) { tr += "Trig;"; return trig_ret; }
extern "C" int ParseHSpriteAttachments__10VIESFParseR9VIHSprite(void*, void*) { tr += "Att;"; return attach_ret; }
extern "C" int ParseHSpriteAnim__10VIESFParse(void*) { tr += "Anim;"; return anim_ret; }
extern "C" int ShareAnimation__7VIScenei(void*, int a) { shared_anim = a; tr += "SA;"; return share_anim_ret; }
extern "C" int AddPlay__9VIHSpriteiR7VIScene(void* s, int a, void* sc) { play_sprite = s; play_anim = a; play_scene = sc; tr += "AP;"; return addplay_ret; }

static int run(unsigned raster) {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[3] = raster; w[6] = 0x20; w[8] = 0x30; w[9] = 0x55; tr.clear(); end_calls = 0; begin_calls = 0; float_calls = 0; sphere_seen = nullptr;
    memset(sprite_mem, 0xee, sizeof sprite_mem);
    return ParseHSpriteObj__10VIESFParse(parse);
}
static int slot(int off) { return *reinterpret_cast<int*>(sprite_mem + off); }
static float fl(int off) { return *reinterpret_cast<float*>(sprite_mem + off); }
int main() {
    id_in = 7; find_ret = 0; find_type = 5; find_handle = 11; create_ret = 6; add_ret = 0; hinit_ret = 0; count_in = 2; version = 2; surf_ret = 0; pal_ret = 3; array_ret = 0; hier_ret = 0; map_ret = 4; share_map_ret = 0; trig_ret = 0; attach_ret = 0; anim_ret = 5; share_anim_ret = 0; addplay_ret = 0;
    for (int& e : end_rets) e = 0; for (int& b : begin_rets) b = 0;
    tags[0] = 0x2200; tags[1] = 0x1110; tags[2] = 0;
    const std::string head = "B;U;F;F;F;F;F;F;E;Find;C;Sp;HI;A;Init;";
    const std::string tail = "Arr;Hier;Map;SM;V;Trig;Att;Anim;SA;AP;E;";
    // version 2: palette header only, triggers parsed
    if (run(0x77) != 6 || tr != head + "V;B;PP;" + tail) return 1;
    for (int i = 0; i < 6; ++i) if (box_seen.v[i] != 1.0f + i) return 2;
    if (fl(8) != 1.0f || fl(12) != 2.0f || fl(16) != 3.0f || fl(20) != 4.0f || fl(24) != 5.0f || fl(28) != 6.0f || sphere_seen != sprite_mem + 32 || hier_sprite != sprite_mem) return 3;
    if (slot(384) != 4 || slot(388) != 5 || shared_map != 4 || shared_anim != 5 || play_sprite != sprite_mem || play_anim != 5 || play_scene != reinterpret_cast<void*>(0x20)) return 4;
    // version 1: no triggers
    version = 1; if (run(0x77) != 6 || tr.find("Trig;") != std::string::npos) return 5;
    // version 0: surface array first
    version = 0; tags[1] = 0x1001; tags[2] = 0x1000; tags[3] = 0x1000; tags[4] = 0x1110;
    if (run(0x77) != 6 || tr != head + "V;B;N;B;PS;N;B;PS;N;E;B;PP;" + "Arr;Hier;Map;SM;V;Att;Anim;SA;AP;E;") return 6;
    tags[1] = 0x1002; if (run(0x77) != -1 || tr != head + "V;B;") return 7;
    tags[1] = 0x1001; tags[2] = 0x1002; if (run(0x77) != -1 || tr != head + "V;B;N;B;") return 8;
    tags[2] = 0x1000; surf_ret = -1; if (run(0x77) != -1 || tr != head + "V;B;N;B;PS;") return 9;
    surf_ret = 0; end_rets[1] = -1; if (run(0x77) != -1 || tr.find("PP;") != std::string::npos) return 10;
    end_rets[1] = 0; count_in = 0; tags[2] = 0x1110; if (run(0x77) != 6 || tr.find("PS;") != std::string::npos) return 11;
    count_in = 2;
    // palette problems
    version = 2; tags[1] = 0x1111; if (run(0x77) != -1 || tr != head + "V;B;") return 12;
    tags[1] = 0x1110; begin_rets[1] = -1; if (run(0x77) != -1 || tr != head + "V;B;") return 13;
    begin_rets[1] = 0; pal_ret = -2; if (run(0x77) != -1 || tr.find("Arr") != std::string::npos) return 14;
    pal_ret = 3;
    // zero raster: skip only
    if (run(0) != 6 || tr != head + "V;B;E;" + tail) return 15;
    version = 0; if (run(0) != 6 || tr != head + "V;B;E;B;E;Arr;Hier;Map;SM;V;Att;Anim;SA;AP;E;") return 16;
    version = 2;
    // tail failures
    array_ret = -1; if (run(0x77) != -1 || tr.find("Hier") != std::string::npos) return 17;
    array_ret = 0; hier_ret = -1; if (run(0x77) != -1 || tr.find("Map;") != std::string::npos) return 18;
    hier_ret = 0; map_ret = -3; if (run(0x77) != -1 || slot(384) != -3 || tr.find("SM;") != std::string::npos) return 19;
    map_ret = 4; share_map_ret = -1; if (run(0x77) != -1 || tr.find("Trig") != std::string::npos) return 20;
    share_map_ret = 0; trig_ret = -1; if (run(0x77) != -1 || tr.find("Att;") != std::string::npos) return 21;
    trig_ret = 0; attach_ret = -1; if (run(0x77) != -1 || tr.find("Anim;") != std::string::npos) return 22;
    attach_ret = 0; anim_ret = -5; if (run(0x77) != -1 || slot(388) != -5 || tr.find("SA;") != std::string::npos) return 23;
    anim_ret = 5; share_anim_ret = -1; if (run(0x77) != -1 || tr.find("AP;") != std::string::npos) return 24;
    share_anim_ret = 0; addplay_ret = -1; if (run(0x77) != -1 || tr.substr(tr.size() - 3) != "AP;") return 25;
    addplay_ret = 0; end_rets[1] = -1; if (run(0x77) != -1) return 26;
    end_rets[1] = 0;
    // dictionary hit, creation failures
    find_ret = 1; if (run(0x77) != 11 || tr != "B;U;F;F;F;F;F;F;E;Find;E;") return 27;
    find_type = 3; if (run(0x77) != -1 || tr != "B;U;F;F;F;F;F;F;E;Find;") return 28;
    find_type = 5; find_ret = 0;
    end_rets[0] = -1; if (run(0x77) != -1 || tr != "B;U;F;F;F;F;F;F;E;") return 29;
    end_rets[0] = 0; hinit_ret = -1; if (run(0x77) != -1 || tr.find("A;") != std::string::npos) return 30;
    hinit_ret = 0; add_ret = -1; if (run(0x77) != -1 || tr.find("Init;") != std::string::npos) return 31;
    add_ret = 0; create_ret = -3; if (run(0x77) != -1 || tr.find("Sp;") != std::string::npos) return 32;
    create_ret = 6; begin_rets[0] = -1; if (run(0x77) != -1 || tr != "B;") return 33;
    begin_rets[0] = 0; id_in = 0; if (run(0x77) != 6 || tr.find("Find;") != std::string::npos || tr.find("HI;A;") != std::string::npos) return 34;
    return 0;
}
