#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_ref_map.cpp"
static std::string tr; static unsigned id_in; static int count_in, find_ret, find_type, find_handle, create_ret, add_ret, init_ret, ins_fail, ins_calls, end_rets[2], end_calls, begin_ret, rd;
static unsigned short tag_in; alignas(16) static unsigned char map_mem[64]; static int keys[8], vals[8];
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = 0; tr += "B;"; return begin_ret; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { if (rd == 0) *v = count_in; else *v = 100 + rd; ++rd; tr += "I;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_rets[end_calls++ ? 1 : 0]; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int CreateRefMap__7VIScenei(void* s, int a) { if (s != reinterpret_cast<void*>(0x20) || a != 0x99) tr += "BADCREATE"; tr += "C;"; return create_ret; }
extern "C" void* RefMap__7VIScenei(void*, int) { tr += "R;"; return map_mem; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) {
    if (r != map_mem || id != id_in || t != 8 || h != create_ret) tr += "BADADD"; tr += "A;"; return add_ret; }
extern "C" int Init__t5VIMap2ZiZii(void* m, int c) { if (m != map_mem + 8 || c != count_in) tr += "BADINIT"; tr += "In;"; return init_ret; }
extern "C" int Insert__t5VIMap2ZiZiRCiT1(void* m, const int* k, const int* v) {
    if (m != map_mem + 8) tr += "BADMAP"; keys[ins_calls] = *k; vals[ins_calls] = *v; tr += "Ins;"; return ins_calls++ == ins_fail ? -1 : 0; }
static int run() {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[6] = 0x20; w[8] = 0x30; w[9] = 0x55; tr.clear(); end_calls = 0; rd = 0; ins_calls = 0;
    return ParseRefMap__10VIESFParse(parse);
}
int main() {
    id_in = 7; count_in = 2; find_ret = 0; find_type = 8; find_handle = 11; create_ret = 6; add_ret = 0; init_ret = 0; ins_fail = -1; begin_ret = 0; tag_in = 0x5000;
    end_rets[0] = end_rets[1] = 0;
    if (run() != 6 || tr != "B;U;I;Find;C;R;A;In;I;I;Ins;I;I;Ins;E;") return 1;
    if (keys[0] != 101 || vals[0] != 102 || keys[1] != 103 || vals[1] != 104) return 2;
    find_ret = 1; if (run() != 11 || tr != "B;U;I;Find;E;") return 3;
    find_type = 3; if (run() != -1 || tr != "B;U;I;Find;") return 4;
    find_type = 8; find_ret = 0; end_rets[0] = -1; if (run() != -1) return 5;
    end_rets[0] = 0; ins_fail = 1; if (run() != -1 || ins_calls != 2 || tr.find("E;") != std::string::npos) return 6;
    ins_fail = -1; init_ret = -1; if (run() != -1 || tr.find("Ins") != std::string::npos) return 7;
    init_ret = 0; add_ret = -1; if (run() != -1 || tr.find("In;") != std::string::npos) return 8;
    add_ret = 0; create_ret = -3; if (run() != -1 || tr.find("R;") != std::string::npos) return 9;
    create_ret = 6; count_in = 0; if (run() != 6 || tr != "B;U;I;Find;C;R;A;In;E;") return 10;
    count_in = -2; if (run() != 6 || tr.find("Ins") != std::string::npos) return 11;
    count_in = 2; tag_in = 0x5001; if (run() != -1 || tr != "B;") return 12;
    tag_in = 0x5000; begin_ret = -1; if (run() != -1 || tr != "B;") return 13;
    begin_ret = 0; id_in = 0; if (run() != 6 || tr.find("Find") != std::string::npos || tr.find("A;") != std::string::npos) return 14;
    return 0;
}
