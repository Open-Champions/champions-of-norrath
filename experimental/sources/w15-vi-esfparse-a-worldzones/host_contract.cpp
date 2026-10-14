#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_world_zones.cpp"

// ReadBegin results are consumed from a queue of tags; every other callee result is a global.
static std::string tr; static unsigned short tags[32]; static unsigned ntags, tag_i; static int begin_ret, end_ret, nso, base_ret, res_ret, act_ret, lit_ret, rpr_ret, rsp_ret;
static unsigned long long keys[8]; static unsigned nkeys; static unsigned parse_zone_seen[8]; static unsigned nzone_seen; static uintptr_t zones_seen[8]; static unsigned nzs;
static unsigned char* parse_ptr;
alignas(16) static unsigned char zone_mem[592 * 2];
static unsigned short next_tag() { return tag_i < ntags ? tags[tag_i++] : 0xffff; }
extern "C" int ReadBegin__9VIObjFileUlRUsT2(void* f, unsigned long long key, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; keys[nkeys++] = key; *tag = next_tag(); *second = 0; tr += "U;"; return begin_ret; }
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = next_tag(); *second = 0; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int NumSubObjects__9VIObjFile(void*) { tr += "N;"; return nso; }
static void note(void* p) { parse_zone_seen[nzone_seen++] = *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(p) + 28); }
extern "C" int ParseZoneBaseObj__10VIESFParse(void* p) { note(p); tr += "Z;"; return base_ret; }
extern "C" int ParseZoneResource__10VIESFParse(void*) { tr += "R;"; return res_ret; }
extern "C" int ParseZoneRoomActorsObj__10VIESFParse(void*) { tr += "A;"; return act_ret; }
extern "C" int ParseZoneRoomStaticLightingsObj__10VIESFParse(void*) { tr += "L;"; return lit_ret; }
extern "C" int ReplaceProxyRooms__7VISceneR6VIZone(void* s, void* z) { if (s != reinterpret_cast<void*>(0x20)) tr += "BADSCENE"; zones_seen[nzs++] = reinterpret_cast<uintptr_t>(z); tr += "P;"; return rpr_ret; }
extern "C" int ReplaceStaticProxyActors__7VISceneR6VIZone(void* s, void* z) { if (s != reinterpret_cast<void*>(0x20)) tr += "BADSCENE"; zones_seen[nzs++] = reinterpret_cast<uintptr_t>(z); tr += "S;"; return rsp_ret; }
static void zone_tags(int a, int b) { // tags for one zone with a room actors and b static lightings
    unsigned short v[32]; unsigned n = 0; v[n++] = 0x3200; v[n++] = 0x3000; v[n++] = 0x3100; v[n++] = 0x3290;
    for (int i = 0; i < a; ++i) v[n++] = 0x3270;
    v[n++] = 0x32b0; for (int i = 0; i < b; ++i) v[n++] = 0x3290;
    for (unsigned i = 0; i < n; ++i) tags[ntags++] = v[i]; }
static int count_in; static unsigned last_w7;
static int run() {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse); parse_ptr = parse;
    w[6] = 0x20; w[9] = 0x55; w[7] = 0xdead; tr.clear(); nkeys = 0; nzone_seen = 0; nzs = 0; tag_i = 0;
    alignas(8) unsigned char world[96] = {0}; unsigned* ww = reinterpret_cast<unsigned*>(world + 72);
    ww[0] = static_cast<unsigned>(count_in); ww[1] = static_cast<unsigned>(reinterpret_cast<uintptr_t>(zone_mem));
    int r = ParseWorldZones__10VIESFParseR7VIWorld(parse, world);
    last_w7 = w[7];
    return r; }
