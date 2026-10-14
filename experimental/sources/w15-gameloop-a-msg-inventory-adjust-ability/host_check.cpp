// Host check for msgInventoryAdjustAbility__Fii. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "msg_inventory_adjust_ability.cpp"

#include <cstdio>
#include <cstring>

int invMsgHandle;
static int calls, seenHandle, seenLength;
static unsigned char seen[4];
void netSendMessage__FiPUci(int handle, unsigned char* data, int length) {
    ++calls; seenHandle = handle; seenLength = length;
    std::memcpy(seen, data, 4);
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    invMsgHandle = 41;
    msgInventoryAdjustAbility__Fii(3, 0x1234);
    CHECK(calls == 1 && seenHandle == 41 && seenLength == 4);
    CHECK(seen[0] == 6 && seen[1] == 3);
    short amount; std::memcpy(&amount, seen + 2, 2);
    CHECK(amount == 0x1234);
    msgInventoryAdjustAbility__Fii(0x1ff, -2);
    CHECK(calls == 2 && seen[1] == 0xff);
    std::memcpy(&amount, seen + 2, 2);
    CHECK(amount == -2);
    std::puts("ok");
    return 0;
}
