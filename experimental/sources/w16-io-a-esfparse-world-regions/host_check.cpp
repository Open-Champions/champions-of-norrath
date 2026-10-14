// Host check only; not a build input.
#include <string>
static unsigned char arena[0x400];
#define NORRATH_WORD_ADDRESS(word) (static_cast<void*>(arena + (word)))
#include "parse_world_regions.cpp"
static std::string tr; static int begin_ret, end_ret; static unsigned short tag_in; static int reads;
extern "C" int ReadBegin__9VIObjFileRUsT1(void*, unsigned short* tag, unsigned short* second) { *tag = tag_in; *second = 0; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int Read__9VIObjFileRUc(void*, unsigned char* value) {
    tr += value == arena + 0x100 + reads ? "r;" : "r?;";
    *value = static_cast<unsigned char>(0x40 + reads);
    ++reads;
    return 0;
}
static int run(unsigned char* parse, unsigned char* world) { tr.clear(); reads = 0; return ParseWorldRegions__10VIESFParseR7VIWorld(parse, world); }
int main() {
    alignas(8) unsigned char parse[64] = {0};
    reinterpret_cast<unsigned*>(parse)[9] = 0x55;
    alignas(8) unsigned char world[96] = {0};
    *reinterpret_cast<unsigned*>(world + 84) = 0x100;
    begin_ret = 0; end_ret = 0; tag_in = 0x8240;
    *reinterpret_cast<int*>(world + 80) = 3;
    if (run(parse, world) != 0 || tr != "B;r;r;r;E;" || arena[0x100] != 0x40 || arena[0x102] != 0x42 || arena[0x103] != 0) return 1;
    *reinterpret_cast<int*>(world + 80) = 0;
    if (run(parse, world) != 0 || tr != "B;E;") return 2;
    *reinterpret_cast<int*>(world + 80) = -3;
    if (run(parse, world) != 0 || tr != "B;E;") return 3;
    *reinterpret_cast<int*>(world + 80) = 1;
    end_ret = -1;
    if (run(parse, world) != -1 || tr != "B;r;E;") return 4;
    end_ret = 0; tag_in = 0x8241;
    if (run(parse, world) != -1 || tr != "B;") return 5;
    tag_in = 0x8240; begin_ret = -1;
    if (run(parse, world) != -1 || tr != "B;") return 6;
    return 0;
}
