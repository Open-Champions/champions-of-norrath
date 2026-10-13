// Host contract for msg_save__6GoblinPc; not an EE input.
#include <cstdio>
#include <cstring>
static int count = 12;
extern "C" int msg_save__8CreaturePc(void*, char* b) { std::memset(b, 0xaa, 12); return count; }
#include "msg_save.cpp"
int main() {
    alignas(16) unsigned char object[1100]; alignas(16) char buffer[64]; int bad = 0;
    std::memset(object, 0, sizeof object); std::memset(buffer, 0x55, sizeof buffer);
    unsigned v972 = 0x11112222u, v988 = 0x33334444u, v992 = 0x55556666u, v208 = 0x77778888u;
    std::memcpy(object + 972, &v972, 4); std::memcpy(object + 988, &v988, 4);
    std::memcpy(object + 992, &v992, 4); std::memcpy(object + 208, &v208, 4);
    int n = msg_save__6GoblinPc(object, buffer);
    unsigned w[4]; std::memcpy(w, buffer + 12, 16);
    if (n != 28) { std::puts("fail: return n + 16"); bad = 1; }
    if (w[0] != v972 || w[1] != v988 || w[2] != v992 || w[3] != v208) { std::puts("fail: word order +972, +988, +992, +208"); bad = 1; }
    if (static_cast<unsigned char>(buffer[11]) != 0xaa || static_cast<unsigned char>(buffer[28]) != 0x55) { std::puts("fail: writes stay in [n, n+16)"); bad = 1; }
    if (!bad) std::puts("goblin msg_save ok");
    return bad;
}
