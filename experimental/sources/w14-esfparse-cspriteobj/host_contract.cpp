#include <stdint.h>
#include <string.h>
#include <map>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_csprite_obj.cpp"

static std::string tr; static unsigned id_in; static std::map<std::string, int> bad_token;  // token -> forced -1
static int find_ret, find_type, find_handle, create_ret, end_rets[12], end_calls, begin_rets[12], begin_calls, count_in, version, int_calls, float_calls;
static unsigned short tags[12], second_in; static CSpriteBox box_seen; static void* sphere_seen;
static int init_skel, init_race, init_sex, init_flags; static float init_scale;
alignas(16) static unsigned char sprite_mem[1024];
static unsigned raster_word;
static int rv(const char* token, int dflt = 0) { tr += std::string(token) + ";"; return bad_token.count(token) ? -1 : dflt; }
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tags[begin_calls]; *second = second_in; tr += "B;"; return begin_rets[begin_calls++]; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = 11 + int_calls++; tr += "I;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = 1.0f + float_calls++; tr += "F;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_rets[end_calls++]; }
extern "C" int NumSubObjects__9VIObjFile(void*) { tr += "N;"; return count_in; }
extern "C" unsigned ObjectVersion__9VIObjFile(void*) { return static_cast<unsigned>(version); }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int CreateSprite__7VIScene12VISpriteTypei(void* s, int t, int a) { if (s != reinterpret_cast<void*>(0x20) || t != 5 || a != 0x99) tr += "BADCREATE"; tr += "C;"; return create_ret; }
extern "C" void* Sprite__7VIScenei(void*, int) { tr += "Sp;"; return sprite_mem; }
extern "C" int Init__9VICSprite17VICSpriteSkelType13VICSpriteRace12VICSpriteSexfi(void* s, int sk, int r, int sx, float sc, int fl) {
    if (s != sprite_mem) tr += "BADINIT"; init_skel = sk; init_race = r; init_sex = sx; init_scale = sc; init_flags = fl; return rv("Init"); }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) {
    if (r != sprite_mem || id != id_in || t != 9 || h != create_ret) tr += "BADADD"; return rv("A"); }
extern "C" void Init__8VISphereRC6VIBBox(void* s, const CSpriteBox* b) { sphere_seen = s; box_seen = *b; tr += "Sph;"; }
extern "C" int ParseSurfaceObj__10VIESFParse(void*) { return rv("PS"); }
extern "C" int ParseMaterialPalObj__10VIESFParse(void*) { return rv("PP", 3); }
static int map_calls;
extern "C" int ParseRefMap__10VIESFParse(void*) { ++map_calls; return rv("Map", 100 + map_calls); }
extern "C" int ParseSoundArray__10VIESFParse(void*) { return rv("Snd"); }
extern "C" int ParseCSpriteSpriteArray__10VIESFParse(void*) { return rv("SA"); }
extern "C" int ParseHSpriteAnimArray__10VIESFParse(void*) { return rv("AA"); }
#define SPRITE_FN(NAME, TOKEN) extern "C" int NAME(void*, void* s) { if (s != sprite_mem) tr += "BADSPRITE"; return rv(TOKEN); }
SPRITE_FN(ParseHSpriteHierarchy__10VIESFParseR9VIHSprite, "Hi")
SPRITE_FN(ParseHSpriteTriggers__10VIESFParseR9VIHSprite, "Tr")
SPRITE_FN(ParseCSpriteSkinList__10VIESFParseR9VICSprite, "Sk")
SPRITE_FN(ParseCSpritePlayList__10VIESFParseR9VICSprite, "Pl")
SPRITE_FN(ParseCSpriteNodeIDList__10VIESFParseR9VICSprite, "Nd")
SPRITE_FN(ParseCSpriteASlotList__10VIESFParseR9VICSprite, "As")
SPRITE_FN(ParseCSpriteTSlotList__10VIESFParseR9VICSprite, "Ts")
SPRITE_FN(ParseCSpriteContSound__10VIESFParseR9VICSprite, "Cs")
SPRITE_FN(ParseCSpritePartEmitters__10VIESFParseR9VICSprite, "Pe")
extern "C" int ParseCSpritePartDefinitions__10VIESFParse(void*) { return rv("Pd"); }
extern "C" int SetDefaultEntries__9VICSprite(void* s) { if (s != sprite_mem) tr += "BADSPRITE"; return rv("De"); }
extern "C" int InitTextSlots__9VICSpriteR8VIRaster(void* s, void* r) { if (s != sprite_mem || r != reinterpret_cast<void*>(0x77)) tr += "BADTEXT"; return rv("It", -5); }
extern "C" int SetAnimPriorities__9VICSprite(void*) { return rv("Ap"); }
extern "C" int SetAnimSoundChannels__9VICSprite(void*) { return rv("Asc"); }
extern "C" int Share__9VICSpriteR7VISceneP8VIRaster(void*, void* sc, void* r) { if (sc != reinterpret_cast<void*>(0x20) || r != host_address(raster_word)) tr += "BADSHARE"; return rv("Sh"); }
extern "C" int SetDefaults__9VICSpriteR7VIScene(void*, void* sc) { if (sc != reinterpret_cast<void*>(0x20)) tr += "BADSC"; return rv("Sd"); }
extern "C" int CalcLODOffDist__9VICSpriteR7VIScene(void*, void* sc) { if (sc != reinterpret_cast<void*>(0x20)) tr += "BADSC"; return rv("Lod"); }
extern "C" int CreateParticleEmitters__9VICSpriteR7VIScene(void*, void* sc) { if (sc != reinterpret_cast<void*>(0x20)) tr += "BADSC"; return rv("Cpe"); }

