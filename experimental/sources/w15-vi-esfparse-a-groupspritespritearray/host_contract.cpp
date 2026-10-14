#include <stdint.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_group_sprite_sprite_array.cpp"

static std::string tr; static int begin_ret, count, end_ret, fail_at, calls; static unsigned short tag_in;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = 0; tr += "B;"; return begin_ret; }
extern "C" int NumSubObjects__9VIObjFile(void*) { tr += "N;"; return count; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int ParseSprite__10VIESFParse(void*) { tr += "S;"; return calls++ == fail_at ? -1 : 0; }
static int run() { alignas(8) unsigned char parse[64] = {0}; reinterpret_cast<unsigned*>(parse)[9] = 0x55; tr.clear(); calls = 0; return ParseGroupSpriteSpriteArray__10VIESFParse(parse); }
int main() {
    tag_in = 0x2c20; begin_ret = 0; count = 3; end_ret = 0; fail_at = -1;
    if (run() != 0 || tr != "B;N;S;S;S;E;") return 1;
    count = 0; if (run() != 0 || tr != "B;N;E;") return 2;
    count = -2; if (run() != 0 || tr != "B;N;E;") return 3;
    count = 3; fail_at = 1; if (run() != -1 || tr != "B;N;S;S;") return 4;
    fail_at = -1; end_ret = -1; if (run() != -1) return 5;
    end_ret = 0; tag_in = 0x2c21; if (run() != -1 || tr != "B;") return 6;
    tag_in = 0x2c20; begin_ret = -1; if (run() != -1 || tr != "B;") return 7;
    return 0;
}
