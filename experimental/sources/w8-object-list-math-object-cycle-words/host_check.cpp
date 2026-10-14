// Host check for the object cycle words. Not an EE plan input.
#include <cstdio>
extern "C" { extern int objectKillingEveryone, slowListCycle, mediumListCycle; }
int main() {
    if (objectKillingEveryone != 0 || slowListCycle != 0 || mediumListCycle != 0) { std::fprintf(stderr, "fail: nonzero start\n"); return 1; }
    std::printf("object cycle words check passed\n");
    return 0;
}
