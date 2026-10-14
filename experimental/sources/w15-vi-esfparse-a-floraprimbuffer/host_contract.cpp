#include <stdint.h>
#include <string.h>
#include <string>
#include <math.h>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#define powf host_powf
#include "parse_flora_prim_buffer.cpp"
#undef powf

static std::string tr; static int begin_ret, end_ret, find_ret, find_type, find_handle, create_ret, add_ret, init_ret, lock_ret, unlock_ret, calc_ret; static unsigned short tag_in;
static unsigned id_in; static int ints[32]; static unsigned ni; static short shorts[64]; static unsigned ns; static float floats[32]; static unsigned nf; static unsigned buffer_type;
static int init_args[5]; static float pow_args[2][2]; static unsigned npow; static int packing_seen[3]; static int create_arg, add_id, add_type, add_handle;
alignas(16) static unsigned char buffer_mem[64];
static float vf[8][3], uvf[8][2]; static unsigned nvf, nuf; static short vs[8][3], uvs[8][2]; static unsigned nvs, nus;
extern "C" float host_powf(float a, float b) { pow_args[npow][0] = a; pow_args[npow][1] = b; ++npow; return a == 2.0f && b == 3.0f ? 8.0f : (float)pow(a, b); }
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = 0; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "u;"; return 0; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = ints[ni++]; tr += "i;"; return 0; }
extern "C" int Read__9VIObjFileRs(void*, short* v) { *v = shorts[ns++]; tr += "s;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = floats[nf++]; tr += "f;"; return 0; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) { if (d != reinterpret_cast<void*>(0x30) || id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Fd;"; return find_ret; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* d, void* r, unsigned id, int t, int h) { if (d != reinterpret_cast<void*>(0x30) || r != buffer_mem) tr += "BADADD"; add_id = id; add_type = t; add_handle = h; tr += "Ad;"; return add_ret; }
extern "C" int CreatePrimBuffer__8VIRasteri(void* r, int a) { if (r != reinterpret_cast<void*>(0x90)) tr += "BADRAST"; create_arg = a; tr += "Cr;"; return create_ret; }
extern "C" void* PrimBuffer__8VIRasteri(void* r, int h) { if (r != reinterpret_cast<void*>(0x90) || h != create_ret) tr += "BADRAST"; *reinterpret_cast<unsigned*>(buffer_mem + 8) = buffer_type; tr += "Pb;"; return buffer_mem; }
extern "C" int Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii(void* b, void* r, int k, int a, int g, int c) { if (b != buffer_mem || r != reinterpret_cast<void*>(0x90)) tr += "BADINIT"; init_args[0] = k; init_args[1] = a; init_args[2] = g; init_args[3] = c; tr += "In;"; return init_ret; }
extern "C" int Lock__12VIPrimBuffer(void*) { tr += "L;"; return lock_ret; }
extern "C" int Unlock__12VIPrimBuffer(void*) { tr += "U;"; return unlock_ret; }
extern "C" int SetPacking__12VIPrimBufferiii(void*, int x, int y, int z) { packing_seen[0] = x; packing_seen[1] = y; packing_seen[2] = z; tr += "Pk;"; return -9; }
extern "C" int BeginMaterial__12VIPrimBufferi(void*, int m) { tr += "BM" + std::to_string(m) + ";"; return 0; }
extern "C" int EndMaterial__12VIPrimBuffer(void*) { tr += "EM;"; return 0; }
extern "C" int Begin__12VIPrimBufferi(void*, int n) { tr += "Bg" + std::to_string(n) + ";"; return 0; }
extern "C" int End__12VIPrimBuffer(void*) { tr += "En;"; return 0; }
extern "C" int UV0__12VIPrimBufferR7VIVect2(void*, const float* v) { memcpy(uvf[nuf++], v, 8); tr += "Uf;"; return 0; }
extern "C" int UV0__12VIPrimBufferR10VIVect2S16(void*, const short* v) { memcpy(uvs[nus++], v, 4); tr += "Us;"; return 0; }
extern "C" int Vertex__12VIPrimBufferR7VIVect3(void*, const float* v) { memcpy(vf[nvf++], v, 12); tr += "Vf;"; return 0; }
extern "C" int Vertex__12VIPrimBufferR10VIVect3S16(void*, const short* v) { memcpy(vs[nvs++], v, 6); tr += "Vs;"; return 0; }
extern "C" int CalcBVolumes__12VIPrimBufferiP7VIVect3(void*, int n, void* t) { if (n != 0 || t != 0) tr += "BADCALC"; tr += "Cv;"; return calc_ret; }
static int run() {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[3] = 0x90; w[8] = 0x30; w[9] = 0x55;
    tr.clear(); ni = ns = nf = nvf = nuf = nvs = nus = npow = 0;
    return ParseFloraPrimBuffer__10VIESFParse(parse); }
