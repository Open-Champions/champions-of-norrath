// Host contract for msg_save__8RollDoorPc; not an EE input. Checks the four words, the order, the
// sign extension of the two halfwords, the float bit pattern, the returned count and that no byte
// outside the first sixteen of the buffer or in the object changes.
#include <cstring>
#include <initializer_list>
#include "msg_save.cpp"
int main() {
    alignas(16) static unsigned char o[128], ref[128], buf[64];
    int bad = 0;
    const short h90[] = {0, 1, -1, 32767, -32768, 0x1234};
    for (short a : h90) for (short b : {short(7), short(-9), short(-32768)}) {
        std::memset(o, 0x5a, sizeof o);
        std::memcpy(o + 90, &a, 2); std::memcpy(o + 76, &b, 2);
        const unsigned fbits = 0x7fc00001u, w84 = 0xdeadbeefu;  // a NaN pattern must pass through
        std::memcpy(o + 80, &fbits, 4); std::memcpy(o + 84, &w84, 4);
        std::memcpy(ref, o, sizeof o); std::memset(buf, 0x7f, sizeof buf);
        const int r = msg_save__8RollDoorPc(o, reinterpret_cast<char*>(buf));
        const int want[2] = {a, b};
        if (r != 16 || std::memcmp(buf, &want[0], 4) || std::memcmp(buf + 4, &want[1], 4) ||
            std::memcmp(buf + 8, &fbits, 4) || std::memcmp(buf + 12, &w84, 4)) bad = 1;
        for (int i = 16; i < 64; ++i) if (buf[i] != 0x7f) bad = 1;
        if (std::memcmp(o, ref, sizeof o)) bad = 1;
    }
    return bad;
}
