#include <cstdio>
#include "yes_no_state.cpp"
static_assert(sizeof(yesNoPos) == 16 && sizeof(activeDelay) == 16, "four words each");
int main() {
    for (int i = 0; i < 4; ++i) if (yesNoPos[i] != 0 || activeDelay[i] != 3) return 1;
    std::puts("yes/no state check passed");
    return 0;
}