int main() {
    begin_ret = end_ret = add_ret = init_ret = lock_ret = unlock_ret = calc_ret = 0; create_ret = 7; find_ret = 0; find_type = 11; find_handle = 55; tag_in = 0x1230; id_in = 0;
    // kind 7: kind, a, groups, c, p1, p2 then per group count and material
    ints[0] = 7; ints[1] = 1; ints[2] = 2; ints[3] = 9; ints[4] = 3; ints[5] = 1; ints[6] = 2; ints[7] = 4; ints[8] = 2; ints[9] = 5;
    for (int i = 0; i < 20; ++i) floats[i] = 1.5f + i;
    buffer_type = 99;
    if (run() != 7) return 1;
    if (tr != "B;u;i;i;i;i;i;i;Cr;Pb;In;L;Pk;i;i;BM4;Bg2;f;f;f;f;f;Uf;Vf;f;f;f;f;f;Uf;Vf;En;i;i;EM;BM5;Bg2;f;f;f;f;f;Uf;Vf;f;f;f;f;f;Uf;Vf;En;EM;Cv;U;E;") return 2;
    if (init_args[0] != 7 || init_args[1] != 1 || init_args[2] != 2 || init_args[3] != 9 || create_arg != 0x99) return 3;
    if (packing_seen[0] != 3 || packing_seen[1] != 1 || packing_seen[2] != 7) return 4;
    if (vf[0][0] != 1.5f || vf[0][2] != 3.5f || uvf[0][0] != 4.5f || uvf[0][1] != 5.5f || vf[3][0] != 16.5f || nvf != 4) return 5;
    if (pow_args[0][0] != 2.0f || pow_args[0][1] != 3.0f || pow_args[1][1] != 1.0f) return 6;
    // a == 0: no trailing EndMaterial; same material in both groups: no End/Begin between
    ints[1] = 0; ints[7] = 4; ints[9] = 4; if (run() != 7 || tr.find("EM;") != std::string::npos || tr.find("En;i;i;Bg2;") == std::string::npos) return 7;
    ints[1] = 1; ints[9] = 5;
    // kind 8, buffer kind 7: converted floats; p1 = 3 scale 1/8, p2 = 1 scale 1/2
    ints[0] = 8; buffer_type = 7; for (int i = 0; i < 20; ++i) shorts[i] = (short)(16 * (i + 1));
    if (run() != 7 || nvf != 4 || nvs != 0 || vf[0][0] != 16 * 0.125f || vf[0][1] != 32 * 0.125f || vf[0][2] != 48 * 0.125f || uvf[0][0] != 64 * 0.5f || uvf[0][1] != 80 * 0.5f) return 8;
    // kind 8, buffer kind 8: raw shorts
    buffer_type = 8; if (run() != 7 || nvs != 4 || nvf != 0 || vs[0][1] != 32 || uvs[0][1] != 80 || vs[1][0] != 96) return 9;
    // kind 8, other buffer kind; other kind
    buffer_type = 3; if (run() != -1 || tr.find("Cv;") != std::string::npos) return 10;
    ints[0] = 5; if (run() != -1) return 11;
    ints[0] = 7; buffer_type = 99;
    // zero groups
    ints[2] = 0; if (run() != 7 || tr != "B;u;i;i;i;i;i;i;Cr;Pb;In;L;Pk;EM;Cv;U;E;") return 12;
    ints[2] = -1; if (run() != 7) return 13;
    ints[2] = 2;
    // cached resource
    id_in = 66; find_ret = 1; if (run() != 55 || tr != "B;u;i;i;i;i;i;i;Fd;E;") return 14;
    end_ret = -1; if (run() != 55) return 15;
    end_ret = 0; find_type = 3; if (run() != -1 || tr.find("E;") != std::string::npos) return 16;
    find_ret = 0; find_type = 11;
    // id registered
    if (run() != 7 || add_id != 66 || add_type != 11 || add_handle != 7) return 17;
    add_ret = -1; if (run() != -1 || tr.find("In;") != std::string::npos) return 18;
    add_ret = 0; id_in = 0;
    end_ret = -1; if (run() != -1) return 19;
    end_ret = 0; unlock_ret = -1; if (run() != -1 || tr.find("E;") != std::string::npos) return 20;
    unlock_ret = 0; calc_ret = -1; if (run() != -1 || tr.find("U;") != std::string::npos) return 21;
    calc_ret = 0; lock_ret = -1; if (run() != -1 || tr.find("Pk;") != std::string::npos) return 22;
    lock_ret = 0; init_ret = -1; if (run() != -1 || tr.find("L;") != std::string::npos) return 23;
    init_ret = 0; create_ret = -2; if (run() != -1 || tr.find("Pb;") != std::string::npos) return 24;
    create_ret = 7; tag_in = 0x1231; if (run() != -1 || tr != "B;") return 25;
    tag_in = 0x1230; begin_ret = -1; if (run() != -1 || tr != "B;") return 26;
    return 0;
}
