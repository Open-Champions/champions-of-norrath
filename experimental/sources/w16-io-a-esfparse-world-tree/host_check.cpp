// Host check only; not a build input.
#include <string>
static unsigned char arena[0x1000];
#define NORRATH_WORD_ADDRESS(word) (static_cast<void*>(arena + (word)))
#include "parse_world_tree.cpp"
static std::string tr; static int begin_ret, end_ret, init_ret, nodeCountIn, entryCountIn, intReads; static unsigned short tag_in;
static int initA, initB, floatReads, uintReads;
extern "C" int ReadBegin__9VIObjFileRUsT1(void*, unsigned short* tag, unsigned short* second) { *tag = tag_in; *second = 0; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int Read__9VIObjFileRi(void*, int* value) {
    if (intReads == 0) *value = nodeCountIn; else if (intReads == 1) *value = entryCountIn; else tr += (value == reinterpret_cast<int*>(arena + 0x300) + (intReads - 2)) ? "i;" : "i?;";
    if (intReads >= 2) *value = 100 + intReads;
    ++intReads;
    return 0;
}
extern "C" int Read__9VIObjFileRf(void*, float* value) {
    unsigned char* expect = arena + 0x200 + 24 * (floatReads / 4) + 4 * (floatReads % 4);
    tr += reinterpret_cast<unsigned char*>(value) == expect ? "f;" : "f?;";
    ++floatReads; return 0;
}
extern "C" int Read__9VIObjFileRUi(void*, unsigned* value) {
    unsigned char* expect = arena + 0x200 + 24 * (uintReads / 2) + 16 + 4 * (uintReads % 2);
    tr += reinterpret_cast<unsigned char*>(value) == expect ? "u;" : "u?;";
    ++uintReads; return 0;
}
extern "C" int Init__11VIWorldTreeii(void*, int a, int b) { initA = a; initB = b; tr += "I;"; return init_ret; }
static int run(unsigned char* parse, unsigned char* tree) { tr.clear(); intReads = floatReads = uintReads = 0; return ParseWorldTree__10VIESFParseR11VIWorldTree(parse, tree); }
int main() {
    alignas(8) unsigned char parse[64] = {0};
    reinterpret_cast<unsigned*>(parse)[9] = 0x55;
    alignas(8) unsigned char tree[32] = {0};
    *reinterpret_cast<unsigned*>(tree + 4) = 0x200;
    *reinterpret_cast<unsigned*>(tree + 12) = 0x300;
    begin_ret = 0; end_ret = 0; init_ret = 0; tag_in = 0x8230; nodeCountIn = 2; entryCountIn = 3;
    if (run(parse, tree) != 0 || initA != 2 || initB != 3) return 1;
    if (tr != "B;I;f;f;f;f;u;u;f;f;f;f;u;u;i;i;i;E;") return 2;
    if (*reinterpret_cast<int*>(arena + 0x300 + 8) != 104) return 3;
    nodeCountIn = 0; entryCountIn = 0;
    if (run(parse, tree) != 0 || tr != "B;I;E;") return 4;
    nodeCountIn = -2; entryCountIn = -1;
    if (run(parse, tree) != 0 || tr != "B;I;E;") return 5;
    nodeCountIn = 1; entryCountIn = 0; end_ret = -1;
    if (run(parse, tree) != -1 || tr != "B;I;f;f;f;f;u;u;E;") return 6;
    end_ret = 0; init_ret = -1;
    if (run(parse, tree) != -1 || tr != "B;I;") return 7;
    init_ret = 0; tag_in = 0x8231;
    if (run(parse, tree) != -1 || tr != "B;") return 8;
    tag_in = 0x8230; begin_ret = -1;
    if (run(parse, tree) != -1 || tr != "B;") return 9;
    return 0;
}
