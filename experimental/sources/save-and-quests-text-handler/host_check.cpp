#include <cstdio>
#include <cstring>
#include "text_handler.cpp"
NorrathTextSlot inMsg[8];
int msgTimeout[8];
unsigned char outMsg[128];
int waiting;
static void reset() {
    std::memset(inMsg, 0, sizeof inMsg);
    std::memset(outMsg, 0, sizeof outMsg);
    for (int i = 0; i < 8; ++i) msgTimeout[i] = 100 + i;
    waiting = 5;
}
int main() {
    reset();
    for (int s = 0; s < 8; ++s) std::memset(inMsg[s].bytes, 0x10 + s, 126);
    unsigned char m[10] = {2, 7, 9, 'a', 'b', 'c', 0, 0, 0, 0};
    outMsg[0] = 3;  // different from the message: no reset
    outMsg[3] = 'z';
    textHandler__FPUci(m, 6);
    // slots shifted by one, the last one dropped, bytes beyond size untouched
    for (int s = 1; s < 8; ++s) if (inMsg[s].bytes[0] != 0x10 + s - 1 || inMsg[s].bytes[125] != 0x10 + s - 1) return 1;
    if (std::memcmp(inMsg[0].bytes, m, 6) != 0 || inMsg[0].bytes[6] != 0x10 || inMsg[0].bytes[125] != 0x10) return 2;
    if (msgTimeout[0] != 0) return 3;
    for (int s = 1; s < 8; ++s) if (msgTimeout[s] != 100 + s - 1) return 4;
    if (waiting != 5 || outMsg[0] != 3 || outMsg[3] != 'z') return 5;
    // an identical outgoing buffer is reset
    outMsg[0] = 2; outMsg[1] = 7; outMsg[2] = 9;
    std::memcpy(outMsg + 3, "abc", 3);
    textHandler__FPUci(m, 6);
    if (outMsg[0] != 1 || outMsg[1] != 0 || outMsg[3] != 32 || outMsg[4] != 0 || waiting != 0) return 6;
    // a difference in the compared span (byte 0 + 1 = 3 bytes) keeps it
    reset();
    outMsg[0] = 2; outMsg[1] = 7; outMsg[2] = 9;
    std::memcpy(outMsg + 3, "abd", 3);
    textHandler__FPUci(m, 6);
    if (outMsg[0] != 2 || waiting != 5) return 7;
    // a difference beyond the span is ignored
    std::memcpy(outMsg + 3, "abc", 3);
    outMsg[6] = 99;
    textHandler__FPUci(m, 6);
    if (outMsg[0] != 1 || waiting != 0) return 8;
    // byte 1 and byte 2 differences each keep it
    reset(); outMsg[0] = 2; outMsg[1] = 8; outMsg[2] = 9; std::memcpy(outMsg + 3, "abc", 3);
    textHandler__FPUci(m, 6);
    if (outMsg[0] != 2) return 9;
    reset(); outMsg[0] = 2; outMsg[1] = 7; outMsg[2] = 8; std::memcpy(outMsg + 3, "abc", 3);
    textHandler__FPUci(m, 6);
    if (outMsg[0] != 2) return 10;
    std::puts("ok");
    return 0;
}
