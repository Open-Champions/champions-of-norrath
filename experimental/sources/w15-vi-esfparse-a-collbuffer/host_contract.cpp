#include <stdint.h>
#include <string.h>
#include <string>
#include <math.h>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#define powf host_powf
#include "parse_coll_buffer.cpp"
#undef powf

static std::string tr; static int begin_ret, end_ret, create_ret, init_ret, lock_ret, unlock_ret, pack_ret, calc_ret; static unsigned short tag_in, ver_in;
static int ints[32]; static unsigned ni; static short shorts[64]; static unsigned ns; static signed char sbytes[16]; static unsigned nsb; static float floats[32]; static unsigned nf;
static unsigned buffer_type; static int calc_count; static void* calc_table; static int init_args[5]; static float pow_args[2]; static int pack_seen;
alignas(16) static unsigned char buffer_mem[64]; alignas(16) static unsigned char object_mem[128]; alignas(16) static float table_mem[16];
static float vf[8][3]; static unsigned nvf; static short vs[8][3]; static unsigned nvs; static signed char groups[8]; static unsigned ng; static signed char floras[8]; static unsigned nfl;
static int collide_arg, collide_seen_handle;
extern "C" float host_powf(float a, float b) { pow_args[0] = a; pow_args[1] = b; return a == 2.0f && b == 3.0f ? 8.0f : (float)pow(a, b); }
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = ver_in; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = ints[ni++]; tr += "i;"; return 0; }
extern "C" int Read__9VIObjFileRs(void*, short* v) { *v = shorts[ns++]; tr += "s;"; return 0; }
extern "C" int Read__9VIObjFileRSc(void*, signed char* v) { *v = sbytes[nsb++]; tr += "c;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = floats[nf++]; tr += "f;"; return 0; }
extern "C" int CreateCollBuffer__9VICollidei(void* c, int a) { if (c != reinterpret_cast<void*>(0x66)) tr += "BADCOLL"; collide_arg = a; tr += "Cr;"; return create_ret; }
extern "C" void* CollBuffer__9VICollidei(void* c, int h) { if (c != reinterpret_cast<void*>(0x66)) tr += "BADCOLL"; collide_seen_handle = h; *reinterpret_cast<unsigned*>(buffer_mem + 8) = buffer_type; tr += "Cb;"; return buffer_mem; }
extern "C" int Init__12VICollBuffer16VICollBufferTypeiii(void* b, int k, int a, int g, int c) { if (b != buffer_mem) tr += "BADBUF"; init_args[0] = k; init_args[1] = a; init_args[2] = g; init_args[3] = c; tr += "In;"; return init_ret; }
extern "C" int Lock__12VICollBuffer(void*) { tr += "L;"; return lock_ret; }
extern "C" int Unlock__12VICollBuffer(void*) { tr += "U;"; return unlock_ret; }
extern "C" int SetPacking__12VICollBufferi(void*, int p) { pack_seen = p; tr += "Pk;"; return pack_ret; }
extern "C" int BeginPrimGroup__12VICollBuffer(void*) { tr += "BG;"; return 0; }
extern "C" int EndPrimGroup__12VICollBuffer(void*) { tr += "EG;"; return 0; }
extern "C" int Begin__12VICollBufferi(void*, int a) { tr += "Bg" + std::to_string(a) + ";"; return 0; }
extern "C" int End__12VICollBuffer(void*) { tr += "En;"; return 0; }
extern "C" int Vertex__12VICollBufferR7VIVect3(void*, const float* v) { memcpy(vf[nvf++], v, 12); tr += "Vf;"; return 0; }
extern "C" int Vertex__12VICollBufferR10VIVect3S16(void*, const short* v) { memcpy(vs[nvs++], v, 6); tr += "Vs;"; return 0; }
extern "C" int VertexGroup__12VICollBufferSc(void*, signed char g) { groups[ng++] = g; tr += "Vg;"; return 0; }
extern "C" int FloraType__12VICollBufferSc(void*, signed char t) { floras[nfl++] = t; tr += "Fl;"; return 0; }
extern "C" int CalcBVolumes__12VICollBufferiP7VIVect3(void*, int n, void* t) { calc_count = n; calc_table = t; tr += "Cv;"; return calc_ret; }
static unsigned object_word;
static int run(unsigned version_words = 1) {
    (void)version_words;
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[5] = 0x66; w[7] = object_word; w[9] = 0x55;
    tr.clear(); ni = ns = nsb = nf = nvf = nvs = ng = nfl = 0;
    return ParseCollBuffer__10VIESFParse(parse); }
int main() {
    begin_ret = end_ret = init_ret = lock_ret = unlock_ret = pack_ret = calc_ret = 0; create_ret = 7; tag_in = 0x4200;
    unsigned long taddr = static_cast<unsigned long>(reinterpret_cast<uintptr_t>(table_mem));
    (void)taddr;
    memset(object_mem, 0, sizeof object_mem);
    *reinterpret_cast<int*>(object_mem + 84) = 5; *reinterpret_cast<unsigned*>(object_mem + 88) = static_cast<unsigned>(reinterpret_cast<uintptr_t>(table_mem));
    for (int i = 0; i < 16; ++i) table_mem[i] = 100.0f + i;
    object_word = static_cast<unsigned>(reinterpret_cast<uintptr_t>(object_mem));
    // kind 0, version 1: kind and packing are not read; a=1, two groups
    ver_in = 1; ints[0] = 1; ints[1] = 2; ints[2] = 9; ints[3] = 2; ints[4] = 4; ints[5] = 7; ints[6] = 1; ints[7] = 4; ints[8] = 8;
    for (int i = 0; i < 9; ++i) floats[i] = 1.5f + i;
    buffer_type = 99;
    if (run() != 7) return 1;
    if (tr != "B;i;i;i;Cr;Cb;In;L;Pk;i;i;i;BG;Bg7;f;f;f;Vf;f;f;f;Vf;En;i;i;i;Bg8;f;f;f;Vf;En;EG;Cv;U;E;") return 2;
    if (init_args[0] != 0 || init_args[1] != 1 || init_args[2] != 2 || init_args[3] != 9 || pack_seen != 0 || collide_arg != 0x99 || collide_seen_handle != 7) return 3;
    if (vf[0][0] != 1.5f || vf[1][2] != 6.5f || vf[2][1] != 8.5f || nvf != 3) return 4;
    if (calc_count != 5 || calc_table != table_mem) return 5;
    // object word zero: CalcBVolumes gets 0 and 0
    object_word = 0; if (run() != 7 || calc_count != 0 || calc_table != 0) return 6;
    object_word = static_cast<unsigned>(reinterpret_cast<uintptr_t>(object_mem));
    *reinterpret_cast<int*>(object_mem + 84) = 0; if (run() != 7 || calc_count != 0 || calc_table != 0) return 7;
    *reinterpret_cast<int*>(object_mem + 84) = 5;
    // a == 0: no trailing EndPrimGroup
    ints[0] = 0; if (run() != 7 || tr.find("EG;") != std::string::npos) return 8;
    ints[0] = 1;
    // kind 1, version 2: kind, a, groups, c, packing
    ver_in = 2; ints[0] = 1; ints[1] = 0; ints[2] = 1; ints[3] = 3; ints[4] = 3; ints[5] = 2; ints[6] = 6; ints[7] = 4;
    for (int i = 0; i < 6; ++i) shorts[i] = (short)(10 + i);
    if (run() != 7 || init_args[0] != 1 || pack_seen != 3 || pow_args[0] != 2.0f || pow_args[1] != 3.0f) return 9;
    if (tr != "B;i;i;i;i;i;Cr;Cb;In;L;Pk;i;i;i;BG;Bg4;s;s;s;Vs;s;s;s;Vs;En;Cv;U;E;" || vs[1][2] != 15 || vs[0][0] != 10) return 10;
    // kind 2, buffer type 2: 4 shorts, the last is the vertex group
    ints[0] = 2; buffer_type = 2; shorts[3] = 0x0155; shorts[7] = -2;
    if (run() != 7 || ng != 2 || groups[0] != 0x55 || groups[1] != -2 || nvs != 2 || nfl != 0) return 11;
    // kind 2, buffer type 3: group and flora type -1
    buffer_type = 3; if (run() != 7 || ng != 2 || nfl != 2 || floras[0] != -1 || nvs != 2) return 12;
    // kind 2, buffer type 0: float positions = short * (1 / 2^3) + table entry of 12 bytes * group
    buffer_type = 0; shorts[0] = 16; shorts[1] = -8; shorts[2] = 4; shorts[3] = 1; ints[7] = 4;
    ints[2] = 1; ints[5] = 1;
    if (run() != 7 || nvf != 1 || nvs != 0 || ng != 0) return 13;
    if (vf[0][0] != 16 * 0.125f + table_mem[3] || vf[0][1] != -8 * 0.125f + table_mem[4] || vf[0][2] != 4 * 0.125f + table_mem[5]) return 14;
    // kind 2, other buffer type
    buffer_type = 1; if (run() != -1 || tr.find("Cv;") != std::string::npos) return 15;
    // kind 3: shorts then two signed bytes
    ints[0] = 3; buffer_type = 3; sbytes[0] = -7; sbytes[1] = 5; sbytes[2] = 9; sbytes[3] = 8;
    if (run() != 7 || ng != 1 || groups[0] != -7 || floras[0] != 5 || nvs != 1) return 16;
    buffer_type = 2; if (run() != -1 || tr.find("Bg") != std::string::npos) return 17;
    // unknown kind
    buffer_type = 3; ints[0] = 4; if (run() != -1) return 18;
    ints[0] = 1;
    // tolerated failures and early exits
    ints[5] = 2;
    end_ret = -1; if (run() != -1) return 19;
    end_ret = 0; unlock_ret = -1; if (run() != -1 || tr.find("E;") != std::string::npos) return 20;
    unlock_ret = 0; calc_ret = -1; if (run() != -1 || tr.find("U;") != std::string::npos) return 21;
    calc_ret = 0; pack_ret = -1; if (run() != -1 || tr.find("BG;") != std::string::npos) return 22;
    pack_ret = 0; lock_ret = -1; if (run() != -1 || tr.find("Pk;") != std::string::npos) return 23;
    lock_ret = 0; init_ret = -1; if (run() != -1 || tr.find("L;") != std::string::npos) return 24;
    init_ret = 0; create_ret = -2; if (run() != -1 || tr.find("Cb;") != std::string::npos) return 25;
    create_ret = 7; tag_in = 0x4201; if (run() != -1 || tr != "B;") return 26;
    tag_in = 0x4200; begin_ret = -1; if (run() != -1 || tr != "B;") return 27;
    return 0;
}
