#include <stdint.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_hsprite_anim.cpp"

static std::string tr; static int begin_ret, obj_ret; static unsigned short tag_in;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = 0; tr += "B;"; return begin_ret; }
extern "C" int ParseHSpriteAnimObj__10VIESFParse(void* p) { tr += "O;"; (void)p; return obj_ret; }
static int run() { alignas(8) unsigned char parse[64] = {0}; reinterpret_cast<unsigned*>(parse)[9] = 0x55; tr.clear(); return ParseHSpriteAnim__10VIESFParse(parse); }
int main() {
    tag_in = 0x2600; begin_ret = 0; obj_ret = 7;
    if (run() != 7 || tr != "B;O;") return 1;
    obj_ret = 0; if (run() != 0) return 2;
    obj_ret = -5; if (run() != -1 || tr != "B;O;") return 3;
    obj_ret = 7; tag_in = 0x2601; if (run() != -1 || tr != "B;") return 4;
    tag_in = 0x2600; begin_ret = -1; if (run() != -1 || tr != "B;") return 5;
    return 0;
}
