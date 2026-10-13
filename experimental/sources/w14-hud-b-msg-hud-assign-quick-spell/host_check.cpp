// Host check for msgHudAssignQuickSpell__Fiii. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "msg_hud_assign_quick_spell.cpp"

#include <cstdio>
#include <cstring>

int hudMsgHandle;
static int calls, seenHandle, seenLength;
static unsigned char seen[8];
void netSendMessage__FiPUci(int handle, unsigned char* data, int length) {
    ++calls; seenHandle = handle; seenLength = length;
    std::memcpy(seen, data, 4);
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    hudMsgHandle = 77;
    msgHudAssignQuickSpell__Fiii(1, 2, 3);
    CHECK(calls == 1 && seenHandle == 77 && seenLength == 4);
    CHECK(seen[0] == 0 && seen[1] == 1 && seen[2] == 2 && seen[3] == 3);
    hudMsgHandle = -5;
    msgHudAssignQuickSpell__Fiii(0x1234, -1, 0x80);
    CHECK(calls == 2 && seenHandle == -5);
    CHECK(seen[0] == 0 && seen[1] == 0x34 && seen[2] == 0xff && seen[3] == 0x80);
    std::puts("ok");
    return 0;
}
