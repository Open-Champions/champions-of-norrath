// Host check for msgInventoryExit__Fiii. Not an EE plan input.
#include "msg_inventory_exit.cpp"
// invMsgHandle is declared extern by the source; define it here.
extern "C" int invMsgHandle = 0;
static int calls, seen_handle, seen_size;
static unsigned char seen[4];
extern "C" void netSendMessage__FiPUci(int handle, unsigned char* bytes, int size) {
    ++calls;
    seen_handle = handle;
    seen_size = size;
    for (int i = 0; i < 4; ++i) seen[i] = bytes[i];
}
int main() {
    invMsgHandle = 77;
    msgInventoryExit__Fiii(1, 2, 3);
    if (calls != 1 || seen_handle != 77 || seen_size != 4) return 1;
    if (seen[0] != 4 || seen[1] != 1 || seen[2] != 2 || seen[3] != 3) return 2;
    // only the low byte of each argument is sent
    invMsgHandle = -5;
    msgInventoryExit__Fiii(0x1a5, -2, 0x7f00);
    if (calls != 2 || seen_handle != -5) return 3;
    if (seen[1] != 0xa5 || seen[2] != 0xfe || seen[3] != 0x00) return 4;
    return 0;
}