int main() {
    for (unsigned i = 0; i < sizeof zone_mem; ++i) zone_mem[i] = (unsigned char)(i * 5 + 1);
    begin_ret = end_ret = base_ret = res_ret = act_ret = lit_ret = rpr_ret = rsp_ret = 0; nso = 1; count_in = 2;
    ntags = 0; zone_tags(1, 1); zone_tags(1, 1);
    if (run() != 0 || last_w7 != 0) return 1;
    std::string zone = "U;Z;U;B;N;R;N;E;B;N;B;A;N;E;B;N;B;L;N;E;P;S;E;";
    if (tr != zone + zone) return 2;
    uintptr_t z0 = reinterpret_cast<uintptr_t>(zone_mem), z1 = z0 + 592;
    if (zones_seen[0] != z0 || zones_seen[1] != z0 || zones_seen[2] != z1 || zones_seen[3] != z1 || nzs != 4) return 3;
    if (parse_zone_seen[0] != static_cast<unsigned>(z0) || parse_zone_seen[1] != static_cast<unsigned>(z1)) return 4;
    unsigned long long k16, k8; memcpy(&k16, zone_mem + 16, 8); memcpy(&k8, zone_mem + 8, 8);
    if (keys[0] != k16 || keys[1] != k8) return 5;
    memcpy(&k16, zone_mem + 592 + 16, 8); memcpy(&k8, zone_mem + 592 + 8, 8);
    if (keys[2] != k16 || keys[3] != k8) return 6;
    nso = 0; ntags = 0; zone_tags(0, 0); zone_tags(0, 0);
    if (run() != 0 || tr != "U;Z;U;B;N;E;B;N;E;B;N;E;P;S;E;U;Z;U;B;N;E;B;N;E;B;N;E;P;S;E;") return 7;
    nso = -3; if (run() != 0) return 8;
    nso = 2; ntags = 0; zone_tags(2, 2); zone_tags(2, 2); count_in = 1; if (run() != 0 || tr.find("A;N;B;A") == std::string::npos) return 9;
    count_in = 0; if (run() != 0 || !tr.empty()) return 10;
    count_in = -2; if (run() != 0 || !tr.empty()) return 11;
    count_in = 1; nso = 1; ntags = 0; zone_tags(1, 1);
    rsp_ret = -1; if (run() != -1 || last_w7 != static_cast<unsigned>(z0) || tr.find("S;E;") != std::string::npos) return 12;
    rsp_ret = 0; rpr_ret = -1; if (run() != -1 || tr.find("S;") != std::string::npos) return 13;
    rpr_ret = 0; end_ret = -1; if (run() != -1 || tr != "U;Z;U;B;N;R;N;E;") return 14;
    end_ret = 0; lit_ret = -1; if (run() != -1 || tr.find("L;N") != std::string::npos) return 15;
    lit_ret = 0; act_ret = -1; if (run() != -1 || tr.find("A;N") != std::string::npos) return 16;
    act_ret = 0; res_ret = -1; if (run() != -1 || tr.find("R;N") != std::string::npos) return 17;
    res_ret = 0; base_ret = -1; if (run() != -1 || tr != "U;Z;") return 18;
    base_ret = 0; begin_ret = -1; if (run() != -1 || tr != "U;") return 19;
    begin_ret = 0; ntags = 0; zone_tags(1, 1); tags[0] = 0x3201; if (run() != -1 || tr != "U;") return 20;
    ntags = 0; zone_tags(1, 1); tags[1] = 0x3001; if (run() != -1 || tr != "U;Z;U;") return 21;
    ntags = 0; zone_tags(1, 1); tags[2] = 0x3101; if (run() != -1 || tr != "U;Z;U;B;") return 22;
    ntags = 0; zone_tags(1, 1); tags[3] = 0x3291; if (run() != -1 || tr.find("E;B;") == std::string::npos || tr.find("A;") != std::string::npos) return 23;
    ntags = 0; zone_tags(1, 1); tags[4] = 0x3271; if (run() != -1 || tr.find("A;") != std::string::npos) return 24;
    ntags = 0; zone_tags(1, 1); tags[5] = 0x32b1; if (run() != -1 || tr.find("L;") != std::string::npos) return 25;
    ntags = 0; zone_tags(1, 1); tags[6] = 0x3291 + 1; if (run() != -1 || tr.find("L;") != std::string::npos) return 26;
    return 0;
}
