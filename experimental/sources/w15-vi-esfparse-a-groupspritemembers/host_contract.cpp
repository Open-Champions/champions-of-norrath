#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_group_sprite_members.cpp"
static std::string tr; static int begin_ret, end_ret; static unsigned short tag_in, second_in;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = second_in; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
static int count_in, find_ret, share_ret, rd_u, rd_f, fail_find, find_calls; alignas(16) static unsigned char grp[1024];
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = count_in; tr += "I;"; return 0; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = 100 + rd_u++; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = 1.0f + rd_f++; tr += "F;"; return 0; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30)) tr += "BADFIND"; *t = 77; *h = 500 + (int)id; tr += "Find;"; return find_calls++ == fail_find ? 0 : find_ret; }
extern "C" int ShareSprite__7VIScenei(void* s, int h) { if (s != reinterpret_cast<void*>(0x20)) tr += "BADSCENE"; tr += "Sh" + std::to_string(h) + ";"; return share_ret; }
static int run() {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[6] = 0x20; w[8] = 0x30; w[9] = 0x55; tr.clear(); memset(grp, 0xee, sizeof grp); rd_u = rd_f = find_calls = 0;
    return ParseGroupSpriteMembers__10VIESFParseR13VIGroupSprite(parse, grp);
}
static int w(int off) { return *reinterpret_cast<int*>(grp + off); }
int main() {
    begin_ret = 0; end_ret = 0; tag_in = 0x2c30; count_in = 2; find_ret = 1; share_ret = 0; fail_find = -1;
    if (run() != 0 || tr != "B;I;U;Find;Sh600;F;F;F;F;F;F;F;U;Find;Sh601;F;F;F;F;F;F;F;E;") return 1;
    if (w(64) != 2 || w(68) != 600 || w(68 + 32) != 601 || w(0) != (int)0xeeeeeeee) return 2;
    const float* f = reinterpret_cast<const float*>(grp + 72);
    for (int k = 0; k < 7; ++k) if (f[k] != 1.0f + k || f[8 + k] != 8.0f + k) return 3;
    count_in = 0; if (run() != 0 || tr != "B;I;E;") return 4;
    count_in = -2; if (run() != 0 || tr != "B;I;E;") return 5;
    count_in = 2; fail_find = 1; if (run() != -1 || tr.find("E;") != std::string::npos || find_calls != 2) return 6;
    fail_find = -1; share_ret = -1; if (run() != -1 || tr != "B;I;U;Find;Sh600;") return 7;
    share_ret = 0; end_ret = -1; if (run() != -1) return 8;
    end_ret = 0; tag_in = 0x2c31; if (run() != -1 || tr != "B;") return 9;
    tag_in = 0x2c30; begin_ret = -1; if (run() != -1 || tr != "B;") return 10;
    return 0;
}
