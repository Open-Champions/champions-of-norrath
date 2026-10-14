static unsigned char arena[1 << 12];
static unsigned callback_word, callback_argument;
static int callback_calls;
#define NORRATH_WORD_ADDRESS(word) (arena + static_cast<unsigned>(word))
#define NORRATH_CALL_WORD(function_word, argument) (++callback_calls, callback_word = (function_word), callback_argument = (argument))
#include "parse_zone_resource.cpp"

#include <cstdio>
#include <cstring>
#include <string>

static std::string trace;
static unsigned short tag_queue[8];
static unsigned tag_index;
static int begin_result, end_result, sub_objects, parse_result, surface_result;

extern "C" int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second) {
    if (file != arena + 0x55) trace += "BADFILE";
    *tag = tag_index < 8 ? tag_queue[tag_index++] : 0xffff;
    *second = 0;
    trace += "B;";
    return begin_result;
}
extern "C" int ReadEnd__9VIObjFile(void*) { trace += "E;"; return end_result; }
extern "C" int NumSubObjects__9VIObjFile(void*) { trace += "N;"; return sub_objects; }
extern "C" int ParseSurfaceObj__10VIESFParse(void*) { trace += "S;"; return surface_result; }
#define PARSE(name, label) extern "C" int Parse##name##__10VIESFParse(void*) { trace += label; return parse_result; }
PARSE(SimpleSpriteObj, "ss;") PARSE(HSpriteObj, "hs;") PARSE(SimpleSubSpriteObj, "sb;") PARSE(CSpriteObj, "cs;")
PARSE(LODSpriteObj, "lod;") PARSE(PointLightObj, "pl;") PARSE(GroupSpriteObj, "gs;") PARSE(SoundSpriteObj, "snd;")
PARSE(ParticleSpriteObj, "pa;") PARSE(PointSpriteObj, "pt;") PARSE(StreamAudioSpriteObj, "sa;") PARSE(FloraSpriteObj, "fl;")
PARSE(EffectVolumeSpriteObj, "ev;") PARSE(ActorObj, "ac;") PARSE(ZoneBaseObj, "zb;") PARSE(ZoneRoomActorsObj, "ra;")
PARSE(ZoneRoomStaticLightingsObj, "rl;") PARSE(MaterialPalObj, "mp;")

static int run(unsigned short tag, int parse, int flag = 1, int end = 0) {
    unsigned* words = reinterpret_cast<unsigned*>(arena + 0x100);
    std::memset(arena, 0, sizeof arena);
    words[1] = 0; words[3] = flag; words[9] = 0x55;
    trace.clear();
    tag_queue[0] = tag; tag_queue[1] = tag_queue[2] = tag_queue[3] = 0x1000;
    tag_index = 0; begin_result = 0; end_result = end; parse_result = parse; surface_result = 0; sub_objects = 2;
    callback_calls = 0;
    return ParseZoneResource__10VIESFParse(arena + 0x100);
}

int main() {
    struct Row { unsigned short tag; const char* label; bool pass_through; };
    const Row rows[] = {
        {0x2000, "ss;", true}, {0x2200, "hs;", true}, {0x2310, "sb;", true}, {0x2700, "cs;", true}, {0x2a00, "lod;", true},
        {0x2b00, "pl;", true}, {0x2c00, "gs;", true}, {0xb100, "snd;", true}, {0xc100, "pa;", true},
        {0x2d00, "pt;", false}, {0x2e00, "sa;", false}, {0x2f00, "fl;", false}, {0xc300, "ev;", false}, {0x6000, "ac;", false},
        {0x3200, "zb;", false}, {0x3270, "ra;", false}, {0x3290, "rl;", false}, {0x1110, "mp;", true},
    };
    for (const Row& row : rows) {
        if (run(row.tag, 6) != (row.pass_through ? 6 : 0)) return 1;
        if (trace != std::string("B;") + row.label) return 2;
        if (run(row.tag, -2) != -1) return 3;
    }
    // Skipped blocks end with ReadEnd.
    if (run(0x1110, 6, 0) != 0 || trace != "B;E;") return 4;
    if (run(0x1110, 6, 0, -1) != -1) return 5;
    if (run(0x7777, 6) != 0 || trace != "B;E;") return 6;
    if (run(0x7777, 6, 1, -4) != -1) return 7;
    // Surface list.
    if (run(0x1001, 0) != 0 || trace != "B;N;B;S;N;B;S;N;E;") return 8;
    if (run(0x1001, 0, 0) != 0 || trace != "B;E;") return 9;
    sub_objects = 2;
    tag_queue[0] = 0x1001;
    // A wrong item tag stops the list.
    {
        unsigned* words = reinterpret_cast<unsigned*>(arena + 0x100);
        words[3] = 1;
        trace.clear(); tag_index = 0; begin_result = 0; end_result = 0; sub_objects = 2; surface_result = 0;
        tag_queue[0] = 0x1001; tag_queue[1] = 0x1002;
        if (ParseZoneResource__10VIESFParse(arena + 0x100) != -1 || trace != "B;N;B;") return 10;
        trace.clear(); tag_index = 0; surface_result = -1; tag_queue[1] = 0x1000;
        if (ParseZoneResource__10VIESFParse(arena + 0x100) != -1 || trace != "B;N;B;S;") return 11;
        trace.clear(); tag_index = 0; surface_result = 0; end_result = -1;
        if (ParseZoneResource__10VIESFParse(arena + 0x100) != -1) return 12;
    }
    // Begin failure.
    {
        unsigned* words = reinterpret_cast<unsigned*>(arena + 0x100);
        words[3] = 1;
        trace.clear(); tag_index = 0; begin_result = -3;
        if (ParseZoneResource__10VIESFParse(arena + 0x100) != -1 || trace != "B;") return 13;
    }
    // Progress callback.
    {
        unsigned* words = reinterpret_cast<unsigned*>(arena + 0x100);
        trace.clear(); tag_index = 0; begin_result = 0; end_result = 0; parse_result = 1; callback_calls = 0;
        words[1] = 0x1234; words[2] = 0x99;
        tag_queue[0] = 0x2000;
        if (ParseZoneResource__10VIESFParse(arena + 0x100) != 1 || callback_calls != 1 || callback_word != 0x1234 || callback_argument != 0x99) return 14;
    }
    std::puts("esfparse zone-resource passed");
    return 0;
}
