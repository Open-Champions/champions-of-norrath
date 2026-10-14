#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_csprite_part_definitions.cpp"
static std::string tr; static int begin_ret[3], nbegin, count, end_ret, fail_at, calls; static unsigned short tags[3];
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; int k = nbegin < 2 ? nbegin : 2; ++nbegin; *tag = tags[k]; *second = 0; tr += "B;"; return begin_ret[k]; }
extern "C" int NumSubObjects__9VIObjFile(void*) { tr += "N;"; return count; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int ParseParticleDefinitionObj__10VIESFParse(void*) { tr += "P;"; return calls++ == fail_at ? -4 : 3; }
static int run() { alignas(8) unsigned char parse[64] = {0}; reinterpret_cast<unsigned*>(parse)[9] = 0x55; tr.clear(); calls = 0; nbegin = 0; return ParseCSpritePartDefinitions__10VIESFParse(parse); }
int main() {
    tags[0] = 0x2950; tags[1] = tags[2] = 0xc000; begin_ret[0] = begin_ret[1] = begin_ret[2] = 0; count = 2; end_ret = 0; fail_at = -1;
    if (run() != 0 || tr != "B;N;B;P;N;B;P;N;E;") return 1;
    count = 0; if (run() != 0 || tr != "B;N;E;") return 2;
    count = -1; if (run() != 0 || tr != "B;N;E;") return 3;
    count = 3; fail_at = 1; if (run() != -1 || tr != "B;N;B;P;N;B;P;") return 4;
    fail_at = -1; tags[1] = 0xc001; if (run() != -1 || tr != "B;N;B;E;") return 5;
    tags[1] = 0xc000; begin_ret[1] = -1; if (run() != -1 || tr != "B;N;B;") return 6;
    begin_ret[1] = 0; end_ret = -1; if (run() != -1) return 7;
    count = 0; if (run() != -1 || tr != "B;N;E;") return 8;
    count = 1; end_ret = 0; tags[0] = 0x2951; if (run() != -1 || tr != "B;") return 9;
    tags[0] = 0x2950; begin_ret[0] = -1; if (run() != -1 || tr != "B;") return 10;
    return 0;
}
