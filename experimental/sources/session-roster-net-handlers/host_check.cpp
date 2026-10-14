#include "net_handlers.cpp"
int main() {
    if (sizeof commSendmask != 4 || commSendmask != 0) return 1;
    if (sizeof msgHandler != 64) return 2;
    if (sizeof pktHandler != 16) return 3;
    for (unsigned i = 0; i < 16; ++i) if (msgHandler[i]) return 4;
    for (unsigned i = 0; i < 4; ++i) if (pktHandler[i]) return 5;
    // The image places pktHandler right after msgHandler: 0x4f3f10 - 0x4f3ed0.
    if (0x4f3f10u - 0x4f3ed0u != sizeof msgHandler) return 6;
    return 0;
}
