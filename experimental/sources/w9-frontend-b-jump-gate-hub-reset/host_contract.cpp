#include "jump_gate_hub_reset.cpp"

#include <cstdio>

signed char MC_SavingOrLoading;
signed char jumpGateDestCount = 9;
char jumpGateEntries[480];
const char* hubNames[] = {"alpha", "bravo", "charlie"};
static int copies, count_at_copy;
extern "C" char* strcpy(char* dst, const char* src) {
    ++copies;
    count_at_copy = jumpGateDestCount;
    char* r = dst;
    while ((*dst++ = *src++) != 0) {}
    return r;
}

int main() {
    MC_SavingOrLoading = 1;
    jumpGateHubReset__Fi(1);
    if (copies != 0 || jumpGateDestCount != 9 || jumpGateEntries[0] != 0) return 1;
    MC_SavingOrLoading = 0;
    jumpGateHubReset__Fi(2);
    if (copies != 1 || jumpGateDestCount != 1) return 2;
    if (count_at_copy != 1) return 3;  // count is stored before the copy
    const char* want = "charlie";
    for (int i = 0; i < 8; ++i) if (jumpGateEntries[i] != want[i]) return 4;
    jumpGateHubReset__Fi(0);
    if (jumpGateEntries[0] != 'a') return 5;
    std::puts("jump gate hub reset passed");
    return 0;
}