static int run(unsigned raster) {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    raster_word = raster; w[0] = 0x99; w[3] = raster; w[6] = 0x20; w[8] = 0x30; w[9] = 0x55; tr.clear(); end_calls = 0; begin_calls = 0; int_calls = 0; float_calls = 0; map_calls = 0; sphere_seen = nullptr;
    memset(sprite_mem, 0xee, sizeof sprite_mem);
    return ParseCSpriteObj__10VIESFParse(parse);
}
static int slot(int off) { return *reinterpret_cast<int*>(sprite_mem + off); }
// the trace the full parse should produce for the given version, header revision and raster presence
static std::string expect(int ver, int second, bool raster) {
    std::string s = "B;U;F;F;F;F;F;F;";
    if (second != 0) s += "I;";
    if (second >= 2) s += "F;";
    if (second >= 3) s += "I;I;";
    if (second >= 4) s += "I;";
    s += "E;Find;C;Sp;Init;A;Sph;";
    if (raster) {
        if (ver < 3) s += "B;N;B;PS;N;E;";
        s += "B;PP;Map;";
    } else {
        if (ver < 3) s += "B;E;";
        s += "B;E;B;E;";
    }
    if (ver >= 4) s += "Snd;";
    s += "SA;AA;Hi;Map;";
    if (ver >= 4) s += "Tr;";
    s += "Sk;Pl;";
    if (ver >= 2) s += "Nd;";
    if (ver != 0) s += "As;";
    if (ver >= 5) s += "Ts;";
    if (ver >= 6) s += "Cs;";
    if (ver >= 7) s += "Pd;Pe;";
    s += "De;";
    if (raster) s += "It;";
    s += "Ap;Asc;Sh;Sd;Lod;Cpe;E;";
    return s;
}
int main() {
    id_in = 7; find_ret = 0; find_type = 9; find_handle = 11; create_ret = 6; count_in = 1; second_in = 5;
    for (int& e : end_rets) e = 0; for (int& b : begin_rets) b = 0;
    tags[0] = 0x3000; tags[1] = 0x1001; tags[2] = 0x1000; tags[3] = 0x1110; tags[4] = 0x1110;
    // every version/revision/raster combination produces exactly its gated sequence
    for (int ver = 0; ver <= 8; ++ver) {
        for (int second = 0; second <= 5; ++second) {
            for (int raster = 0; raster < 2; ++raster) {
                version = ver; second_in = static_cast<unsigned short>(second);
                // header 1 is the first tag; the array tag, surface tag and palette tag follow in order
                if (ver < 3) { tags[1] = 0x1001; tags[2] = 0x1000; tags[3] = 0x1110; } else { tags[1] = 0x1110; }
                if (run(raster ? 0x77 : 0) != 6 || tr != expect(ver, second, raster != 0)) return 1 + ver * 100 + second * 10 + raster;
            }
        }
    }
    // header values reach the sprite init call
    version = 8; second_in = 5; tags[1] = 0x1110; run(0x77);
    if (init_skel != 11 || init_race != 12 || init_sex != 13 || init_scale != 7.0f || init_flags != 14) return 2000;
    second_in = 1; run(0x77); if (init_skel != 11 || init_race != 9 || init_sex != 0 || init_scale != 1.0f || init_flags != 1) return 2001;
    second_in = 0; run(0x77); if (init_skel != 0 || init_race != 9 || init_sex != 0 || init_scale != 1.0f || init_flags != 1) return 2002;
    second_in = 3; run(0x77); if (init_skel != 11 || init_race != 12 || init_sex != 13 || init_scale != 7.0f || init_flags != 1) return 2003;
    second_in = 5; run(0x77);
    // box, sprite slots
    for (int i = 0; i < 6; ++i) if (box_seen.v[i] != 1.0f + i) return 2004;
    const float* lo = reinterpret_cast<const float*>(sprite_mem + 8); const float* hi = reinterpret_cast<const float*>(sprite_mem + 20);
    if (lo[0] != 1.0f || lo[2] != 3.0f || hi[0] != 4.0f || hi[2] != 6.0f || sphere_seen != sprite_mem + 32) return 2005;
    if (slot(416) != 3 || slot(420) != 101 || slot(384) != 102) return 2006;
    run(0); if (slot(384) != 101 || slot(416) != static_cast<int>(0xeeeeeeee) || slot(420) != static_cast<int>(0xeeeeeeee)) return 2007;
    // each failing call stops the parse right there
    const char* tokens[] = {"Init", "A", "PP", "Map", "Snd", "SA", "AA", "Hi", "Tr", "Sk", "Pl", "Nd", "As", "Ts", "Cs", "Pd", "Pe", "De", "Ap", "Asc", "Sh", "Sd", "Lod", "Cpe"};
    for (const char* tk : tokens) {
        bad_token.clear(); bad_token[tk] = 1;
        if (run(0x77) != -1 || tr.substr(tr.size() - strlen(tk) - 1) != std::string(tk) + ";") return 3000 + static_cast<int>(tk[0]) * 3 + static_cast<int>(tk[1]);
    }
    bad_token.clear();
    // a failing palette parse is stored, and the map after it is never parsed
    bad_token["PP"] = 1; run(0x77); if (slot(416) != -1 || tr.find("Map;") != std::string::npos) return 4000;
    bad_token.clear(); bad_token["Map"] = 1; run(0x77); if (slot(420) != -1) return 4001;
    bad_token.clear();
    // the surface array failures (version below 3)
    version = 2; tags[1] = 0x1002; if (run(0x77) != -1 || tr.back() != ';' || tr.find("PS;") != std::string::npos) return 4002;
    tags[1] = 0x1001; tags[2] = 0x1002; if (run(0x77) != -1 || tr.find("PS;") != std::string::npos) return 4003;
    tags[2] = 0x1000; bad_token["PS"] = 1; if (run(0x77) != -1 || tr.find("SA;") != std::string::npos) return 4004;
    bad_token.clear(); end_rets[1] = -1; if (run(0x77) != -1 || tr.find("PP") != std::string::npos) return 4005;
    end_rets[1] = 0; tags[3] = 0x1111; if (run(0x77) != -1 || slot(416) != -1) return 4006;
    tags[3] = 0x1110; version = 8; tags[1] = 0x1110;
    // palette header read failure
    begin_rets[1] = -1; if (run(0x77) != -1 || slot(416) != -1) return 4007;
    begin_rets[1] = 0;
    // closing ReadEnd
    end_rets[1] = -1; if (run(0x77) != -1) return 4008;
    end_rets[1] = 0; end_rets[0] = -1; if (run(0x77) != -1 || tr != "B;U;F;F;F;F;F;F;I;F;I;I;I;E;") return 4009;
    end_rets[0] = 0;
    // dictionary hit, creation failures
    find_ret = 1; if (run(0x77) != 11 || tr.find("Find;E;") == std::string::npos || tr.find("C;") != std::string::npos) return 4010;
    find_type = 3; if (run(0x77) != -1 || tr.back() != ';' || tr.substr(tr.size() - 5) != "Find;") return 4011;
    find_type = 9; find_ret = 0; create_ret = -3; if (run(0x77) != -1 || tr.find("Sp;") != std::string::npos) return 4012;
    create_ret = 6; begin_rets[0] = -1; if (run(0x77) != -1 || tr != "B;") return 4013;
    begin_rets[0] = 0; id_in = 0; if (run(0x77) != 6 || tr.find("Find;") != std::string::npos || tr.find("Init;A;") != std::string::npos) return 4014;
    return 0;
}
