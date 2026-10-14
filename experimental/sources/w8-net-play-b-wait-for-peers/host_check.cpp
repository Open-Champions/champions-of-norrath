// Host check for netWaitForPeersToCatchUp. Not an EE plan input.
#include "net_wait_for_peers.cpp"
#include <cstring>
extern "C" {
int engineDisplayDisable;
int nmPartners;
int trueNetFrameNm;
unsigned char partners[70528];
}
static int steps, syncs, order_bad, disable_at_step;
static int arrive_after;  // step count after which peer 1 has caught up (-1 never)
static int count_change_after = -1;
static bool last_was_step;
extern "C" void netGameStep__FbT0(bool a, bool b) {
    if (!a || !b) order_bad = 1;
    disable_at_step = engineDisplayDisable;
    ++steps; last_was_step = true;
    if (steps == arrive_after) *(int*)(partners + 17632 + 17600) = trueNetFrameNm;
    if (steps == count_change_after) nmPartners = 1;
}
extern "C" void engineSyncVBlank__Fv() {
    if (!last_was_step) order_bad = 1;
    last_was_step = false; ++syncs;
}
static void setup() {
    std::memset(partners, 0, sizeof partners);
    for (int i = 0; i < 4; ++i) *(int*)(partners + i * 17632 + 17600) = 50;
    trueNetFrameNm = 50; nmPartners = 3; steps = syncs = order_bad = 0; engineDisplayDisable = 0;
    arrive_after = -1; count_change_after = -1; last_was_step = false;
}
int main() {
    setup();
    netWaitForPeersToCatchUp__Fv();
    if (steps != 0 || engineDisplayDisable != 3) return 1;  // nobody behind
    setup(); *(int*)(partners + 17632 + 17600) = 49; arrive_after = 4;
    netWaitForPeersToCatchUp__Fv();
    if (steps != 4 || syncs != 4 || order_bad || disable_at_step != 3) return 2;
    setup(); *(int*)(partners + 17632 + 17600) = 49;  // never arrives
    netWaitForPeersToCatchUp__Fv();
    if (steps != 121 || syncs != 121) return 3;
    setup(); *(int*)(partners + 3 * 17632 + 17600) = 0;  // outside nmPartners
    netWaitForPeersToCatchUp__Fv();
    if (steps != 0) return 4;
    for (int off = 17620; off <= 17621; ++off) {  // flagged peers are ignored, even a negative byte
        setup(); *(int*)(partners + 2 * 17632 + 17600) = 0; partners[2 * 17632 + off] = 0x80;
        netWaitForPeersToCatchUp__Fv();
        if (steps != 0) return 5;
    }
    setup(); partners[2 * 17632 + 17622] = 1; *(int*)(partners + 2 * 17632 + 17600) = 0;  // byte 17622 is not tested
    arrive_after = 2; count_change_after = -1;
    netWaitForPeersToCatchUp__Fv();
    if (steps != 121) return 6;
    setup(); *(int*)(partners + 2 * 17632 + 17600) = 0; count_change_after = 3;  // nmPartners is reloaded each scan
    netWaitForPeersToCatchUp__Fv();
    if (steps != 3) return 7;
    setup(); trueNetFrameNm = 51;  // equal is not behind, below is
    *(int*)(partners + 0 * 17632 + 17600) = 51; *(int*)(partners + 17632 + 17600) = 51; *(int*)(partners + 2 * 17632 + 17600) = 51;
    netWaitForPeersToCatchUp__Fv();
    if (steps != 0) return 8;
    return 0;
}
