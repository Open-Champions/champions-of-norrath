// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "save.cpp"
#include <cstdio>
#include <cstring>
int main() {
    int bad = 0;
    unsigned char object[260];
    for (unsigned i = 0; i < sizeof object; ++i) object[i] = static_cast<unsigned char>(i * 11 + 5);
    unsigned char out[20];
    std::memset(out, 0xee, sizeof out);
    const int n = msg_save__9AutoChestPc(object, reinterpret_cast<char*>(out + 1));
    if (n != 12) { std::printf("count %d\n", n); ++bad; }
    if (std::memcmp(out + 1, object + 200, 4) != 0) { std::printf("word 200\n"); ++bad; }
    if (std::memcmp(out + 5, object + 220, 2) != 0) { std::printf("half 220\n"); ++bad; }
    if (out[7] != object[228]) { std::printf("byte 228\n"); ++bad; }
    if (out[8] != object[229]) { std::printf("byte 229\n"); ++bad; }
    if (std::memcmp(out + 9, object + 216, 4) != 0) { std::printf("word 216\n"); ++bad; }
    if (out[0] != 0xee || out[13] != 0xee) { std::printf("overrun\n"); ++bad; }
    std::printf(bad ? "failed\n" : "msg_save__9AutoChestPc passed\n");
    return bad != 0;
}
