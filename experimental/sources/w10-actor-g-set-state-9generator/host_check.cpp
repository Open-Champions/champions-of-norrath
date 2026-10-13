// Host contract for setState__9Generatori; not an EE input. Includes the source.
#include "set_state.cpp"
#include <cstdio>
#include <cstring>
static int failures, procResult, procForce = -1000, procSeen;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }
extern "C" int setStateProcessing__8CreaturePi(void*, int* state) {
    procSeen = *state; if (procForce != -1000) *state = procForce; return procResult;
}
static unsigned short half(const unsigned char* o) { unsigned short h; std::memcpy(&h, o + 572, 2); return h; }
int main() {
    alignas(16) unsigned char o[1024];
    for (int s = -3; s < 40; ++s) {
        std::memset(o, 0, sizeof o); unsigned short seed = 0x1234; std::memcpy(o + 572, &seed, 2);
        procResult = 0; procForce = -1000;
        setState__9Generatori(o, s);
        const bool keep = (s == 5 || s == 8 || s == 9 || s == 10);
        expect(procSeen == s, "processing sees the requested state");
        expect(half(o) == (keep ? seed : static_cast<unsigned short>(s)), "keep set or low half stored");
    }
    std::memset(o, 0, sizeof o); procResult = 1; procForce = 7;
    setState__9Generatori(o, 2); expect(half(o) == 0, "nonzero processing result stores nothing");
    procResult = 0; procForce = 9; std::memset(o, 0, sizeof o); o[572] = 0x55;
    setState__9Generatori(o, 2); expect(half(o) == 0x55, "processing may rewrite the state before the table");
    procForce = 3;
    setState__9Generatori(o, 9); expect(half(o) == 3, "rewritten state is the one stored");
    if (!failures) std::puts("generator setState ok");
    return failures ? 1 : 0;
}
