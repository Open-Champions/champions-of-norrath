#include <stdint.h>
#include <string.h>
#include <string>
alignas(16) static unsigned char arena[0x2000];
static void* host_address(unsigned word) { return word ? static_cast<void*>(arena + word) : nullptr; }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_hsprite_triggers.cpp"
static std::string tr; static int begin_ret, end_ret; static unsigned short tag_in;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != host_address(0x55)) tr += "BADFILE"; *tag = tag_in; *second = 0; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
static int count_in, init_ret, rd; static unsigned char* spr = arena + 0x1000;
extern "C" int Read__9VIObjFileRi(void*, int* v) { if (rd == 0) *v = count_in; else *v = 1000 + rd; ++rd; tr += "I;"; return 0; }
extern "C" int Init__t7VIArray1Z16VIHSpriteTriggeri(void* a, int c) { if (a != spr + 88 || c != count_in) tr += "BADINIT"; tr += "In;"; return init_ret; }
static int run() {
    alignas(8) unsigned char parse[64] = {0}; reinterpret_cast<unsigned*>(parse)[9] = 0x55; tr.clear(); rd = 0; memset(arena, 0xee, 0x1000);
    *reinterpret_cast<unsigned*>(spr + 88 + 4) = 0x200;
    return ParseHSpriteTriggers__10VIESFParseR9VIHSprite(parse, spr);
}
int main() {
    begin_ret = 0; end_ret = 0; init_ret = 0; count_in = 3; tag_in = 0x2450;
    if (run() != 0 || tr != "B;I;In;I;I;I;E;") return 1;
    const int* e = reinterpret_cast<const int*>(arena + 0x200);
    if (e[0] != 1001 || e[4] != 1002 || e[8] != 1003 || e[1] != (int)0xeeeeeeee || e[5] != (int)0xeeeeeeee || e[12] != (int)0xeeeeeeee) return 2;
    count_in = 0; if (run() != 0 || tr != "B;I;In;E;") return 3;
    count_in = -2; if (run() != 0 || tr != "B;I;In;E;") return 4;
    count_in = 3; init_ret = -1; if (run() != -1 || tr != "B;I;In;") return 5;
    init_ret = 0; end_ret = -1; if (run() != -1) return 6;
    end_ret = 0; tag_in = 0x2451; if (run() != -1 || tr != "B;") return 7;
    tag_in = 0x2450; begin_ret = -1; if (run() != -1 || tr != "B;") return 8;
    return 0;
}
