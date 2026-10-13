#include "hud_words.cpp"
#include <cstddef>
#include <cstdio>

// Sizes are the gaps between neighbouring symbols in the pinned image, written out here.
int main() {
    int bad = 0;
    // multiBody holds 4-byte pointers on the EE, so 16 bytes means four slots on any host.
    bad |= sizeof(levelUpFade) != 16 || sizeof(multiBody) / sizeof(multiBody[0]) != 4;
    bad |= sizeof(multiHelpMessageOn) != 4 || sizeof(shipOn) != 4 || sizeof(shopPromptOK) != 2;
    for (int i = 0; i < 4; ++i) bad |= levelUpFade[i] != 0 || multiBody[i] != 0 || multiHelpMessageOn[i] != 0;
    bad |= shipOn != 0 || shopPromptOK[0] != 0 || shopPromptOK[1] != 0;
    std::printf(bad ? "FAIL\n" : "ok: sizes and zero start\n");
    return bad;
}
