#include <cstring>
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
extern "C" {
int levelFlags;
unsigned char pad[4 * 88];
unsigned char player[4 * 23984];
static int rotCalls, rotA, rotB, rotOut;
int fastestRotDirection__FssPi(short a, short b, int* out) { ++rotCalls; rotA = a; rotB = b; rotOut = out != nullptr; return -1; }
void checkYawReset__6Camera(unsigned char* camera);
}
int main() {
    unsigned char* cam = arena_at(0x100);
    unsigned char* target = arena_at(0x1000);
    auto arm = [&] {
        levelFlags = 0; rotCalls = 0;
        std::memset(cam, 0, 512); std::memset(pad, 0, sizeof pad); std::memset(player, 0, sizeof player);
        sw_(cam + 64, 2); sh_(cam + 104, (unsigned short)-300);
        sw_(pad + 2 * 88 + 12, 0x200); sf_(pad + 2 * 88 + 20, 0.1f); sf_(pad + 2 * 88 + 24, -0.1f);
        sw_(player + 2 * 23984 + 21872, norrath_host_address(target));
        sh_(target + 56, 1000);
    };
    arm();
    checkYawReset__6Camera(cam);
    if (rotCalls != 1 || hw(cam + 138) != 15384 || rotA != -300 || rotB != 15384 || rotOut != 0 || hw(cam + 56) != 0xffffffffu) return 1;
    // No player target: -8192.
    arm(); sw_(player + 2 * 23984 + 21872, 0);
    checkYawReset__6Camera(cam);
    if (rotCalls != 1 || (short)(cam[138] | cam[139] << 8) != -8192) return 2;
    // Each guard stops it with no store.
    arm(); levelFlags = 0x2000000; checkYawReset__6Camera(cam); if (rotCalls) return 3;
    arm(); cam[352] = 0x80; checkYawReset__6Camera(cam); if (rotCalls) return 4;
    arm(); sw_(cam + 100, 1); checkYawReset__6Camera(cam); if (rotCalls) return 5;
    arm(); sw_(cam + 56, 1); checkYawReset__6Camera(cam); if (rotCalls || hw(cam + 56) != 1) return 6;
    arm(); sw_(pad + 2 * 88 + 12, 0x1ff); checkYawReset__6Camera(cam); if (rotCalls) return 7;
    arm(); sf_(pad + 2 * 88 + 20, 0.2f); checkYawReset__6Camera(cam); if (rotCalls) return 8;
    arm(); sf_(pad + 2 * 88 + 20, 0.17f); sf_(pad + 2 * 88 + 24, 0.0f); checkYawReset__6Camera(cam); if (rotCalls != 1) return 9;
    if (hw(cam + 138) != 15384) return 10;
    return 0;
}
