#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_flora_sprite_obj.cpp"

static std::string tr; static unsigned id_in; static int find_ret, find_type, find_handle, create_ret, add_ret, end_rets[5], end_calls, begin_rets[4], begin_calls, pal_ret, prim_ret, share_pal_ret, share_prim_ret, float_calls;
static unsigned short tags[4]; static FloraBox box_seen; static void* sphere_seen; static int shared_palette, shared_buffer; static void* shared_raster;
alignas(16) static unsigned char sprite_mem[128];
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tags[begin_calls]; *second = 0; tr += "B;"; return begin_rets[begin_calls++]; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = 1.0f + float_calls++; tr += "F;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_rets[end_calls++]; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int CreateSprite__7VIScene12VISpriteTypei(void* s, int t, int a) { if (s != reinterpret_cast<void*>(0x20) || t != 13 || a != 0x99) tr += "BADCREATE"; tr += "C;"; return create_ret; }
extern "C" void* Sprite__7VIScenei(void*, int) { tr += "Sp;"; return sprite_mem; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) {
    if (r != sprite_mem || id != id_in || t != 25 || h != create_ret) tr += "BADADD"; tr += "A;"; return add_ret; }
extern "C" void Init__8VISphereRC6VIBBox(void* s, const FloraBox* b) { sphere_seen = s; box_seen = *b; tr += "Init;"; }
extern "C" int ParseMaterialPalObj__10VIESFParse(void*) { tr += "Pal;"; return pal_ret; }
extern "C" int ParseFloraPrimBuffer__10VIESFParse(void*) { tr += "Prim;"; return prim_ret; }
extern "C" int ShareMaterialPal__8VIRasteri(void* r, int p) { shared_raster = r; shared_palette = p; tr += "SPal;"; return share_pal_ret; }
extern "C" int SharePrimBuffer__8VIRasteri(void* r, int b) { if (r != shared_raster) tr += "BADRASTER"; shared_buffer = b; tr += "SPrim;"; return share_prim_ret; }

static int run(unsigned raster) {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[3] = raster; w[6] = 0x20; w[8] = 0x30; w[9] = 0x55; tr.clear(); end_calls = 0; begin_calls = 0; float_calls = 0; sphere_seen = nullptr; shared_raster = nullptr;
    memset(sprite_mem, 0xee, sizeof sprite_mem);
    return ParseFloraSpriteObj__10VIESFParse(parse);
}
static int slot(int off) { return *reinterpret_cast<int*>(sprite_mem + off); }
int main() {
    id_in = 7; find_ret = 0; find_type = 25; find_handle = 11; create_ret = 6; add_ret = 0; pal_ret = 3; prim_ret = 5; share_pal_ret = 0; share_prim_ret = 0;
    for (int& e : end_rets) e = 0; for (int& b : begin_rets) b = 0; tags[0] = 0x2f10; tags[1] = 0x1110; tags[2] = 0; tags[3] = 0;
    const char* head = "B;U;F;F;F;F;F;F;E;Find;C;Sp;A;Init;";
    if (run(0x77) != 6 || tr != std::string(head) + "B;Pal;SPal;Prim;SPrim;E;") return 1;
    if (slot(68) != 3 || slot(64) != 5 || shared_palette != 3 || shared_buffer != 5 || shared_raster != reinterpret_cast<void*>(0x77)) return 2;
    const float* lo = reinterpret_cast<const float*>(sprite_mem + 8); const float* hi = reinterpret_cast<const float*>(sprite_mem + 20);
    if (lo[0] != 1.0f || lo[1] != 2.0f || lo[2] != 3.0f || hi[0] != 4.0f || hi[1] != 5.0f || hi[2] != 6.0f || sphere_seen != sprite_mem + 32) return 3;
    for (int i = 0; i < 6; ++i) if (box_seen.v[i] != 1.0f + i) return 4;
    // raster word zero: skip two sub-objects, both slots become -1
    if (run(0) != 6 || tr != std::string(head) + "B;E;B;E;E;" || slot(68) != -1 || slot(64) != -1) return 5;
    // other tag: palette is -1, stored, and fails
    tags[1] = 0x1111; if (run(0x77) != -1 || slot(68) != -1 || tr != std::string(head) + "B;") return 6;
    tags[1] = 0x1110; begin_rets[1] = -1; if (run(0x77) != -1 || slot(68) != -1) return 7;
    begin_rets[1] = 0; pal_ret = -2; if (run(0x77) != -1 || slot(68) != -2 || tr.find("SPal") != std::string::npos) return 8;
    pal_ret = 3; share_pal_ret = -1; if (run(0x77) != -1 || tr.find("Prim;") != std::string::npos) return 9;
    share_pal_ret = 0; prim_ret = -4; if (run(0x77) != -1 || slot(64) != -4 || tr.find("SPrim") != std::string::npos) return 10;
    prim_ret = 5; share_prim_ret = -1; if (run(0x77) != -1 || tr.find("E;", tr.find("SPrim")) != std::string::npos) return 11;
    share_prim_ret = 0; end_rets[1] = -1; if (run(0x77) != -1) return 12;
    end_rets[1] = 0;
    find_ret = 1; if (run(0x77) != 11 || tr != "B;U;F;F;F;F;F;F;E;Find;E;") return 13;
    find_type = 3; if (run(0x77) != -1 || tr != "B;U;F;F;F;F;F;F;E;Find;") return 14;
    find_type = 25; find_ret = 0;
    end_rets[0] = -1; if (run(0x77) != -1 || tr != "B;U;F;F;F;F;F;F;E;") return 15;
    end_rets[0] = 0; add_ret = -1; if (run(0x77) != -1 || tr.find("Init") != std::string::npos) return 16;
    add_ret = 0; create_ret = -3; if (run(0x77) != -1 || tr.find("Sp;") != std::string::npos) return 17;
    create_ret = 6; begin_rets[0] = -1; if (run(0x77) != -1 || tr != "B;") return 18;
    begin_rets[0] = 0; id_in = 0; if (run(0x77) != 6 || tr.find("Find") != std::string::npos || tr.find("A;") != std::string::npos) return 19;
    return 0;
}
