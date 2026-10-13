// Host check for msgInvItemSetAttributes__FiiPcPQ24item11description. Not an EE plan input.
// Pointer words are sent as 32-bit values; on the 64-bit host they are the low halves of the addresses.
#include "msg_inv_item_set_attributes.cpp"
extern "C" int invMsgHandle = 0;
static int calls, seen_handle, seen_size;
static unsigned char seen[16];
extern "C" void netSendMessage__FiPUci(int handle, unsigned char* bytes, int size) {
    ++calls;
    seen_handle = handle;
    seen_size = size;
    for (int i = 0; i < 16; ++i) seen[i] = bytes[i];
}
static unsigned int word(int at) {
    return seen[at] | seen[at + 1] << 8 | seen[at + 2] << 16 | static_cast<unsigned>(seen[at + 3]) << 24;
}
int main() {
    char text[4] = {};
    int item = 0;
    invMsgHandle = 31;
    msgInvItemSetAttributes__FiiPcPQ24item11description(0x1a3, 0x11223344, text, &item);
    if (calls != 1 || seen_handle != 31 || seen_size != 16) return 1;
    if (seen[0] != 8 || seen[1] != 0xa3 || seen[2] != 0 || seen[3] != 0) return 2;
    if (seen[4] != 0x44 || seen[5] != 0x33 || seen[6] != 0x22 || seen[7] != 0x11) return 3;
    if (word(8) != static_cast<unsigned int>(reinterpret_cast<unsigned long>(text))) return 4;
    if (word(12) != static_cast<unsigned int>(reinterpret_cast<unsigned long>(&item))) return 5;
    msgInvItemSetAttributes__FiiPcPQ24item11description(-1, -2, 0, 0);
    if (calls != 2 || seen[1] != 0xff || word(4) != 0xfffffffe || word(8) || word(12)) return 6;
    return 0;
}
