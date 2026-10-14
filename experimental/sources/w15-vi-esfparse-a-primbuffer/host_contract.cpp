#include <stdint.h>
#include <string.h>
#include <string>
#include <math.h>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#define powf host_powf
#include "parse_prim_buffer.cpp"
#undef powf

static std::string tr; static int begin_ret, end_ret, find_ret, find_type, find_handle, create_ret, add_ret, init_ret, lock_ret, unlock_ret, calc_ret, v0_ret; static unsigned short tag_in, ver_in;
static unsigned id_in; static int ints[32]; static unsigned ni; static short shorts[128]; static unsigned ns; static signed char sbytes[64]; static unsigned nsb; static unsigned char ubytes[64]; static unsigned nub; static float floats[64]; static unsigned nf; static unsigned buffer_type;
static int init_args[5]; static int packing_seen[3]; static int calc_count; static void* calc_table;
alignas(16) static unsigned char buffer_mem[64]; alignas(16) static unsigned char object_mem[128]; alignas(16) static float table_mem[16];
static float vf[8][3], uvf[8][2], nrf[8][3]; static unsigned nvf, nuf, nnf; static short vs[8][3], uvs[8][2]; static unsigned nvs, nus; static signed char nrs[8][3]; static unsigned nns; static unsigned char cols[8][4]; static unsigned ncol; static short grp[8]; static unsigned ng;
extern "C" float host_powf(float a, float b) { return (float)pow(a, b); }
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = ver_in; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "u;"; return 0; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = ints[ni++]; tr += "i;"; return 0; }
extern "C" int Read__9VIObjFileRs(void*, short* v) { *v = shorts[ns++]; tr += "s;"; return 0; }
extern "C" int Read__9VIObjFileRSc(void*, signed char* v) { *v = sbytes[nsb++]; tr += "n;"; return 0; }
extern "C" int Read__9VIObjFileRUc(void*, unsigned char* v) { *v = ubytes[nub++]; tr += "c;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = floats[nf++]; tr += "f;"; return 0; }
extern "C" int ParsePrimBufferObjV0__10VIESFParse(void*) { tr += "V0;"; return v0_ret; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void*, unsigned, int* t, int* h) { *t = find_type; *h = find_handle; tr += "Fd;"; return find_ret; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void*, unsigned, int, int) { tr += "Ad;"; return add_ret; }
extern "C" int CreatePrimBuffer__8VIRasteri(void*, int) { tr += "Cr;"; return create_ret; }
extern "C" void* PrimBuffer__8VIRasteri(void*, int) { *reinterpret_cast<unsigned*>(buffer_mem + 8) = buffer_type; tr += "Pb;"; return buffer_mem; }
extern "C" int Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii(void*, void*, int k, int a, int g, int c) { init_args[0] = k; init_args[1] = a; init_args[2] = g; init_args[3] = c; tr += "In;"; return init_ret; }
extern "C" int Lock__12VIPrimBuffer(void*) { tr += "L;"; return lock_ret; }
extern "C" int Unlock__12VIPrimBuffer(void*) { tr += "U;"; return unlock_ret; }
extern "C" int SetPacking__12VIPrimBufferiii(void*, int x, int y, int z) { packing_seen[0] = x; packing_seen[1] = y; packing_seen[2] = z; tr += "Pk;"; return -9; }
extern "C" int BeginMaterial__12VIPrimBufferi(void*, int m) { tr += "BM" + std::to_string(m) + ";"; return 0; }
extern "C" int EndMaterial__12VIPrimBuffer(void*) { tr += "EM;"; return 0; }
extern "C" int Begin__12VIPrimBufferi(void*, int n) { tr += "Bg" + std::to_string(n) + ";"; return 0; }
extern "C" int End__12VIPrimBuffer(void*) { tr += "En;"; return 0; }
extern "C" int UV0__12VIPrimBufferR7VIVect2(void*, const float* v) { memcpy(uvf[nuf++], v, 8); tr += "Uf;"; return 0; }
extern "C" int UV0__12VIPrimBufferR10VIVect2S16(void*, const short* v) { memcpy(uvs[nus++], v, 4); tr += "Us;"; return 0; }
extern "C" int Normal__12VIPrimBufferR7VIVect3(void*, const float* v) { memcpy(nrf[nnf++], v, 12); tr += "Nf;"; return 0; }
extern "C" int Normal__12VIPrimBufferR9VIVect3S8(void*, const signed char* v) { memcpy(nrs[nns++], v, 3); tr += "Ns;"; return 0; }
extern "C" int Color__12VIPrimBufferR9VIColor32(void*, const unsigned char* v) { memcpy(cols[ncol++], v, 4); tr += "Co;"; return 0; }
extern "C" int VertexGroup__12VIPrimBuffers(void*, short g) { grp[ng++] = g; tr += "Vg;"; return 0; }
extern "C" int Vertex__12VIPrimBufferR7VIVect3(void*, const float* v) { memcpy(vf[nvf++], v, 12); tr += "Vf;"; return 0; }
extern "C" int Vertex__12VIPrimBufferR10VIVect3S16(void*, const short* v) { memcpy(vs[nvs++], v, 6); tr += "Vs;"; return 0; }
extern "C" int CalcBVolumes__12VIPrimBufferiP7VIVect3(void*, int n, void* t) { calc_count = n; calc_table = t; tr += "Cv;"; return calc_ret; }
static unsigned object_word;
static int run() {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[3] = 0x90; w[7] = object_word; w[8] = 0x30; w[9] = 0x55;
    tr.clear(); ni = ns = nsb = nub = nf = nvf = nuf = nnf = nvs = nus = nns = ncol = ng = 0;
    return ParsePrimBuffer__10VIESFParse(parse); }
int main() {
    begin_ret = end_ret = add_ret = init_ret = lock_ret = unlock_ret = calc_ret = 0; create_ret = 7; find_ret = 0; find_type = 11; find_handle = 55; tag_in = 0x1200; ver_in = 2; id_in = 0; v0_ret = 5;
    memset(object_mem, 0, sizeof object_mem);
    *reinterpret_cast<int*>(object_mem + 84) = 5; *reinterpret_cast<unsigned*>(object_mem + 88) = static_cast<unsigned>(reinterpret_cast<uintptr_t>(table_mem));
    object_word = static_cast<unsigned>(reinterpret_cast<uintptr_t>(object_mem));
    for (int i = 0; i < 64; ++i) { floats[i] = 1.5f + i; shorts[i] = (short)(16 * (i + 1)); sbytes[i] = (signed char)(i % 2 ? -(i + 1) : i + 1); ubytes[i] = (unsigned char)(0x40 + i); }
    // kind 0 (floats), version 2, one group of two vertices; kind, a, groups, c, p1, p2, p3 then count and material
    ints[0] = 0; ints[1] = 1; ints[2] = 1; ints[3] = 9; ints[4] = 3; ints[5] = 1; ints[6] = 4; ints[7] = 2; ints[8] = 6;
    buffer_type = 99;
    if (run() != 7) return 1;
    if (tr != "B;u;i;i;i;i;i;i;i;Cr;Pb;In;L;Pk;i;i;BM6;Bg2;f;f;f;f;f;f;f;f;c;c;c;c;Uf;Nf;Co;Vf;f;f;f;f;f;f;f;f;c;c;c;c;Uf;Nf;Co;Vf;En;EM;Cv;U;E;") return 2;
    if (init_args[0] != 0 || init_args[1] != 1 || init_args[2] != 1 || init_args[3] != 9 || packing_seen[0] != 3 || packing_seen[1] != 1 || packing_seen[2] != 4) return 3;
    if (vf[0][0] != 1.5f || uvf[0][0] != 4.5f || nrf[0][2] != 8.5f || cols[0][0] != 0x40 || cols[1][3] != 0x47 || vf[1][0] != 9.5f) return 4;
    if (calc_count != 5 || calc_table != table_mem) return 5;
    // version 1: no id is read; version 0 calls the V0 parser
    ver_in = 1; id_in = 66; find_ret = 1; if (run() != 7 || tr.find("u;") != std::string::npos || tr.find("Fd;") != std::string::npos) return 6;
    ver_in = 0; if (run() != 5 || tr != "B;V0;") return 7;
    v0_ret = -3; if (run() != -1) return 8;
    v0_ret = 5; ver_in = 2; id_in = 0;
    // kind 2, buffer kind 2: shorts with signed normals
    ints[0] = 2; buffer_type = 2;
    if (run() != 7 || nvs != 2 || nus != 2 || nns != 2 || nvf != 0 || vs[0][1] != 32 || uvs[0][0] != 64 || nrs[0][0] != 1 || nrs[0][1] != -2 || cols[0][0] != 0x40 || ng != 0) return 9;
    // kind 2, buffer kind 0: converted floats; p1 = 3 -> 1/8, p2 = 1 -> 1/2, normals * (1/127.5)
    buffer_type = 0;
    if (run() != 7 || nvf != 2 || nvs != 0 || vf[0][0] != 16 * 0.125f || vf[0][2] != 48 * 0.125f || uvf[0][0] != 64 * 0.5f || uvf[0][1] != 80 * 0.5f) return 10;
    if (nrf[0][0] != 1 * 0x1.020408p-7f || nrf[0][1] != -2 * 0x1.020408p-7f || nrf[0][2] != 3 * 0x1.020408p-7f || cols[1][3] != 0x47) return 11;
    buffer_type = 4; if (run() != -1 || tr.find("Cv;") != std::string::npos) return 12;
    // kind 4, buffer kind 4: shorts with a group; buffer kind 6: floats with a group
    ints[0] = 4; buffer_type = 4;
    if (run() != 7 || nvs != 2 || ng != 2 || grp[0] != shorts[5] || grp[1] != shorts[11]) return 13;
    buffer_type = 6; if (run() != 7 || nvf != 2 || ng != 2 || grp[0] != shorts[5] || vf[0][0] != 16 * 0.125f) return 14;
    buffer_type = 2; if (run() != -1) return 15;
    ints[0] = 3; if (run() != -1) return 16;
    ints[0] = 0;
    // a == 0 and equal materials in two groups
    ints[1] = 0; ints[2] = 2; ints[3] = 9; ints[7] = 1; ints[8] = 6; ints[9] = 1; ints[10] = 6;
    if (run() != 7 || tr.find("EM;") != std::string::npos || tr.find("En;i;i;Bg1;") == std::string::npos) return 17;
    ints[10] = 8; if (run() != 7 || tr.find("En;i;i;EM;BM8;Bg1;") == std::string::npos) return 18;
    ints[1] = 1; ints[2] = 1; ints[7] = 2; ints[8] = 6;
    // zero and negative group counts
    ints[2] = 0; if (run() != 7 || tr.find("i;i;BM") != std::string::npos) return 19;
    ints[2] = -1; if (run() != 7) return 20;
    ints[2] = 1;
    // object word zero or count zero
    object_word = 0; if (run() != 7 || calc_count != 0 || calc_table != 0) return 21;
    object_word = static_cast<unsigned>(reinterpret_cast<uintptr_t>(object_mem)); *reinterpret_cast<int*>(object_mem + 84) = 0; if (run() != 7 || calc_count != 0) return 22;
    *reinterpret_cast<int*>(object_mem + 84) = 5;
    // cached resource and registration
    id_in = 66; find_ret = 1; if (run() != 55 || tr != "B;u;i;i;i;i;i;i;i;Fd;E;") return 23;
    find_type = 3; if (run() != -1) return 24;
    find_ret = 0; find_type = 11; if (run() != 7) return 25;
    add_ret = -1; if (run() != -1 || tr.find("In;") != std::string::npos) return 26;
    add_ret = 0; id_in = 0;
    end_ret = -1; if (run() != -1) return 27;
    end_ret = 0; unlock_ret = -1; if (run() != -1 || tr.find("E;") != std::string::npos) return 28;
    unlock_ret = 0; calc_ret = -1; if (run() != -1 || tr.find("U;") != std::string::npos) return 29;
    calc_ret = 0; lock_ret = -1; if (run() != -1 || tr.find("Pk;") != std::string::npos) return 30;
    lock_ret = 0; init_ret = -1; if (run() != -1 || tr.find("L;") != std::string::npos) return 31;
    init_ret = 0; create_ret = -2; if (run() != -1 || tr.find("Pb;") != std::string::npos) return 32;
    create_ret = 7; tag_in = 0x1201; if (run() != -1 || tr != "B;") return 33;
    tag_in = 0x1200; begin_ret = -1; if (run() != -1 || tr != "B;") return 34;
    return 0;
}
