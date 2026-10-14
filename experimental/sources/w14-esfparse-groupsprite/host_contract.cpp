#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_group_sprite_obj.cpp"

static std::string tr; static unsigned id_in; static int find_ret, find_type, find_handle, create_ret, add_ret, end_rets[2], end_calls, begin_ret, array_ret, members_ret, float_calls;
static unsigned short tag_in; static GroupBox box_seen; static void* sphere_seen; static void* members_sprite;
alignas(16) static unsigned char sprite_mem[128];
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = 0; tr += "B;"; return begin_ret; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = 1.0f + float_calls++; tr += "F;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_rets[end_calls++]; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int CreateSprite__7VIScene12VISpriteTypei(void* s, int t, int a) { if (s != reinterpret_cast<void*>(0x20) || t != 11 || a != 0x99) tr += "BADCREATE"; tr += "C;"; return create_ret; }
extern "C" void* Sprite__7VIScenei(void*, int) { tr += "Sp;"; return sprite_mem; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) {
    if (r != sprite_mem || id != id_in || t != 20 || h != create_ret) tr += "BADADD"; tr += "A;"; return add_ret; }
extern "C" void Init__8VISphereRC6VIBBox(void* s, const GroupBox* b) { sphere_seen = s; box_seen = *b; tr += "Init;"; }
extern "C" int ParseGroupSpriteSpriteArray__10VIESFParse(void*) { tr += "Arr;"; return array_ret; }
extern "C" int ParseGroupSpriteMembers__10VIESFParseR13VIGroupSprite(void*, void* s) { members_sprite = s; tr += "Mem;"; return members_ret; }

static int run() {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[6] = 0x20; w[8] = 0x30; w[9] = 0x55; tr.clear(); end_calls = 0; float_calls = 0; members_sprite = nullptr; sphere_seen = nullptr;
    memset(sprite_mem, 0xee, sizeof sprite_mem);
    return ParseGroupSpriteObj__10VIESFParse(parse);
}
int main() {
    id_in = 7; find_ret = 0; find_type = 20; find_handle = 11; create_ret = 6; add_ret = 0; array_ret = 0; members_ret = 0; begin_ret = 0; tag_in = 0x2c10;
    end_rets[0] = end_rets[1] = 0;
    const char* full = "B;U;F;F;F;F;F;F;E;Find;C;Sp;A;Init;Arr;Mem;E;";
    if (run() != 6 || tr != full) return 1;
    const float* lo = reinterpret_cast<const float*>(sprite_mem + 8); const float* hi = reinterpret_cast<const float*>(sprite_mem + 20);
    if (lo[0] != 1.0f || lo[1] != 2.0f || lo[2] != 3.0f || hi[0] != 4.0f || hi[1] != 5.0f || hi[2] != 6.0f) return 2;
    for (int i = 0; i < 6; ++i) if (box_seen.v[i] != 1.0f + i) return 3;
    if (sphere_seen != sprite_mem + 32 || members_sprite != sprite_mem || sprite_mem[0] != 0xee || sprite_mem[7] != 0xee || sprite_mem[32] != 0xee) return 4;
    find_ret = 1; if (run() != 11 || tr != "B;U;F;F;F;F;F;F;E;Find;E;") return 5;
    find_type = 3; if (run() != -1 || tr != "B;U;F;F;F;F;F;F;E;Find;") return 6;
    find_type = 20; find_ret = 0;
    end_rets[0] = -1; if (run() != -1 || tr != "B;U;F;F;F;F;F;F;E;") return 7;
    end_rets[0] = 0; add_ret = -1; if (run() != -1 || tr.find("Init") != std::string::npos) return 8;
    add_ret = 0; create_ret = -3; if (run() != -1 || tr.find("Sp;") != std::string::npos) return 9;
    create_ret = 6; array_ret = -1; if (run() != -1 || tr.find("Mem") != std::string::npos) return 10;
    array_ret = 0; members_ret = -1; if (run() != -1 || tr.substr(tr.size() - 4) != "Mem;") return 11;
    members_ret = 0; end_rets[1] = -1; if (run() != -1) return 12;
    end_rets[1] = 0; tag_in = 0x2c11; if (run() != -1 || tr != "B;") return 13;
    tag_in = 0x2c10; begin_ret = -1; if (run() != -1 || tr != "B;") return 14;
    begin_ret = 0; id_in = 0; if (run() != 6 || tr.find("Find") != std::string::npos || tr.find("A;") != std::string::npos) return 15;
    return 0;
}
