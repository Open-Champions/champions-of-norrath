// Host contract for msg_save__3OrcPc; not an EE input. The Creature routine is a stub that
// writes a marker and returns a chosen count; the contract checks the four appended words,
// the returned count and that bytes outside the written range stay untouched.
#include <cstring>
#include <initializer_list>
static int count_to_return; static void* seen_self; static char* seen_buf;
extern "C" int msg_save__8CreaturePc(void* self, char* buffer) {
    seen_self = self; seen_buf = buffer;
    std::memset(buffer, 0x11, count_to_return);
    return count_to_return;
}
#include "msg_save.cpp"
int main() {
    alignas(16) static unsigned char o[2048];
    alignas(16) static unsigned char buf[256];
    int bad = 0;
    for (int n : {0, 8, 20, 64}) {
        std::memset(o, 0, sizeof o); std::memset(buf, 0x7f, sizeof buf);
        const int w[4] = {0x01020304, -77, 0x55aa55aa, 123456};
        std::memcpy(o + 1116, &w[0], 4); std::memcpy(o + 972, &w[1], 4);
        std::memcpy(o + 1112, &w[2], 4); std::memcpy(o + 208, &w[3], 4);
        count_to_return = n;
        const int r = msg_save__3OrcPc(o, reinterpret_cast<char*>(buf));
        if (r != n + 16 || seen_self != o || seen_buf != reinterpret_cast<char*>(buf)) bad = 1;
        if (std::memcmp(buf + n, w, 16) != 0) bad = 1;
        for (int i = 0; i < n; ++i) if (buf[i] != 0x11) bad = 1;
        for (int i = n + 16; i < 256; ++i) if (buf[i] != 0x7f) bad = 1;
    }
    return bad;
}
