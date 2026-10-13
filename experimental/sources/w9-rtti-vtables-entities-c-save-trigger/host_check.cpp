// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "save.cpp"
#include <cstdio>
#include <cstring>
int main() {
    int bad = 0;
    unsigned char object[128];
    for (unsigned i = 0; i < sizeof object; ++i) object[i] = static_cast<unsigned char>(i * 7 + 3);
    unsigned char out[16];
    std::memset(out, 0xee, sizeof out);
    const int n = msg_save__7TriggerPc(object, reinterpret_cast<char*>(out + 1));
    if (n != 5) { std::printf("count %d\n", n); ++bad; }
    if (std::memcmp(out + 1, object + 100, 4) != 0) { std::printf("word\n"); ++bad; }
    if (out[5] != object[96]) { std::printf("byte\n"); ++bad; }
    if (out[0] != 0xee || out[6] != 0xee) { std::printf("overrun\n"); ++bad; }
    std::printf(bad ? "failed\n" : "msg_save__7TriggerPc passed\n");
    return bad != 0;
}
