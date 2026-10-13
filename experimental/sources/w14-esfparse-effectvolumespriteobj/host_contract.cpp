#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_effect_volume_sprite_obj.cpp"

static std::string tr; static unsigned id_in; static int find_ret, find_type, find_handle, create_ret, add_ret, end_rets[6], end_calls, begin_rets[4], begin_calls, count_in, obj_ret[8], obj_calls, float_calls, int_calls;
static int surf_find_ret[2], surf_find_type[2], surf_find_handle[2]; static unsigned short tags[4], second_in; static unsigned flags_in;
static EffectBox box_seen; static int vol_type, att_slot[2], att_surface[2], att_calls; static unsigned flags_seen; static void* vol_seen; static float vol_float;
alignas(16) static unsigned char sprite_mem[256];
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tags[begin_calls]; *second = second_in; tr += "B;"; return begin_rets[begin_calls++]; }
static int uint_calls;
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = uint_calls++ == 0 ? id_in : flags_in; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = int_calls == 0 ? 3 : (int_calls == 1 ? 50 : 60); ++int_calls; tr += "I;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = 1.0f + float_calls++; tr += "F;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_rets[end_calls++]; }
extern "C" int NumSubObjects__9VIObjFile(void*) { tr += "N;"; return count_in; }
extern "C" int ParseObject__10VIESFParse(void*) { tr += "O;"; return obj_ret[obj_calls++]; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30)) tr += "BADFIND";
    if (id == id_in) { *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
    int k = id == 50 ? 0 : 1;
    *t = surf_find_type[k]; if (surf_find_ret[k]) *h = surf_find_handle[k]; tr += id == 50 ? "S0;" : "S1;"; return surf_find_ret[k]; }
extern "C" int CreateSprite__7VIScene12VISpriteTypei(void* s, int t, int a) { if (s != reinterpret_cast<void*>(0x20) || t != 14 || a != 0x99) tr += "BADCREATE"; tr += "C;"; return create_ret; }
extern "C" void* Sprite__7VIScenei(void*, int) { tr += "Sp;"; return sprite_mem; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) {
    if (r != sprite_mem || id != id_in || t != 26 || h != create_ret) tr += "BADADD"; tr += "A;"; return add_ret; }
extern "C" void Init__20VIEffectVolumeSpriteRC6VIBBox(void* s, const EffectBox* b) { if (s != sprite_mem) tr += "BADINIT"; box_seen = *b; tr += "Init;"; }
extern "C" void SetType__14VIEffectVolume18VIEffectVolumeTypef(void* v, int t, float f) { vol_seen = v; vol_type = t; vol_float = f; tr += "ST;"; }
extern "C" void AttachSurface__14VIEffectVolumeii(void* v, int slot, int surface) { if (v != vol_seen) tr += "BADVOL"; att_slot[att_calls] = slot; att_surface[att_calls++] = surface; tr += "AS;"; }
extern "C" void SetFlags__14VIEffectVolumeUi(void* v, unsigned f) { if (v != vol_seen) tr += "BADVOL"; flags_seen = f; tr += "SF;"; }

