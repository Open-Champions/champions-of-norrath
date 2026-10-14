#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_hsprite_anim_array.cpp"
static std::string tr; static int begin_ret[2], nbegin, count, end_ret, fail_at, calls; static unsigned short tags[2];
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; int k = nbegin++ ? 1 : 0; *tag = tags[k]; *second = 0; tr += "B;"; return begin_ret[k]; }
extern "C" int NumSubObjects__9VIObjFile(void*) { tr += "N;"; return count; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int ParseHSpriteAnimObj__10VIESFParse(void*) { tr += "A;"; return calls++ == fail_at ? -4 : 3; }
static int run() { alignas(8) unsigned char parse[64] = {0}; reinterpret_cast<unsigned*>(parse)[9] = 0x55; tr.clear(); calls = 0; nbegin = 0; return ParseHSpriteAnimArray__10VIESFParse(parse); }
int main() {
    tags[0] = 0x2610; tags[1] = 0x2600; begin_ret[0] = begin_ret[1] = 0; count = 2; end_ret = 0; fail_at = -1;
    if (run() != 0 || tr != "B;N;B;A;B;A;E;") return 1;
    count = 0; if (run() != 0 || tr != "B;N;E;") return 2;
    count = 3; fail_at = 1; if (run() != -1 || tr != "B;N;B;A;B;A;") return 3;
    fail_at = -1; tags[1] = 0x2601; if (run() != -1 || tr != "B;N;B;") return 4;
    tags[1] = 0x2600; begin_ret[1] = -1; if (run() != -1 || tr != "B;N;B;") return 5;
    begin_ret[1] = 0; end_ret = -1; if (run() != -1) return 6;
    end_ret = 0; tags[0] = 0x2611; if (run() != -1 || tr != "B;") return 7;
    tags[0] = 0x2610; begin_ret[0] = -1; if (run() != -1 || tr != "B;") return 8;
    return 0;
}
