// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
// Stubs record the key passed to textGetString__FPc and the arguments of ustrcpy__FPUsT0; the check
// requires the key "no", that the stub's text is what is copied, and that the destination is the buffer.
#include "get_rank_description.cpp"
#include <cstdio>
#include <cstring>

static const unsigned short kText[] = {'o', 'k', 0};
static const char* seenKey = nullptr;
static char seenKeyCopy[16];
static unsigned short* seenTo = nullptr;
static const unsigned short* seenFrom = nullptr;
static int copies = 0;

extern "C" const unsigned short* textGetString__FPc(const char* key) {
    std::strncpy(seenKeyCopy, key, sizeof seenKeyCopy - 1);
    seenKey = seenKeyCopy;
    return kText;
}
extern "C" void ustrcpy__FPUsT0(unsigned short* to, const unsigned short* from) {
    seenTo = to;
    seenFrom = from;
    ++copies;
}

int main() {
    unsigned short buffer[8] = {1, 2, 3};
    for (int rank = -1; rank <= 9; ++rank) {
        seenKey = nullptr; seenTo = nullptr; seenFrom = nullptr; copies = 0;
        getRankDescription__5SkillPUsi(nullptr, buffer, rank);
        if (!seenKey || std::strcmp(seenKey, "no") != 0 || copies != 1 || seenTo != buffer || seenFrom != kText) {
            std::puts("FAIL");
            return 1;
        }
    }
    std::puts("ok");
    return 0;
}