static int run(unsigned flag) {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[3] = flag; w[6] = 0x20; w[8] = 0x30; w[9] = 0x55; tr.clear(); end_calls = 0; begin_calls = 0; float_calls = 0; int_calls = 0; obj_calls = 0; uint_calls = 0; att_calls = 0; vol_seen = nullptr;
    memset(sprite_mem, 0xee, sizeof sprite_mem);
    return ParseEffectVolumeSpriteObj__10VIESFParse(parse);
}
static float fl(int off) { return *reinterpret_cast<float*>(sprite_mem + off); }
int main() {
    id_in = 7; find_ret = 0; find_type = 26; find_handle = 11; create_ret = 6; add_ret = 0; count_in = 2; second_in = 2; flags_in = 0x40;
    for (int& e : end_rets) e = 0; for (int& b : begin_rets) b = 0; for (int& o : obj_ret) o = 0;
    tags[0] = 0x2000; tags[1] = 0xc320; tags[2] = 0xc330; tags[3] = 0;
    for (int k = 0; k < 2; ++k) { surf_find_ret[k] = 1; surf_find_type[k] = 1; surf_find_handle[k] = 100 + k; }
    const std::string head = "B;U;F;F;F;F;F;F;E;Find;C;Sp;A;Init;";
    const std::string vol = "I;F;F;F;F;F;I;I;";
    // nested block, volume parameters, surfaces found
    if (run(0x77) != 6 || tr != head + "B;N;O;O;E;B;" + vol + "U;S0;S1;ST;AS;AS;SF;E;E;") return 1;
    for (int i = 0; i < 6; ++i) if (box_seen.v[i] != 1.0f + i) return 2;
    if (vol_type != 3 || vol_float != 11.0f || fl(128) != 10.0f || fl(140) != 7.0f || fl(144) != 8.0f || fl(136) != 9.0f) return 3;
    if (att_slot[0] != 0 || att_slot[1] != 1 || att_surface[0] != 100 || att_surface[1] != 101 || flags_seen != 0x40 || vol_seen != sprite_mem + 88) return 4;
    // flag word zero: the nested objects are not parsed and the volume is untouched
    if (run(0) != 6 || tr != head + "B;E;B;" + vol + "U;E;E;" || sprite_mem[128] != 0xee) return 5;
    // second output below 2: no flags word, flags 0
    second_in = 1; if (run(0x77) != 6 || tr != head + "B;N;O;O;E;B;" + vol + "S0;S1;ST;AS;AS;SF;E;E;" || flags_seen != 0) return 6;
    second_in = 2;
    // missed surfaces leave the default slot -1; a hit of another type fails
    surf_find_ret[0] = 0; surf_find_ret[1] = 0; if (run(0x77) != 6 || att_surface[0] != -1 || att_surface[1] != -1) return 7;
    surf_find_ret[0] = 1; surf_find_ret[1] = 1; surf_find_type[1] = 5; if (run(0x77) != -1 || tr.find("ST;") != std::string::npos) return 8;
    surf_find_type[1] = 1; surf_find_type[0] = 5; if (run(0x77) != -1 || tr.find("S1;") != std::string::npos) return 9;
    surf_find_type[0] = 1;
    // tag 0xc330 directly: the nested block is skipped
    tags[1] = 0xc330; if (run(0x77) != 6 || tr != head + "B;" + vol + "U;S0;S1;ST;AS;AS;SF;E;E;") return 10;
    // any other tag: nothing more is read before the closing ReadEnds
    tags[1] = 0xc331; if (run(0x77) != 6 || tr != head + "B;E;E;") return 11;
    tags[1] = 0xc320; tags[2] = 0xc330;
    // nested block failures
    obj_ret[1] = -1; if (run(0x77) != -1 || tr != head + "B;N;O;O;") return 12;
    obj_ret[1] = 0; end_rets[1] = -1; if (run(0x77) != -1 || tr != head + "B;N;O;O;E;") return 13;
    end_rets[1] = 0; begin_rets[2] = -1; if (run(0x77) != -1 || tr != head + "B;N;O;O;E;B;") return 14;
    begin_rets[2] = 0; begin_rets[1] = -1; if (run(0x77) != -1 || tr != head + "B;") return 25;
    begin_rets[1] = 0; count_in = 0; if (run(0x77) != 6 || tr.find("O;") != std::string::npos) return 15;
    count_in = 2; end_rets[2] = -1; if (run(0x77) != -1) return 16;
    end_rets[2] = 0; end_rets[3] = -1; if (run(0x77) != -1) return 17;
    end_rets[3] = 0; begin_rets[0] = -1; if (run(0x77) != -1 || tr != "B;") return 18;
    begin_rets[0] = 0;
    // dictionary hit, creation failures
    find_ret = 1; if (run(0x77) != 11 || tr != "B;U;F;F;F;F;F;F;E;Find;E;") return 19;
    find_type = 3; if (run(0x77) != -1 || tr != "B;U;F;F;F;F;F;F;E;Find;") return 20;
    find_type = 26; find_ret = 0;
    end_rets[0] = -1; if (run(0x77) != -1 || tr != "B;U;F;F;F;F;F;F;E;") return 21;
    end_rets[0] = 0; add_ret = -1; if (run(0x77) != -1 || tr.find("Init") != std::string::npos) return 22;
    add_ret = 0; create_ret = -3; if (run(0x77) != -1 || tr.find("Sp;") != std::string::npos) return 23;
    create_ret = 6; begin_rets[0] = 0; id_in = 0; if (run(0x77) != 6 || tr.find("Find;") != std::string::npos || tr.find("A;") != std::string::npos) return 24;
    return 0;
}
