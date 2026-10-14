#define NORRATH_SQ_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> trace;
static float got_quat[4];
static void* got_matrix;
static int lump_calls;
extern "C" {
const char* objectSaveGetFromStringPool__Fi(int i) {
    trace.push_back("pool" + std::to_string(i));
    static char names[3][8] = {"model", "tex", "anim"};
    return names[(i - 10) % 3];
}
void quatToMat__FP8Matrix34P4Quat(void* m, void* q) { got_matrix = m; std::memcpy(got_quat, q, 16); }
static char r1, r2;
void* lumpFindResource__FPcT0(char* a, char* b) {
    lump_calls += 1;
    trace.push_back(std::string("lump:") + a + "," + b);
    return lump_calls == 1 ? (void*)&r1 : (void*)&r2;
}
}
#include "prop_load.cpp"
using sq::read_word;
using sq::write_word;
static unsigned char object[400];
static unsigned char buf[64];
static void head() {
    std::memset(object, 0, sizeof object);
    std::memset(buf, 0, sizeof buf);
    trace.clear();
    lump_calls = 0;
    write_word(object + 12, 0x4u);
    write_word(buf, 10u); write_word(buf + 4, 11u); write_word(buf + 8, 12u);
}
static void tail(unsigned char* p, int q0, int q1, int q2, int q3, int a, int b, int c) {
    p[0] = (unsigned char)q0; p[1] = (unsigned char)q1; p[2] = (unsigned char)q2;
    p[3] = (unsigned char)q3; p[4] = (unsigned char)(q3 >> 8);
    p[5] = (unsigned char)a; p[6] = (unsigned char)(a >> 8);
    p[7] = (unsigned char)b; p[8] = (unsigned char)(b >> 8);
    p[9] = (unsigned char)c; p[10] = (unsigned char)(c >> 8);
}
int main() {
    // exponent byte
    head();
    buf[12] = 40;
    tail(buf + 13, 127, -127, 64, -9999, -5, 300, -32768);
    int n = msg_load__4PropPc(object, (char*)buf);
    if (n != 24) return 1;
    if (trace[0] != "pool10" || trace[1] != "pool11" || trace[2] != "pool12") return 2;
    if (std::strcmp((char*)object + 84, "model") || std::strcmp((char*)object + 100, "tex") || std::strcmp((char*)object + 164, "anim")) return 3;
    if (read_word(object + 12) != 0x1004u) return 4;
    if (read_word(object + 232) != 0u || read_word(object + 236) != 256u) return 5;
    if (got_matrix != object + 272) return 6;
    if (got_quat[0] != 127 * __builtin_bit_cast(float, 0x3c010204u) || got_quat[1] != -127 * __builtin_bit_cast(float, 0x3c010204u)) return 7;
    if (got_quat[2] != 64 * __builtin_bit_cast(float, 0x3c010204u)) return 8;
    if (got_quat[3] != -9999 * __builtin_bit_cast(float, 0x38d1b717u)) return 9;
    if (sq::read_single(object + 308) != -5.0f || sq::read_single(object + 312) != 300.0f || sq::read_single(object + 316) != -32768.0f) return 10;
    if (read_word(object + 76) != sq::pointer_bits(&r1) || read_word(object + 80) != sq::pointer_bits(&r2)) return 11;
    if (trace[3] != "lump:model,tex" || trace[4] != "lump:model,anim") return 12;
    // escape: -1 then seven bytes, eight value bytes, one more byte
    head();
    buf[12] = 0xff; buf[13] = buf[14] = buf[15] = 0xff;
    write_word(buf + 16, 0x89abcdefu); write_word(buf + 20, 0x01234567u);
    buf[24] = 0;
    tail(buf + 25, 0, 0, 0, 0, 1, 2, 3);
    n = msg_load__4PropPc(object, (char*)buf);
    if (n != 36 || read_word(object + 232) != 0x89abcdefu || read_word(object + 236) != 0x01234567u) return 13;
    if (sq::read_single(object + 316) != 3.0f) return 14;
    // exponent 63 and exponent 64 (shift count is taken modulo 64)
    head(); buf[12] = 63; tail(buf + 13, 0, 0, 0, 0, 0, 0, 0);
    msg_load__4PropPc(object, (char*)buf);
    if (read_word(object + 232) != 0u || read_word(object + 236) != 0x80000000u) return 15;
    head(); buf[12] = 0; tail(buf + 13, 0, 0, 0, 0, 0, 0, 0);
    msg_load__4PropPc(object, (char*)buf);
    if (read_word(object + 232) != 1u || read_word(object + 236) != 0u) return 16;
    std::puts("ok");
    return 0;
}
