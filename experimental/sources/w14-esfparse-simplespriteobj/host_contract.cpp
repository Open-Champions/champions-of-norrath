#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_simple_sprite_obj.cpp"

static std::string tr; static unsigned id_in;
static int find_ret, find_type, find_handle, create_ret, add_ret, end_rets[12], end_calls, begin_rets[12], begin_calls, count_in, version, surf_ret, pal_ret, prim_ret, coll_ret, share_pal_ret, share_prim_ret, share_coll_ret, float_calls;
static unsigned short tags[12], second_in; static SimpleBox box_seen; static void* sphere_seen; static int shared_pal, shared_prim, shared_coll; static void* shared_raster; static void* shared_collision; static float extra_float = 123.0f;
alignas(16) static unsigned char sprite_mem[256];
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tags[begin_calls]; *second = begin_calls == 0 ? second_in : 0; tr += "B;"; return begin_rets[begin_calls++]; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = float_calls < 6 ? 1.0f + float_calls : extra_float; ++float_calls; tr += "F;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_rets[end_calls++]; }
extern "C" int NumSubObjects__9VIObjFile(void*) { tr += "N;"; return count_in; }
extern "C" int ObjectVersion__9VIObjFile(void*) { tr += "V;"; return version; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int CreateSprite__7VIScene12VISpriteTypei(void* s, int t, int a) { if (s != reinterpret_cast<void*>(0x20) || t != 1 || a != 0x99) tr += "BADCREATE"; tr += "C;"; return create_ret; }
extern "C" void* Sprite__7VIScenei(void*, int) { tr += "Sp;"; return sprite_mem; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) {
    if (r != sprite_mem || id != id_in || t != 4 || h != create_ret) tr += "BADADD"; tr += "A;"; return add_ret; }
extern "C" void Init__8VISphereRC6VIBBox(void* s, const SimpleBox* b) { sphere_seen = s; box_seen = *b; tr += "Init;"; }
extern "C" int ParseSurfaceObj__10VIESFParse(void*) { tr += "PS;"; return surf_ret; }
extern "C" int ParseMaterialPalObj__10VIESFParse(void*) { tr += "PP;"; return pal_ret; }
extern "C" int ParsePrimBuffer__10VIESFParse(void*) { tr += "PB;"; return prim_ret; }
extern "C" int ParseCollBuffer__10VIESFParse(void*) { tr += "PC;"; return coll_ret; }
extern "C" int ShareMaterialPal__8VIRasteri(void* r, int p) { shared_raster = r; shared_pal = p; tr += "SP;"; return share_pal_ret; }
extern "C" int SharePrimBuffer__8VIRasteri(void* r, int b) { if (r != shared_raster) tr += "BADRASTER"; shared_prim = b; tr += "SB;"; return share_prim_ret; }
extern "C" int ShareCollBuffer__9VICollidei(void* c, int b) { shared_collision = c; shared_coll = b; tr += "SC;"; return share_coll_ret; }

static int run(unsigned raster) {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[3] = raster; w[5] = 0x88; w[6] = 0x20; w[8] = 0x30; w[9] = 0x55; tr.clear(); end_calls = 0; begin_calls = 0; float_calls = 0; sphere_seen = nullptr; shared_raster = nullptr;
    memset(sprite_mem, 0xee, sizeof sprite_mem);
    return ParseSimpleSpriteObj__10VIESFParse(parse);
}
static int slot(int off) { return *reinterpret_cast<int*>(sprite_mem + off); }
static float fl(int off) { return *reinterpret_cast<float*>(sprite_mem + off); }
int main() {
    id_in = 7; find_ret = 0; find_type = 4; find_handle = 11; create_ret = 6; add_ret = 0; count_in = 2; version = 2; surf_ret = 0; pal_ret = 3; prim_ret = 5; coll_ret = 8; share_pal_ret = 0; share_prim_ret = 0; share_coll_ret = 0; second_in = 1;
    for (int& e : end_rets) e = 0; for (int& b : begin_rets) b = 0;
    tags[0] = 0x2000; tags[1] = 0x1110; tags[2] = 0;
    const std::string head = "B;U;F;F;F;F;F;F;F;E;Find;C;Sp;A;Init;";
    // version 2: no surface array, palette header then primitive buffer, then collision buffer
    if (run(0x77) != 6 || tr != head + "V;B;PP;SP;PB;SB;V;PC;SC;E;") return 1;
    if (slot(72) != 3 || slot(64) != 5 || slot(68) != 8 || shared_pal != 3 || shared_prim != 5 || shared_coll != 8 || shared_raster != reinterpret_cast<void*>(0x77) || shared_collision != reinterpret_cast<void*>(0x88)) return 2;
    for (int i = 0; i < 6; ++i) if (box_seen.v[i] != 1.0f + i) return 3;
    if (fl(8) != 1.0f || fl(12) != 2.0f || fl(16) != 3.0f || fl(20) != 4.0f || fl(24) != 5.0f || fl(28) != 6.0f || fl(48) != 123.0f || sphere_seen != sprite_mem + 32) return 4;
    // no extra float: the default distance is 500
    second_in = 0; if (run(0x77) != 6 || fl(48) != 500.0f || tr.find("F;F;F;F;F;F;E;") == std::string::npos) return 5;
    second_in = 1;
    // version 1: the 0x1001 surface array comes first
    version = 1; tags[1] = 0x1001; tags[2] = 0x1000; tags[3] = 0x1000; tags[4] = 0x1110;
    if (run(0x77) != 6 || tr != head + "V;B;N;B;PS;N;B;PS;N;E;B;PP;SP;PB;SB;V;PC;SC;E;") return 6;
    // version 0 skips the collision buffer
    version = 0; count_in = 0; tags[2] = 0x1110; if (run(0x77) != 6 || tr.find("PC;") != std::string::npos) return 7;
    count_in = 2; tags[2] = 0x1000;
    tags[1] = 0x1002; if (run(0x77) != -1 || tr != head + "V;B;") return 8;
    tags[1] = 0x1001; tags[2] = 0x1002; if (run(0x77) != -1 || tr != head + "V;B;N;B;") return 9;
    tags[2] = 0x1000; surf_ret = -1; if (run(0x77) != -1 || tr != head + "V;B;N;B;PS;") return 10;
    surf_ret = 0; end_rets[1] = -1; if (run(0x77) != -1 || tr.find("PP") != std::string::npos) return 11;
    end_rets[1] = 0;
    // palette header problems
    version = 2; tags[1] = 0x1111; if (run(0x77) != -1 || slot(72) != -1 || tr.find("SP;") != std::string::npos) return 12;
    tags[1] = 0x1110; begin_rets[1] = -1; if (run(0x77) != -1 || slot(72) != -1) return 13;
    begin_rets[1] = 0; pal_ret = -2; if (run(0x77) != -1 || slot(72) != -2) return 14;
    pal_ret = 3; share_pal_ret = -1; if (run(0x77) != -1 || tr.find("PB;") != std::string::npos) return 15;
    share_pal_ret = 0; prim_ret = -4; if (run(0x77) != -1 || slot(64) != -4 || tr.find("SB;") != std::string::npos) return 16;
    prim_ret = 5; share_prim_ret = -1; if (run(0x77) != -1 || tr.find("PC;") != std::string::npos) return 17;
    share_prim_ret = 0; coll_ret = -6; if (run(0x77) != -1 || slot(68) != -6 || tr.find("SC;") != std::string::npos) return 18;
    coll_ret = 8; share_coll_ret = -1; if (run(0x77) != -1 || tr.find("E;", tr.find("SC;")) != std::string::npos) return 19;
    share_coll_ret = 0; end_rets[1] = -1; if (run(0x77) != -1) return 20;
    end_rets[1] = 0;
    // zero raster: skip only
    version = 2; if (run(0) != 6 || tr != head + "V;B;E;B;E;V;PC;SC;E;" || slot(72) != -1 || slot(64) != -1 || fl(48) != 123.0f) return 21;
    version = 1; if (run(0) != 6 || tr != head + "V;B;E;B;E;B;E;V;PC;SC;E;") return 22;
    version = 0; if (run(0) != 6 || tr.find("PC;") != std::string::npos) return 23;
    version = 2;
    // dictionary hit, creation failures
    find_ret = 1; if (run(0x77) != 11 || tr != "B;U;F;F;F;F;F;F;F;E;Find;E;") return 24;
    find_type = 3; if (run(0x77) != -1 || tr != "B;U;F;F;F;F;F;F;F;E;Find;") return 25;
    find_type = 4; find_ret = 0;
    end_rets[0] = -1; if (run(0x77) != -1 || tr != "B;U;F;F;F;F;F;F;F;E;") return 26;
    end_rets[0] = 0; add_ret = -1; if (run(0x77) != -1 || tr.find("Init") != std::string::npos) return 27;
    add_ret = 0; create_ret = -3; if (run(0x77) != -1 || tr.find("Sp;") != std::string::npos) return 28;
    create_ret = 6; begin_rets[0] = -1; if (run(0x77) != -1 || tr != "B;") return 29;
    begin_rets[0] = 0; id_in = 0; if (run(0x77) != 6 || tr.find("Find;") != std::string::npos || tr.find("A;") != std::string::npos) return 30;
    return 0;
}
