#include "hud_net_wait_msg_ready.cpp"

#include <cstdio>

int nmPlayers;
int netWaitMsgHandler = 42;
static int local_mask, queried[8], nq, sent, s_handler, s_size, s_b0, s_b1;
extern "C" int netPlayerIsLocal__Fi(int p) { queried[nq++] = p; return (local_mask >> p) & 1; }
extern "C" int netSendMessage__FiPUci(int h, unsigned char* d, int n) {
    ++sent; s_handler = h; s_size = n; s_b0 = d[0]; s_b1 = d[1];
    return 0;
}

static int run(int players, int mask) {
    nmPlayers = players; local_mask = mask; nq = 0; sent = 0;
    hudNetWaitMsgReady__Fv();
    return s_b1;
}

int main() {
    if (run(4, 0x4) != 2 || nq != 3) return 1;  // stops at first local
    if (sent != 1 || s_handler != 42 || s_size != 2 || s_b0 != 1) return 2;
    if (run(4, 0x0) != 4 || nq != 4) return 3;  // none local: index is the count
    if (run(0, 0x1) != 0 || nq != 0) return 4;  // no players: no query
    if (sent != 1 || s_size != 2) return 5;
    if (run(3, 0x1) != 0 || nq != 1) return 6;
    std::puts("hud net wait msg ready passed");
    return 0;
}
