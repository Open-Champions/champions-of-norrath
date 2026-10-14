#define NORRATH_SQ_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> pool;
extern "C" int objectSaveAddToStringPool__FPCc(const char* t) { pool.push_back(t); return 70 + (int)pool.size(); }
#include "cosmetic_save.cpp"
using sq::read_word;
using sq::write_word;
int main() {
    unsigned char object[160];
    unsigned char buf[48];
    char t0[] = "a", t1[] = "b", t2[] = "c", t3[] = "d";
    std::memset(object, 0, sizeof object);
    std::memset(buf, 0x77, sizeof buf);
    write_word(object + 76, sq::pointer_bits(t0));
    write_word(object + 80, sq::pointer_bits(t1));
    write_word(object + 84, sq::pointer_bits(t2));
    write_word(object + 88, sq::pointer_bits(t3));
    write_word(object + 12, 0x01020304u);
    write_word(object + 148, 0x0a0b0c0du);
    write_word(object + 144, 0x55667788u);
    sq::write_single(object + 32, -3.9f);
    sq::write_single(object + 28, 1234.7f);
    int n = msg_save__16CosmeticPropAnimPc(object, (char*)buf);
    if (n != 32) return 1;
    if (pool.size() != 4 || pool[0] != "a" || pool[1] != "b" || pool[2] != "c" || pool[3] != "d") return 2;
    for (int i = 0; i < 4; ++i) if (read_word(buf + i * 4) != 71u + i) return 3;
    if (read_word(buf + 16) != 0x01020304u || read_word(buf + 20) != 0x0a0b0c0du || read_word(buf + 24) != 0x55667788u) return 4;
    if ((short)(buf[28] | (buf[29] << 8)) != -3 || (short)(buf[30] | (buf[31] << 8)) != 1234 || buf[32] != 0x77) return 5;
    std::puts("ok");
    return 0;
}
