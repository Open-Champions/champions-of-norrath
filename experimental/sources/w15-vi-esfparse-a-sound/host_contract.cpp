#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_sound.cpp"
static std::string tr; static int begin_ret, end_ret; static unsigned short tag_in;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = 0; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
static int adpcm_ret, xm_ret;
extern "C" int ParseAdpcmObj__10VIESFParse(void*) { tr += "A;"; return adpcm_ret; }
extern "C" int ParseXmObj__10VIESFParse(void*) { tr += "X;"; return xm_ret; }
static int run() { alignas(8) unsigned char parse[64] = {0}; reinterpret_cast<unsigned*>(parse)[9] = 0x55; tr.clear(); return ParseSound__10VIESFParse(parse); }
int main() {
    begin_ret = 0; end_ret = 0; adpcm_ret = 4; xm_ret = 9; tag_in = 0xb000;
    if (run() != 4 || tr != "B;A;") return 1;
    tag_in = 0xb030; if (run() != 9 || tr != "B;X;") return 2;
    adpcm_ret = -3; xm_ret = -7; tag_in = 0xb000; if (run() != -1) return 3;
    tag_in = 0xb030; if (run() != -1) return 4;
    for (unsigned t : {0xb001u, 0xb010u, 0xb020u, 0xb031u, 0u}) { tag_in = t; end_ret = -2; if (run() != -1 || tr != "B;E;") return 5; end_ret = 5; if (run() != -1 || tr != "B;E;") return 6; }
    tag_in = 0xb000; begin_ret = -1; if (run() != -1 || tr != "B;") return 7;
    return 0;
}
