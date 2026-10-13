// Host contract for msg_save__5ChestPc; not an EE input. Includes the source.
#include "msg_save.cpp"
#include <cstdio>
#include <cstring>
int main() {
    alignas(16) static unsigned char object[200];
    alignas(16) static unsigned char buffer[32];
    for (int i = 0; i < 200; ++i) object[i] = static_cast<unsigned char>(i * 7 + 1);
    std::memset(buffer, 0xee, sizeof buffer);
    int n = msg_save__5ChestPc(object, reinterpret_cast<char*>(buffer));
    unsigned char want[12];
    std::memcpy(want, object + 84, 4); std::memcpy(want + 4, object + 76, 2);
    want[6] = object[88]; want[7] = object[89]; std::memcpy(want + 8, object + 132, 4);
    if (n != 12 || std::memcmp(buffer, want, 12) != 0) { std::puts("fail: payload"); return 1; }
    for (int i = 12; i < 32; ++i) if (buffer[i] != 0xee) { std::puts("fail: wrote past 12 bytes"); return 1; }
    return 0;
}
