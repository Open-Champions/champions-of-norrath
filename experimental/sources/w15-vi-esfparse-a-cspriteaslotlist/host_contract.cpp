#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_csprite_aslot_list.cpp"
static std::string tr; static int begin_ret, end_ret; static unsigned short tag_in, second_in;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = second_in; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
static int count_in, rd; static int vals[8] = {0, 0, 0, 0, 0, 0, 0, 0}; alignas(16) static unsigned char spr[8192];
extern "C" int Read__9VIObjFileRi(void*, int* v) { if (rd == 0) *v = count_in; else *v = vals[rd - 1]; ++rd; tr += "I;"; return 0; }
static int run() {
    alignas(8) unsigned char parse[64] = {0}; reinterpret_cast<unsigned*>(parse)[9] = 0x55; tr.clear(); memset(spr, 0xee, sizeof spr); rd = 0;
    return ParseCSpriteASlotList__10VIESFParseR9VICSprite(parse, spr);
}
static int word(int off) { return *reinterpret_cast<int*>(spr + off); }
int main() {
    begin_ret = 0; end_ret = 0; tag_in = 0x2920; count_in = 2; vals[0] = 1; vals[1] = 50; vals[2] = 3; vals[3] = 60;
    if (run() != 0 || tr != "B;I;I;I;I;I;E;" || word(4364 + 12) != 50 || word(4364 + 36) != 60 || word(4364) != (int)0xeeeeeeee || word(4364 + 16) != (int)0xeeeeeeee) return 1;
    count_in = 0; if (run() != 0 || tr != "B;I;E;") return 2;
    count_in = -1; if (run() != 0 || tr != "B;I;E;") return 3;
    count_in = 2; end_ret = -1; if (run() != -1) return 4;
    end_ret = 0; tag_in = 0x2921; if (run() != -1 || tr != "B;") return 5;
    tag_in = 0x2920; begin_ret = -1; if (run() != -1 || tr != "B;") return 6;
    return 0;
}
