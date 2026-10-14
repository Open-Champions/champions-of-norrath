#include "draw_trap.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
int main() {
    unsigned char object[16];
    std::memset(object, 0x91, sizeof object);
    msg_draw__4Trap(object);
    msg_draw__4Trap(nullptr);
    for (unsigned char byte : object) if (byte != 0x91) { std::fprintf(stderr, "fail: trap draw wrote\n"); return 1; }
    (void)failures;
    return 0;
}
