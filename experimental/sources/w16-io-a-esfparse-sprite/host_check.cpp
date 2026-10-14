// Host check only; not a build input.
#include <stdint.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_sprite.cpp"
static std::string tr; static int begin_ret, end_ret; static unsigned short tag_in;
static int kind_ret;
extern "C" int ReadBegin__9VIObjFileRUsT1(void*, unsigned short* tag, unsigned short* second) { *tag = tag_in; *second = 0; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
#define KIND(name, mark) extern "C" int Parse##name##__10VIESFParse(void*) { tr += mark; return kind_ret; }
KIND(SimpleSpriteObj, "a;") KIND(HSpriteObj, "b;") KIND(CSpriteObj, "c;") KIND(LODSpriteObj, "d;") KIND(PointLightObj, "e;")
KIND(GroupSpriteObj, "f;") KIND(PointSpriteObj, "g;") KIND(StreamAudioSpriteObj, "h;") KIND(FloraSpriteObj, "i;")
KIND(SoundSpriteObj, "j;") KIND(ParticleSpriteObj, "k;") KIND(EffectVolumeSpriteObj, "l;")
static int run() { alignas(8) unsigned char parse[64] = {0}; reinterpret_cast<unsigned*>(parse)[9] = 0x55; tr.clear(); return ParseSprite__10VIESFParse(parse); }
int main() {
    static const struct { unsigned short tag; const char* mark; } table[] = {
        {0x2000, "a;"}, {0x2200, "b;"}, {0x2700, "c;"}, {0x2a00, "d;"}, {0x2b00, "e;"}, {0x2c00, "f;"},
        {0x2d00, "g;"}, {0x2e00, "h;"}, {0x2f00, "i;"}, {0xb100, "j;"}, {0xc100, "k;"}, {0xc300, "l;"}};
    begin_ret = 0; end_ret = 0;
    for (int i = 0; i < 12; ++i) {
        tag_in = table[i].tag;
        kind_ret = 10 + i;
        if (run() != 10 + i || tr != std::string("B;") + table[i].mark) return 1 + i;
        kind_ret = -4;
        if (run() != -1) return 20 + i;
    }
    for (unsigned t : {0u, 0x2001u, 0x2100u, 0x2800u, 0x2c01u, 0x2f01u, 0xb000u, 0xc000u, 0xc200u, 0xffffu}) {
        tag_in = static_cast<unsigned short>(t);
        end_ret = -2; if (run() != -1 || tr != "B;E;") return 40;
        end_ret = 5; if (run() != -1 || tr != "B;E;") return 41;
    }
    begin_ret = -1; tag_in = 0x2000; if (run() != -1 || tr != "B;") return 42;
    return 0;
}
