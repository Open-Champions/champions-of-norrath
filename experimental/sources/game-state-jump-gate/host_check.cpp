// Host contract for jumpGateJumping. Not an EE plan input.
#include "jump_gate.cpp"

#include <cstdio>

extern "C" int jumping = 0;

static int failures = 0;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    jumping = 0;
    expect(jumpGateJumping__Fv() == 0, "zero word");
    jumping = 1;
    expect(jumpGateJumping__Fv() == 1, "one");
    jumping = -5;
    expect(jumpGateJumping__Fv() == -5, "negative word");
    jumping = 0x7fffffff;
    expect(jumpGateJumping__Fv() == 0x7fffffff, "high positive word");
    if (failures != 0) return 1;
    std::puts("jumpGateJumping contract passed");
    return 0;
}
