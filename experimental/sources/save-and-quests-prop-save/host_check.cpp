#define NORRATH_SQ_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> pool;
static float stub_quat[4];
static int normalize_calls;
static void* last_matrix;
static int lilog2_calls;
extern "C" {
int objectSaveAddToStringPool__FPCc(const char* t) { pool.push_back(t); return 500 + (int)pool.size(); }
int lilog2__Fl(long long v) {
    lilog2_calls += 1;
    int k = -1;
    unsigned long long u = (unsigned long long)v;
    while (u) { k += 1; u >>= 1; }
    return k;
}
void matToQuaternion__FP4QuatP8Matrix34(void* q, void* m) { last_matrix = m; std::memcpy(q, stub_quat, 16); }
void quatNormalize__FP4Quat(void* q) { normalize_calls += 1; ((float*)q)[0] *= 0.5f; }
}
#include "prop_save.cpp"
using sq::read_word;
using sq::write_word;
static unsigned char object[400];
static unsigned char buf[64];
static void setup(unsigned long long value) {
    std::memset(object, 0, sizeof object);
    std::memset(buf, 0x77, sizeof buf);
    pool.clear();
    normalize_calls = lilog2_calls = 0;
    std::strcpy((char*)object + 84, "model");
    std::strcpy((char*)object + 100, "tex");
    std::strcpy((char*)object + 164, "anim");
    write_word(object + 232, (unsigned)value);
    write_word(object + 236, (unsigned)(value >> 32));
    stub_quat[0] = 1.0f; stub_quat[1] = -0.5f; stub_quat[2] = 0.3f; stub_quat[3] = -0.9999f;
    sq::write_single(object + 308, 1234.9f);
    sq::write_single(object + 312, -2.5f);
    sq::write_single(object + 316, 7.0f);
}
int main() {
    // power of two: a single exponent byte
    setup(1ull << 40);
    int n = msg_save__4PropPc(object, (char*)buf);
    if (n != 24) return 1;
    if (read_word(buf) != 501u || read_word(buf + 4) != 502u || read_word(buf + 8) != 503u) return 2;
    if (pool[0] != "model" || pool[1] != "tex" || pool[2] != "anim") return 3;
    if (buf[12] != 40) return 4;
    if (last_matrix != object + 272 || normalize_calls != 1) return 5;
    // quaternion x was halved by the stub normaliser: 0.5 * 127 = 63.5 -> 63
    if (buf[13] != 63 || (signed char)buf[14] != -63 || buf[15] != 38) return 6;
    // -0.9999 * 10000 = -9999 as a halfword
    if ((short)(buf[16] | (buf[17] << 8)) != -9999) return 7;
    if ((short)(buf[18] | (buf[19] << 8)) != 1234) return 8;
    if ((short)(buf[20] | (buf[21] << 8)) != -2) return 9;
    if ((short)(buf[22] | (buf[23] << 8)) != 7 || buf[24] != 0x77) return 10;
    // not a power of two: escape word, eight bytes, one zero byte
    setup(0x0123456789abcdefull);
    n = msg_save__4PropPc(object, (char*)buf);
    if (n != 36) return 11;
    if (read_word(buf + 12) != 0xffffffffu || read_word(buf + 16) != 0x89abcdefu || read_word(buf + 20) != 0x01234567u || buf[24] != 0) return 12;
    if ((signed char)buf[25] != 63) return 13;
    // zero is not a power of two either
    setup(0);
    n = msg_save__4PropPc(object, (char*)buf);
    if (n != 36 || read_word(buf + 12) != 0xffffffffu) return 14;
    // bit 0 is a power of two with exponent 0
    setup(1);
    n = msg_save__4PropPc(object, (char*)buf);
    if (n != 24 || buf[12] != 0) return 15;
    // bit 63
    setup(1ull << 63);
    n = msg_save__4PropPc(object, (char*)buf);
    if (n != 24 || buf[12] != 63) return 16;
    std::puts("ok");
    return 0;
}
