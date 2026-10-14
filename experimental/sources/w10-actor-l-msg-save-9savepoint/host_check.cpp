// Host contract for msg_save__9SavepointPc; not an EE input. Checks the word choice (+132 over
// +128 unless zero), the two bytes, the count and that nothing else in the buffer or object changes.
#include <cstring>
#include <initializer_list>
#include "msg_save.cpp"
int main() {
    alignas(16) static unsigned char o[256], ref[256], buf[32];
    int bad = 0;
    for (unsigned w132 : {0u, 1u, 0xffffffffu, 0x80000000u}) for (unsigned w128 : {0u, 7u, 0xdeadbeefu}) {
        std::memset(o, 0x5a, sizeof o);
        std::memcpy(o + 132, &w132, 4); std::memcpy(o + 128, &w128, 4);
        o[197] = 0xf0; o[198] = 0x0f; std::memcpy(ref, o, sizeof o); std::memset(buf, 0x7f, sizeof buf);
        const int r = msg_save__9SavepointPc(o, reinterpret_cast<char*>(buf));
        const unsigned want = w132 ? w132 : w128;
        if (r != 6 || std::memcmp(buf, &want, 4) || buf[4] != 0xf0 || buf[5] != 0x0f) bad = 1;
        for (int i = 6; i < 32; ++i) if (buf[i] != 0x7f) bad = 1;
        if (std::memcmp(o, ref, sizeof o)) bad = 1;
    }
    return bad;
}
