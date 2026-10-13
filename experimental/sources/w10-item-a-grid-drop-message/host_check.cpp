// Host check for msgInventoryGridDropItem__Fiiciiii: one send, handle, length and
// the 13 message bytes in their observed positions.
#include <cstring>
extern "C" int invMsgHandle = 0x2468;
static int calls, sentHandle, sentLength;
static unsigned char sent[13];
extern "C" void netSendMessage__FiPUci(int h, unsigned char* m, int n) {
    ++calls; sentHandle = h; sentLength = n; std::memcpy(sent, m, 13);
}
#include "grid_drop_message.cpp"
int main() {
    msgInventoryGridDropItem__Fiiciiii(0x1a5, 0x11223344, (char)0x9c, 0x7abcd, 0x65432, 0x1f7, -2);
    if (calls != 1 || sentHandle != 0x2468 || sentLength != 13) return 1;
    unsigned char expect[13] = {0, 0xa5, 0xcd, 0xab, 0x44, 0x33, 0x22, 0x11, 0x32, 0x54, 0x9c, 0xf7, 0xfe};
    if (std::memcmp(sent, expect, 13) != 0) return 2;
    return 0;
}
