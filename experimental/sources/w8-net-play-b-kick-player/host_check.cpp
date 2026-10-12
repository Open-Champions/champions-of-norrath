// Host check for netKickPlayer. Not an EE plan input.
// Build on the host with: c++ -std=c++17 -fno-builtin host_check.cpp
#include "net_kick_player.cpp"
extern "C" {
int nmPartners;
unsigned char partners[70528];
unsigned kickState[2];
}
static int local_result, local_calls, local_arg, memset_calls, memset_len;
static bool memset_saw_marks;
extern "C" int netGetLocalPlayerNm__Fi(int p) { ++local_calls; local_arg = p; return local_result; }
extern "C" void* memset(void* d, int v, unsigned long n) {
    ++memset_calls; memset_len = (int)n;
    memset_saw_marks = partners[2 * 17632 + 17622] == 1;  // partner stores happen before the clear
    unsigned char* b = (unsigned char*)d;
    for (unsigned long i = 0; i < n; ++i) b[i] = (unsigned char)v;
    return d;
}
static void reset() {
    for (auto& b : partners) b = 0;
    for (int i = 0; i < 2; ++i) kickState[i] = 0xdeadbeef;
    local_result = 0; local_calls = memset_calls = 0; nmPartners = 3;
}
static bool untouched() {
    for (unsigned char b : partners) if (b) return false;
    return kickState[0] == 0xdeadbeef && kickState[1] == 0xdeadbeef && memset_calls == 0;
}
int main() {
    reset();
    netKickPlayer__Fi(3);  // not below nmPartners: no call at all
    if (local_calls != 0 || !untouched()) return 1;
    netKickPlayer__Fi(2);
    if (local_calls != 1 || local_arg != 0) return 2;
    if (memset_calls != 1) return 30; if (memset_len != 8) return 31; if (!memset_saw_marks) return 32;
    unsigned char* r = partners + 2 * 17632;
    if (r[17622] != 1 || *(int*)(r + 17624) != -1 || *(int*)(r + 17628) != 1) return 4;
    if (kickState[0] != 1 || kickState[1] != 0) return 5;
    for (int i = 0; i < (int)sizeof partners; ++i) {
        bool mine = i == 2 * 17632 + 17622 || (i >= 2 * 17632 + 17624 && i < 2 * 17632 + 17632);
        if (!mine && partners[i]) return 6;
    }
    reset(); local_result = 256;  // a nonzero local-player result blocks everything
    netKickPlayer__Fi(1);
    if (local_calls != 1 || !untouched()) return 7;
    for (int off = 17620; off <= 17622; ++off) {  // each guard byte blocks, even a negative one
        reset();
        partners[17632 + off] = 0x80;
        netKickPlayer__Fi(1);
        if (memset_calls != 0 || kickState[0] != 0xdeadbeef || partners[17632 + 17622] != (off == 17622 ? 0x80 : 0)) return 8;
    }
    reset(); partners[17632 + 17623] = 1;  // byte 17623 is not a guard
    netKickPlayer__Fi(1);
    return kickState[0] == 1 ? 0 : 9;
}
