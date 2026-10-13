#include <stdint.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_world_obj.cpp"

static std::string tr; static int begin_ret, base_ret, zones_ret; static int end_rets[2], end_calls; static unsigned short next_tag;
static uintptr_t world_seen;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = next_tag; *second = 0x1234; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void* f) { if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; tr += "E;"; return end_rets[end_calls++]; }
extern "C" int ParseWorldBase__10VIESFParseR7VIWorld(void* p, void* w) { (void)p; world_seen = reinterpret_cast<uintptr_t>(w); tr += "Base;"; return base_ret; }
extern "C" int ParseWorldZones__10VIESFParseR7VIWorld(void* p, void* w) { (void)p; if (reinterpret_cast<uintptr_t>(w) != world_seen) tr += "BADWORLD"; tr += "Zones;"; return zones_ret; }

static int run(int flag) { alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse); w[9] = 0x55; w[6] = 0x1000; end_calls = 0; tr.clear(); return ParseWorldObj__10VIESFParsei(parse, flag); }
int main() {
    begin_ret = 0; base_ret = 0; zones_ret = 0; end_rets[0] = end_rets[1] = 0; next_tag = 0x8100;
    if (run(0) != 0 || tr != "B;E;Base;E;Zones;" || world_seen != 0x1004) return 1;
    if (run(7) != 0 || tr != "B;E;Base;E;") return 2;
    zones_ret = -1; if (run(0) != -1) return 3;
    zones_ret = 4; if (run(0) != 0) return 4;
    end_rets[1] = -1; if (run(0) != -1 || tr != "B;E;Base;E;") return 5;
    end_rets[1] = 0; end_rets[0] = -1; if (run(0) != -1 || tr != "B;E;") return 6;
    end_rets[0] = 0; base_ret = -1; if (run(1) != -1 || tr != "B;E;Base;") return 7;
    base_ret = 0; next_tag = 0x8000; if (run(0) != -1 || tr != "B;") return 8;
    next_tag = 0x8100; begin_ret = -3; if (run(0) != -1 || tr != "B;") return 9;
    return 0;
}
