// Host check for msgHudAssignSpellBookSpell__Fiiil. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "msg_hud_assign_spell_book_spell.cpp"

#include <cstdio>
#include <cstring>

int hudMsgHandle;
static int calls, seenHandle, seenLength;
static unsigned char seen[16];
void netSendMessage__FiPUci(int handle, unsigned char* data, int length) {
    ++calls; seenHandle = handle; seenLength = length;
    std::memcpy(seen, data, 16);
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    hudMsgHandle = 9;
    msgHudAssignSpellBookSpell__Fiiil(1, 2, 0x11223344, 0x0102030405060708LL);
    CHECK(calls == 1 && seenHandle == 9 && seenLength == 16);
    CHECK(seen[0] == 1 && seen[1] == 1 && seen[2] == 2 && seen[3] == 0);
    const unsigned char word[4] = {0x44, 0x33, 0x22, 0x11};
    CHECK(std::memcmp(seen + 4, word, 4) == 0);
    const unsigned char dword[8] = {8, 7, 6, 5, 4, 3, 2, 1};
    CHECK(std::memcmp(seen + 8, dword, 8) == 0);
    hudMsgHandle = -3;
    msgHudAssignSpellBookSpell__Fiiil(0x1ff, -2, -1, -1LL);
    CHECK(calls == 2 && seenHandle == -3);
    CHECK(seen[0] == 1 && seen[1] == 0xff && seen[2] == 0xfe && seen[3] == 0);
    const unsigned char ff[4] = {255, 255, 255, 255};
    CHECK(std::memcmp(seen + 4, ff, 4) == 0 && std::memcmp(seen + 8, ff, 4) == 0 && std::memcmp(seen + 12, ff, 4) == 0);
    std::puts("ok");
    return 0;
}
