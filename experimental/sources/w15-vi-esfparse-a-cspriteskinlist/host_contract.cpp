#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_csprite_skin_list.cpp"
static std::string tr; static int begin_ret, end_ret; static unsigned short tag_in, second_in;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = second_in; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
static int count_in, find_ret, rd, ids; static unsigned id_val[4]; static int slot_val[4] = {1, 3, 0, 0}; alignas(16) static unsigned char spr[8192];
extern "C" int Read__9VIObjFileRi(void*, int* v) { if (rd == 0) *v = count_in; else *v = slot_val[rd - 1]; ++rd; tr += "I;"; return 0; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_val[ids++ % 4]; tr += "U;"; return 0; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30)) tr += "BADFIND"; *t = 99; if (find_ret) *h = 700 + (int)id; tr += "Find;"; return find_ret; }
static int run() {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[8] = 0x30; w[9] = 0x55; tr.clear(); memset(spr, 0xee, sizeof spr); rd = 0; ids = 0;
    return ParseCSpriteSkinList__10VIESFParseR9VICSprite(parse, spr);
}
static int slot(int i) { return *reinterpret_cast<int*>(spr + 3856 + 4 * i); }
int main() {
    begin_ret = 0; end_ret = 0; tag_in = 0x2900; count_in = 2; find_ret = 1; id_val[0] = 5; id_val[1] = 0; id_val[2] = 9; id_val[3] = 0;
    if (run() != 0 || tr != "B;I;U;I;Find;U;I;E;") return 1;
    if (slot(1) != 705 || slot(3) != -1 || slot(0) != (int)0xeeeeeeee) return 2;
    find_ret = 0; if (run() != -1 || tr != "B;I;U;I;Find;") return 3;
    count_in = 0; if (run() != 0 || tr != "B;I;E;") return 4;
    count_in = -3; if (run() != 0 || tr != "B;I;E;") return 5;
    count_in = 2; find_ret = 1; end_ret = -1; if (run() != -1) return 6;
    end_ret = 0; tag_in = 0x2901; if (run() != -1 || tr != "B;") return 7;
    tag_in = 0x2900; begin_ret = -1; if (run() != -1 || tr != "B;") return 8;
    return 0;
}
