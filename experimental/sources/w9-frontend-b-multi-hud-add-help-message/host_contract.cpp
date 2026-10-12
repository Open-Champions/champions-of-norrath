#include "multi_hud_add_help_message.cpp"

#include <cstdio>

unsigned char multiHelpMessageOn[4];
int multiSide[4];
unsigned short* multiTitle[4];
unsigned short* multiBody[4];
unsigned char shopList[9488 + 4];
static int local_mask, order[4], n_order, flared;
extern "C" int netPlayerIsLocal__Fi(int p) { order[n_order++] = 1; return (local_mask >> p) & 1; }
extern "C" void coreInterfaceFlare__Fi(int p) { order[n_order++] = 2; flared = p; }

int main() {
    unsigned short t[2] = {1, 0}, b[2] = {2, 0};
    for (auto& x : shopList) x = 0x55;
    local_mask = 0;
    multiHudAddHelpMessage__FiPUsT1i(1, t, b, 9);
    if (n_order != 1 || multiHelpMessageOn[1] || multiSide[1] || multiTitle[1] || multiBody[1]) return 1;
    if (shopList[9489] != 0x55) return 2;
    local_mask = 2;
    multiHudAddHelpMessage__FiPUsT1i(1, t, b, 9);
    if (n_order != 3 || order[1] != 1 || order[2] != 2 || flared != 1) return 3;
    if (multiHelpMessageOn[1] != 1 || multiSide[1] != 9 || multiTitle[1] != t || multiBody[1] != b) return 4;
    if (shopList[9489] != 0 || shopList[9488] != 0x55 || shopList[9490] != 0x55) return 5;
    if (multiHelpMessageOn[0] || multiTitle[0] || multiTitle[2]) return 6;
    std::puts("multi hud add help message passed");
    return 0;
}
