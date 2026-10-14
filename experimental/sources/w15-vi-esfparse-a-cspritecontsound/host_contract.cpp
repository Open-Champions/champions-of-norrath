#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_csprite_cont_sound.cpp"
static std::string tr; static int begin_ret, end_ret; static unsigned short tag_in, second_in;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = second_in; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
static unsigned id_in; static int find_ret, find_handle, dev;
alignas(16) static unsigned char spr[8192];
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = 2.5f; tr += "F;"; return 0; }
extern "C" int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* d, unsigned id, int t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || id != id_in || t != 14) tr += "BADFIND"; if (find_ret) *h = find_handle; tr += "Find;"; return find_ret; }
static int run() {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[4] = dev; w[8] = 0x30; w[9] = 0x55; tr.clear(); memset(spr, 0xee, sizeof spr);
    return ParseCSpriteContSound__10VIESFParseR9VICSprite(parse, spr);
}
static int stored() { return *reinterpret_cast<int*>(spr + 4592); }
int main() {
    begin_ret = 0; end_ret = 0; tag_in = 0x2940; id_in = 5; find_ret = 1; find_handle = 33; dev = 1;
    if (run() != 0 || tr != "B;U;Find;F;E;" || stored() != 33 || *reinterpret_cast<float*>(spr + 4596) != 2.5f) return 1;
    find_ret = 0; if (run() != -1 || tr != "B;U;Find;" || stored() != (int)0xeeeeeeee) return 2;
    find_ret = 1; dev = 0; if (run() != 0 || tr != "B;U;F;E;" || stored() != -1) return 3;
    dev = 1; id_in = 0; if (run() != 0 || tr != "B;U;F;E;" || stored() != -1) return 4;
    id_in = 5; end_ret = -1; if (run() != -1 || stored() != 33) return 5;
    end_ret = 0; tag_in = 0x2941; if (run() != -1 || tr != "B;") return 6;
    tag_in = 0x2940; begin_ret = -1; if (run() != -1 || tr != "B;") return 7;
    return 0;
}
