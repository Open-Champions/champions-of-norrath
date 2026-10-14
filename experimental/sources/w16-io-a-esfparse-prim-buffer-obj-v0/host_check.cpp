// Host check only; not a build input.
#include <string>
static unsigned char arena[0x100];
#define NORRATH_WORD_ADDRESS(word) (static_cast<void*>(arena + (word)))
#include "parse_prim_buffer_obj_v0.cpp"
static std::string tr; static int createId, initRet, lockRet, unlockRet, endRet; static int intReads; static int ints[16];
static int floatReads;
static void* bufferObj = arena + 0x80;
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return endRet; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = ints[intReads++]; tr += "i;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = 1.0f + floatReads++; tr += "f;"; return 0; }
extern "C" int Read__9VIObjFileRUc(void*, unsigned char* v) { *v = static_cast<unsigned char>(0x10 + floatReads++); tr += "c;"; return 0; }
extern "C" int CreatePrimBuffer__8VIRasteri(void* r, int t) { tr += (r == arena + 4 && t == 5) ? "C;" : "C?;"; return createId; }
extern "C" void* PrimBuffer__8VIRasteri(void*, int id) { tr += "P" + std::to_string(id) + ";"; return bufferObj; }
extern "C" int Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii(void* b, void* r, int t, int a, int bb, int c) {
    tr += (b == bufferObj && r == arena + 4 && t == 0) ? "I" : "I?"; tr += std::to_string(a) + std::to_string(bb) + std::to_string(c) + ";"; return initRet; }
extern "C" int Lock__12VIPrimBuffer(void*) { tr += "L;"; return lockRet; }
extern "C" int Unlock__12VIPrimBuffer(void*) { tr += "U;"; return unlockRet; }
extern "C" void BeginMaterial__12VIPrimBufferi(void*, int m) { tr += "bm" + std::to_string(m) + ";"; }
extern "C" void EndMaterial__12VIPrimBuffer(void*) { tr += "em;"; }
extern "C" void Begin__12VIPrimBufferi(void*, int n) { tr += "b" + std::to_string(n) + ";"; }
extern "C" void End__12VIPrimBuffer(void*) { tr += "e;"; }
extern "C" void UV0__12VIPrimBufferR7VIVect2(void*, float* uv) { tr += "uv" + std::to_string(static_cast<int>(uv[0])) + std::to_string(static_cast<int>(uv[1])) + ";"; }
extern "C" void Normal__12VIPrimBufferR7VIVect3(void*, float* n) { tr += "n" + std::to_string(static_cast<int>(n[0])) + ";"; }
extern "C" void Color__12VIPrimBufferR9VIColor32(void*, unsigned char* c) { tr += "co" + std::to_string(c[0]) + "," + std::to_string(c[3]) + ";"; }
extern "C" void Vertex__12VIPrimBufferR7VIVect3(void*, float* v) { tr += "v" + std::to_string(static_cast<int>(v[0])) + std::to_string(static_cast<int>(v[2])) + ";"; }
static int run(unsigned char* parse) { tr.clear(); intReads = floatReads = 0; return ParsePrimBufferObjV0__10VIESFParse(parse); }
int main() {
    alignas(8) unsigned char parse[64] = {0};
    unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 5; w[3] = 4; w[9] = 0x55;
    createId = 7; initRet = 0; lockRet = 0; unlockRet = 0; endRet = 0;
    // A=1, B=2 groups, C=9; group 1: 1 vertex, material 3; group 2: 1 vertex, material 4
    int seq[] = {1, 2, 9, 1, 3, 1, 4};
    for (int i = 0; i < 7; ++i) ints[i] = seq[i];
    std::string head = "i;i;i;C;P7;I129;L;";
    std::string vert1 = "f;f;f;f;f;f;f;f;c;c;c;c;uv45;n6;co24,27;v13;";
    std::string vert2 = "f;f;f;f;f;f;f;f;c;c;c;c;uv";
    if (run(parse) != 7) return 1;
    // group 1: i;i;bm3;b1;<vertex>;e;   group 2: i;i;em;bm4;b1;<vertex>;e;  then em; U; E
    if (tr.compare(0, head.size(), head) != 0) return 2;
    if (tr.find("i;i;bm3;b1;" + vert1 + "e;i;i;em;bm4;b1;" + vert2) == std::string::npos) return 3;
    if (tr.substr(tr.size() - 9) != "e;em;U;E;") { return 4; }
    // a material that stays the same is not re-begun; A = 0 skips the final EndMaterial
    int seq2[] = {0, 2, 9, 1, 3, 1, 3};
    for (int i = 0; i < 7; ++i) ints[i] = seq2[i];
    run(parse);
    if (tr.find("bm") != tr.rfind("bm") || tr.find("em;") != std::string::npos) return 5;
    // failures
    for (int i = 0; i < 7; ++i) ints[i] = seq[i];
    createId = -2; if (run(parse) != -1 || tr != "i;i;i;C;") return 6;
    createId = 7; initRet = -1; if (run(parse) != -1 || tr != "i;i;i;C;P7;I129;") return 7;
    initRet = 0; lockRet = -1; if (run(parse) != -1 || tr != head) return 8;
    lockRet = 0; unlockRet = -1; if (run(parse) != -1 || tr.back() != ';' || tr.substr(tr.size() - 5) != "em;U;") return 9;
    unlockRet = 0; endRet = -1; if (run(parse) != -1) return 10;
    return 0;
}
