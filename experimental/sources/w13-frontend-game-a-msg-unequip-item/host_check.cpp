// Host check for msgUnequipItem__FiPQ24item11description. Not an EE plan input.
#include "msg_unequip_item.cpp"
extern "C" int invMsgHandle = 0;
static int calls, seen_handle, seen_size;
static unsigned char seen[8];
extern "C" void netSendMessage__FiPUci(int handle, unsigned char* bytes, int size) {
    ++calls;
    seen_handle = handle;
    seen_size = size;
    for (int i = 0; i < 8; ++i) seen[i] = bytes[i];
}
int main() {
    int item = 0;
    invMsgHandle = 4;
    msgUnequipItem__FiPQ24item11description(0x0102, &item);
    if (calls != 1 || seen_handle != 4 || seen_size != 8) return 1;
    if (seen[0] != 2 || seen[1] != 0 || seen[2] != 0x02 || seen[3] != 0x01) return 2;
    unsigned int word = seen[4] | seen[5] << 8 | seen[6] << 16 | static_cast<unsigned>(seen[7]) << 24;
    if (word != static_cast<unsigned int>(reinterpret_cast<unsigned long long>(&item))) return 3;
    msgUnequipItem__FiPQ24item11description(-1, 0);
    if (calls != 2 || seen[2] != 0xff || seen[3] != 0xff || seen[4] || seen[5] || seen[6] || seen[7]) return 4;
    return 0;
}
