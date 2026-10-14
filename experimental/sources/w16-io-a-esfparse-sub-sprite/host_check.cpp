// Host check only; not a build input.
#include <stdint.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_sub_sprite.cpp"
static std::string tr; static int begin_ret, end_ret, simple_ret, skin_ret, lod_ret; static unsigned short tag_in;
extern "C" int ReadBegin__9VIObjFileRUsT1(void*, unsigned short* tag, unsigned short* second) { *tag = tag_in; *second = 0; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int ParseSimpleSubSpriteObj__10VIESFParse(void*) { tr += "S;"; return simple_ret; }
extern "C" int ParseSkinSubSpriteObj__10VIESFParse(void*) { tr += "K;"; return skin_ret; }
extern "C" int ParseSkinLODSpriteObj__10VIESFParse(void*) { tr += "L;"; return lod_ret; }
static int run(bool) {
    alignas(8) unsigned char parse[64] = {0};
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    words[9] = 0x55;
    words[2] = 0x77;
    words[1] = 0;
    tr.clear();
    return ParseSubSprite__10VIESFParse(parse);
}
int main() {
    // The progress callback word is a 32-bit target address, so the callback call is exercised only by the private paired run.
    begin_ret = 0; end_ret = 0; simple_ret = 4; skin_ret = 5; lod_ret = 6; tag_in = 0x2310;
    if (run(false) != 4 || tr != "B;S;") return 1;
    tag_in = 0x2320; if (run(false) != 5 || tr != "B;K;") return 2;
    tag_in = 0x2a40; if (run(false) != 6 || tr != "B;L;") return 3;
    simple_ret = -3; skin_ret = -7; lod_ret = -9;
    tag_in = 0x2310; if (run(false) != -1) return 4;
    tag_in = 0x2320; if (run(false) != -1) return 5;
    tag_in = 0x2a40; if (run(false) != -1) return 6;
    for (unsigned t : {0x2311u, 0x2300u, 0x2a41u, 0x2321u, 0u}) {
        tag_in = t; end_ret = -2; if (run(false) != -1 || tr != "B;E;") return 7;
        end_ret = 5; if (run(false) != -1 || tr != "B;E;") return 8;
    }
    begin_ret = -1; tag_in = 0x2310; if (run(false) != -1 || tr != "B;") return 9;
    return 0;
}
