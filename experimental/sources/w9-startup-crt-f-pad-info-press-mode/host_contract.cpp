// Host check only; not an EE build input.
extern "C" {
unsigned char norrath_libpad_port_info_view[224];
static int calls, seen_port = -1, seen_slot = -1;
static unsigned mask_result;
unsigned scePadGetButtonMask(int port, int slot) { ++calls; seen_port = port; seen_slot = slot; return mask_result; }
int scePadInfoPressMode(int port, int slot);
}
int main() {
    unsigned* info = reinterpret_cast<unsigned*>(norrath_libpad_port_info_view);
    mask_result = 0x3ffff;
    if (scePadInfoPressMode(1, 2) != 0 || calls != 0) return 1;           // closed: no call
    info[(112 + 2 * 28 + 16) / 4] = 1;                                    // port 1 slot 2 open
    if (scePadInfoPressMode(1, 2) != 1 || calls != 1 || seen_port != 1 || seen_slot != 2) return 2;
    mask_result = 0x1ffff;
    if (scePadInfoPressMode(1, 2) != 0) return 3;
    mask_result = 0x7ffff;
    if (scePadInfoPressMode(1, 2) != 0) return 4;
    if (scePadInfoPressMode(0, 2) != 0) return 5;                         // other port still closed
    return 0;
}
