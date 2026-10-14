#include <stdint.h>
#include <string.h>
#include <string>
static void callback(void* arg);
static void* host_address(unsigned word) { return word == 1 ? reinterpret_cast<void*>(callback) : reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_object.cpp"

static std::string tr; static int handler_ret, begin_ret, end_ret, array_parse_ret; static unsigned short next_tag, next_second = 0x1234;
static void* last_file; static void* cb_arg; static int cb_calls;
static void callback(void* arg) { cb_arg = arg; ++cb_calls; tr += "cb;"; }
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) { last_file = f; *tag = next_tag; *second = next_second; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void* f) { if (f != last_file) tr += "BADFILE"; tr += "E;"; return end_ret; }
extern "C" int ParseSurfaceObj__10VIESFParse(void* p) { tr += "ParseSurfaceObj;"; (void)p; return handler_ret; }
extern "C" int ParseSurfaceArrayObj__10VIESFParse(void* p) { tr += "ParseSurfaceArrayObj;"; (void)p; return handler_ret; }
extern "C" int ParseMaterialPalObj__10VIESFParse(void* p) { tr += "ParseMaterialPalObj;"; (void)p; return handler_ret; }
extern "C" int ParseSimpleSpriteObj__10VIESFParse(void* p) { tr += "ParseSimpleSpriteObj;"; (void)p; return handler_ret; }
extern "C" int ParseHSpriteObj__10VIESFParse(void* p) { tr += "ParseHSpriteObj;"; (void)p; return handler_ret; }
extern "C" int ParseHSpriteAnimObj__10VIESFParse(void* p) { tr += "ParseHSpriteAnimObj;"; (void)p; return handler_ret; }
extern "C" int ParseCSpriteObj__10VIESFParse(void* p) { tr += "ParseCSpriteObj;"; (void)p; return handler_ret; }
extern "C" int ParseLODSpriteObj__10VIESFParse(void* p) { tr += "ParseLODSpriteObj;"; (void)p; return handler_ret; }
extern "C" int ParsePointLightObj__10VIESFParse(void* p) { tr += "ParsePointLightObj;"; (void)p; return handler_ret; }
extern "C" int ParseGroupSpriteObj__10VIESFParse(void* p) { tr += "ParseGroupSpriteObj;"; (void)p; return handler_ret; }
extern "C" int ParsePointSpriteObj__10VIESFParse(void* p) { tr += "ParsePointSpriteObj;"; (void)p; return handler_ret; }
extern "C" int ParseStreamAudioSpriteObj__10VIESFParse(void* p) { tr += "ParseStreamAudioSpriteObj;"; (void)p; return handler_ret; }
extern "C" int ParseFloraSpriteObj__10VIESFParse(void* p) { tr += "ParseFloraSpriteObj;"; (void)p; return handler_ret; }
extern "C" int ParseActorObj__10VIESFParse(void* p) { tr += "ParseActorObj;"; (void)p; return handler_ret; }
extern "C" int ParseFontObj__10VIESFParse(void* p) { tr += "ParseFontObj;"; (void)p; return handler_ret; }
extern "C" int ParseAdpcmObj__10VIESFParse(void* p) { tr += "ParseAdpcmObj;"; (void)p; return handler_ret; }
extern "C" int ParseXmObj__10VIESFParse(void* p) { tr += "ParseXmObj;"; (void)p; return handler_ret; }
extern "C" int ParseSoundSpriteObj__10VIESFParse(void* p) { tr += "ParseSoundSpriteObj;"; (void)p; return handler_ret; }
extern "C" int ParseParticleDefinitionObj__10VIESFParse(void* p) { tr += "ParseParticleDefinitionObj;"; (void)p; return handler_ret; }
extern "C" int ParseParticleSpriteObj__10VIESFParse(void* p) { tr += "ParseParticleSpriteObj;"; (void)p; return handler_ret; }
extern "C" int ParseSpellEffectObj__10VIESFParse(void* p) { tr += "ParseSpellEffectObj;"; (void)p; return handler_ret; }
extern "C" int ParseEffectVolumeSpriteObj__10VIESFParse(void* p) { tr += "ParseEffectVolumeSpriteObj;"; (void)p; return handler_ret; }
extern "C" int ParseWorldObj__10VIESFParsei(void* p, int flag) { (void)p; tr += "World" + std::to_string(flag) + ";"; return handler_ret; }
extern "C" int ParseResourceDirObj__10VIESFParseiR13VIResourceDir(void* p, int flag, void* a) { (void)p; tr += "Dir" + std::to_string(flag) + (a ? "" : "NULL") + ";"; return array_parse_ret; }
static unsigned char* arrayp;
extern "C" void* __t7VIArray1Z14VIResourceElem(void* a) { arrayp = (unsigned char*)a; tr += "ctor;"; return a; }
extern "C" void _$_t7VIArray1Z14VIResourceElem(void* a, int f) { tr += std::string(a == arrayp ? "dtor" : "BADARRAY") + std::to_string(f) + ";"; }

