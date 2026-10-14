// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "save.cpp"
#include <cstdio>
#include <cstring>
int main() {
    int bad = 0;
    for (unsigned flag = 0; flag < 2; ++flag) {
        unsigned char object[100];
        for (unsigned i = 0; i < sizeof object; ++i) object[i] = static_cast<unsigned char>(i * 13 + 9);
        object[88] = flag ? 0xfe : 0x05;
        unsigned char out[16];
        std::memset(out, 0xee, sizeof out);
        const int n = msg_save__9DoorSwingPc(object, reinterpret_cast<char*>(out + 1));
        if (n != 8) { std::printf("count %d\n", n); ++bad; }
        if (std::memcmp(out + 1, object + 80, 2) != 0) { std::printf("half 80\n"); ++bad; }
        if (out[3] != (flag ? 0xfe : 0x05) || out[4] != (flag ? 0xff : 0x00)) { std::printf("byte 88 extension\n"); ++bad; }
        if (std::memcmp(out + 5, object + 84, 4) != 0) { std::printf("word 84\n"); ++bad; }
        if (out[0] != 0xee || out[9] != 0xee) { std::printf("overrun\n"); ++bad; }
    }
    std::printf(bad ? "failed\n" : "msg_save__9DoorSwingPc passed\n");
    return bad != 0;
}
