// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "save.cpp"
#include <cstdio>
#include <cstring>
int main() {
    int bad = 0;
    unsigned char object[400];
    for (unsigned i = 0; i < sizeof object; ++i) object[i] = static_cast<unsigned char>(i * 5 + 1);
    unsigned char out[12];
    std::memset(out, 0xee, sizeof out);
    const int n = msg_save__8AutoPropPc(object, reinterpret_cast<char*>(out + 3));
    if (n != 4) { std::printf("count %d\n", n); ++bad; }
    if (std::memcmp(out + 3, object + 376, 4) != 0) { std::printf("word\n"); ++bad; }
    if (out[2] != 0xee || out[7] != 0xee) { std::printf("overrun\n"); ++bad; }
    std::printf(bad ? "failed\n" : "msg_save__8AutoPropPc passed\n");
    return bad != 0;
}