struct Entry { unsigned short tag; const char* name; };
static const Entry entries[] = {
    {0x1000, "ParseSurfaceObj"},
    {0x1001, "ParseSurfaceArrayObj"},
    {0x1110, "ParseMaterialPalObj"},
    {0x2000, "ParseSimpleSpriteObj"},
    {0x2200, "ParseHSpriteObj"},
    {0x2600, "ParseHSpriteAnimObj"},
    {0x2700, "ParseCSpriteObj"},
    {0x2a00, "ParseLODSpriteObj"},
    {0x2b00, "ParsePointLightObj"},
    {0x2c00, "ParseGroupSpriteObj"},
    {0x2d00, "ParsePointSpriteObj"},
    {0x2e00, "ParseStreamAudioSpriteObj"},
    {0x2f00, "ParseFloraSpriteObj"},
    {0x6000, "ParseActorObj"},
    {0x7000, "ParseFontObj"},
    {0xb000, "ParseAdpcmObj"},
    {0xb030, "ParseXmObj"},
    {0xb100, "ParseSoundSpriteObj"},
    {0xc000, "ParseParticleDefinitionObj"},
    {0xc100, "ParseParticleSpriteObj"},
    {0xc200, "ParseSpellEffectObj"},
    {0xc300, "ParseEffectVolumeSpriteObj"}
};

int main() {
    alignas(8) unsigned char parse[64]; memset(parse, 0, sizeof parse);
    handler_ret = 0; end_ret = 0; begin_ret = 0; array_parse_ret = 0;
    for (const Entry& e : entries) {
        for (int ret = -1; ret <= 1; ret += 2) {
            tr.clear(); handler_ret = ret; next_tag = e.tag;
            int r = ParseObject__10VIESFParse(parse);
            if (tr != std::string("B;") + e.name + ";") return 1;
            if (r != (ret < 0 ? -1 : 0)) return 2;
        }
    }
    // tag 0x8000 passes flag 0; 0xa000 constructs, parses, destroys with flag 2 and returns by the parse result
    tr.clear(); handler_ret = 3; next_tag = 0x8000;
    if (ParseObject__10VIESFParse(parse) != 0 || tr != "B;World0;") return 3;
    handler_ret = -3; tr.clear();
    if (ParseObject__10VIESFParse(parse) != -1) return 4;
    tr.clear(); next_tag = 0xa000; array_parse_ret = 0;
    if (ParseObject__10VIESFParse(parse) != 0 || tr != "B;ctor;Dir0;dtor2;") return 5;
    tr.clear(); array_parse_ret = -4;
    if (ParseObject__10VIESFParse(parse) != -1 || tr != "B;ctor;Dir0;dtor2;") return 6;
    // end_ret decides for an unknown tag (the handler result is ignored by the end step)
    unsigned short others[] = {0, 0x0fff, 0x1002, 0x1111, 0x2001, 0x2601, 0x2b01, 0x2e01, 0x6001, 0x7001, 0x8001, 0xa001, 0xb001, 0xb031, 0xc001, 0xc101, 0xc301, 0xffff};
    for (unsigned short tag : others) {
        for (int ret = -1; ret <= 1; ret += 2) {
            tr.clear(); end_ret = ret; handler_ret = 9; next_tag = tag;
            if (ParseObject__10VIESFParse(parse) != (ret < 0 ? -1 : 0) || tr != "B;E;") return 7;
        }
    }
    // the callback word (here 1 = the host callback) is called with the argument word before ReadBegin,
    // and the file word is handed to ReadBegin and ReadEnd
    *(unsigned*)(parse + 4) = 1; *(unsigned*)(parse + 8) = 0x1234; *(unsigned*)(parse + 36) = 0x77;
    tr.clear(); begin_ret = 0; next_tag = 0x7777; end_ret = 0;
    if (ParseObject__10VIESFParse(parse) != 0 || tr != "cb;B;E;" || cb_calls != 1 || cb_arg != (void*)(uintptr_t)0x1234 || last_file != (void*)(uintptr_t)0x77) return 9;
    *(unsigned*)(parse + 4) = 0; cb_calls = 0;
    ParseObject__10VIESFParse(parse);
    if (cb_calls != 0) return 10;
    // a negative ReadBegin result returns -1 before any dispatch
    tr.clear(); begin_ret = -1; next_tag = 0x1000; handler_ret = 0;
    if (ParseObject__10VIESFParse(parse) != -1 || tr != "B;") return 8;
    return 0;
}
