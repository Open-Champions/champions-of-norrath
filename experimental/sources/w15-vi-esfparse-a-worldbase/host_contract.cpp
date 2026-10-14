#include <stdint.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_world_base.cpp"

static std::string tr; static int rm_ret, begin_ret, hdr_ret, prox_ret, tree_ret, reg_ret, end_ret, upd_ret; static unsigned short tag_in, second_in;
static uintptr_t arg_seen[4];
extern "C" int RemoveAllActors__7VIScene(void* s) { if (s != reinterpret_cast<void*>(0x1000)) tr += "BADSCENE"; tr += "Rm;"; return rm_ret; }
extern "C" int UpdateAllActors__7VIScene(void* s) { if (s != reinterpret_cast<void*>(0x1000)) tr += "BADSCENE"; tr += "Up;"; return upd_ret; }
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = second_in; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void* f) { if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; tr += "E;"; return end_ret; }
extern "C" int ParseWorldBaseHeader__10VIESFParseR7VIWorld(void*, void* w) { arg_seen[0] = reinterpret_cast<uintptr_t>(w); tr += "H;"; return hdr_ret; }
extern "C" int ParseWorldZoneProxies__10VIESFParseR7VIWorld(void*, void* w) { arg_seen[1] = reinterpret_cast<uintptr_t>(w); tr += "P;"; return prox_ret; }
extern "C" int ParseWorldTree__10VIESFParseR11VIWorldTree(void*, void* w) { arg_seen[2] = reinterpret_cast<uintptr_t>(w); tr += "T;"; return tree_ret; }
extern "C" int ParseWorldRegions__10VIESFParseR7VIWorld(void*, void* w) { arg_seen[3] = reinterpret_cast<uintptr_t>(w); tr += "R;"; return reg_ret; }

static int run() { alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse); w[6] = 0x1000; w[9] = 0x55; tr.clear();
    return ParseWorldBase__10VIESFParseR7VIWorld(parse, reinterpret_cast<void*>(0x7777)); }
int main() {
    rm_ret = begin_ret = hdr_ret = prox_ret = tree_ret = reg_ret = end_ret = upd_ret = 0; tag_in = 0x8200; second_in = 2;
    if (run() != 0 || tr != "Rm;B;H;P;T;R;E;Up;") return 1;
    if (arg_seen[0] != 0x1004 || arg_seen[1] != 0x1004 || arg_seen[2] != 0x1038 || arg_seen[3] != 0x1004) return 2;
    second_in = 1; if (run() != 0 || tr != "Rm;B;H;P;T;E;Up;") return 3;
    second_in = 0; if (run() != 0 || tr != "Rm;B;P;T;E;Up;") return 4;
    second_in = 2; upd_ret = -4; if (run() != -1) return 5;
    upd_ret = 5; end_ret = -1; if (run() != -1 || tr != "Rm;B;H;P;T;R;E;") return 6;
    end_ret = 0; reg_ret = -1; if (run() != -1 || tr != "Rm;B;H;P;T;R;") return 7;
    reg_ret = 0; tree_ret = -1; if (run() != -1 || tr != "Rm;B;H;P;T;") return 8;
    tree_ret = 0; prox_ret = -1; if (run() != -1 || tr != "Rm;B;H;P;") return 9;
    prox_ret = 0; hdr_ret = -1; if (run() != -1 || tr != "Rm;B;H;") return 10;
    hdr_ret = 0; tag_in = 0x8201; if (run() != -1 || tr != "Rm;B;") return 11;
    tag_in = 0x8200; begin_ret = -1; if (run() != -1 || tr != "Rm;B;") return 12;
    begin_ret = 0; rm_ret = -1; if (run() != -1 || tr != "Rm;") return 13;
    return 0;
}
