// Host check only; not a build input.
#include <string>
#include <string.h>
#include "parse_world_base_header.cpp"
static std::string tr; static int begin_ret, end_ret, init_ret; static unsigned short tag_in, second_in;
static int intReads, floatReads;
static int gotA, gotB, gotC, gotD; static float gotE; static float gotVec[3], gotBox[6]; static void* gotWorld;
extern "C" int ReadBegin__9VIObjFileRUsT1(void*, unsigned short* tag, unsigned short* second) { *tag = tag_in; *second = second_in; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int Read__9VIObjFileRi(void*, int* value) { *value = 10 + intReads++; tr += "i;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* value) { *value = 0.5f + floatReads++; tr += "f;"; return 0; }
extern "C" int Init__7VIWorldiiiifRC7VIVect3RC6VIBBox(void* world, int a, int b, int c, int d, float e, const float* vec, const float* box) {
    gotWorld = world; gotA = a; gotB = b; gotC = c; gotD = d; gotE = e; memcpy(gotVec, vec, 12); memcpy(gotBox, box, 24); tr += "I;"; return init_ret;
}
static int run(unsigned char* parse, void* world) { tr.clear(); intReads = floatReads = 0; return ParseWorldBaseHeader__10VIESFParseR7VIWorld(parse, world); }
int main() {
    alignas(8) unsigned char parse[64] = {0};
    reinterpret_cast<unsigned*>(parse)[9] = 0x55;
    char world[8];
    begin_ret = 0; end_ret = 0; init_ret = 0; tag_in = 0x8220; second_in = 1;
    // new format: A, six box floats, B, C, D, then E and three vector floats
    if (run(parse, world) != 0 || tr != "B;i;f;f;f;f;f;f;i;i;i;f;f;f;f;I;E;") return 1;
    if (gotWorld != world || gotA != 10 || gotB != 11 || gotC != 12 || gotD != 13) return 2;
    if (gotBox[0] != 0.5f || gotBox[5] != 5.5f || gotE != 6.5f || gotVec[0] != 7.5f || gotVec[2] != 9.5f) return 3;
    second_in = 0;
    if (run(parse, world) != 0 || tr != "B;f;f;f;f;f;f;i;i;f;f;f;f;I;E;") return 4;
    if (gotA != 1 || gotD != 4 || gotB != 10 || gotC != 11) return 5;
    init_ret = -1;
    if (run(parse, world) != -1 || tr.back() != 'I' - 'I' + ';' || tr.find("E;") != std::string::npos) return 6;
    init_ret = 0; end_ret = -1;
    if (run(parse, world) != -1) return 7;
    end_ret = 0; tag_in = 0x8221;
    if (run(parse, world) != -1 || tr != "B;") return 8;
    tag_in = 0x8220; begin_ret = -1;
    if (run(parse, world) != -1 || tr != "B;") return 9;
    return 0;
}
