// Host check only; not a build input.
#include <string>
static unsigned char arena[0x2000];
#define NORRATH_WORD_ADDRESS(word) (static_cast<void*>(arena + (word)))
#include "parse_world_zone_proxies.cpp"
static std::string tr; static int begin_ret, end_ret, init_ret, countIn, initCalls; static unsigned short tag_in, second_in;
static unsigned char* zoneBase;
static int ulReads, uiReads, floatReads, stringReads;
static std::string at(void* p, int off) { return p == zoneBase + off ? "" : "?"; }
extern "C" int ReadBegin__9VIObjFileRUsT1(void*, unsigned short* tag, unsigned short* second) { *tag = tag_in; *second = second_in; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int Read__9VIObjFileRi(void*, int* value) { *value = countIn; tr += "n;"; return 0; }
extern "C" int Init__6VIZone(void* zone) { zoneBase = static_cast<unsigned char*>(zone) - 0; tr += "Z" + std::to_string(static_cast<unsigned char*>(zone) - (arena + 0x1000)) + ";"; ++initCalls; return init_ret; }
extern "C" int Read__9VIObjFileRUl(void*, unsigned long long* v) { tr += "l" + std::to_string(reinterpret_cast<unsigned char*>(v) - zoneBase) + ";"; return 0; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { tr += "u" + std::to_string(reinterpret_cast<unsigned char*>(v) - zoneBase) + ";"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { tr += "f" + std::to_string(reinterpret_cast<unsigned char*>(v) - zoneBase) + ";"; return 0; }
extern "C" int ReadString__9VIObjFilePcUs(void*, char*, unsigned short size) { tr += "s" + std::to_string(size) + ";"; return 0; }
static const char* kRecord = "l8;l16;u24;f32;f36;f40;f44;f48;f52;f56;f60;f64;";
static int run(unsigned char* parse, unsigned char* world) { tr.clear(); initCalls = 0; return ParseWorldZoneProxies__10VIESFParseR7VIWorld(parse, world); }
int main() {
    alignas(8) unsigned char parse[64] = {0};
    reinterpret_cast<unsigned*>(parse)[9] = 0x55;
    alignas(8) unsigned char world[96] = {0};
    *reinterpret_cast<unsigned*>(world + 76) = 0x1000;
    begin_ret = 0; end_ret = 0; init_ret = 0; tag_in = 0x8210; second_in = 0; countIn = 2;
    std::string rec = kRecord;
    if (run(parse, world) != 0 || tr != "B;n;Z0;" + rec + "Z592;" + rec + "E;") return 1;
    second_in = 1;
    if (run(parse, world) != 0 || tr != "B;n;Z0;" + rec + "s31;Z592;" + rec + "s31;E;") return 2;
    second_in = 2;
    if (run(parse, world) != 0 || tr.find('s') != std::string::npos) return 3;
    countIn = 0;
    if (run(parse, world) != 0 || tr != "B;n;E;") return 4;
    countIn = -1;
    if (run(parse, world) != 0 || tr != "B;n;E;") return 5;
    countIn = 3; init_ret = -1;
    if (run(parse, world) != -1 || tr != "B;n;Z0;" || initCalls != 1) return 6;
    init_ret = 0; end_ret = -1; countIn = 1;
    if (run(parse, world) != -1) return 7;
    end_ret = 0; tag_in = 0x8211;
    if (run(parse, world) != -1 || tr != "B;") return 8;
    tag_in = 0x8210; begin_ret = -1;
    if (run(parse, world) != -1 || tr != "B;") return 9;
    return 0;
}
