// Host check for msgEquipItem__FiPQ24item11descriptioni. Not an EE plan input.
#include "msg_equip_item.cpp"
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
    invMsgHandle = 9;
    msgEquipItem__FiPQ24item11descriptioni(0x1234, &item, 1);
    if (calls != 1 || seen_handle != 9 || seen_size != 8) return 1;
    if (seen[0] != 1 || seen[1] != 1 || seen[2] != 0x34 || seen[3] != 0x12) return 2;
    // the pointer word at offset 4 is sent as a 32-bit value (the low half of the host address)
    unsigned int word = seen[4] | seen[5] << 8 | seen[6] << 16 | static_cast<unsigned>(seen[7]) << 24;
    if (word != static_cast<unsigned int>(reinterpret_cast<unsigned long long>(&item))) return 3;
    // only low halves/bytes of the integer arguments are sent
    invMsgHandle = -2;
    msgEquipItem__FiPQ24item11descriptioni(0x7fff8001, &item, 0x1ff);
    if (calls != 2 || seen_handle != -2) return 4;
    if (seen[1] != 0xff || seen[2] != 0x01 || seen[3] != 0x80) return 5;
    return 0;
}
