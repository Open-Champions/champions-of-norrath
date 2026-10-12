// Host check only; not an EE build input.
extern "C" {
unsigned char norrath_libpad_port_info_view[224];
static int calls, seen_port = -1, seen_slot = -1;
static unsigned seen_mask;
int scePadSetButtonInfo(int port, int slot, unsigned mask) { ++calls; seen_port = port; seen_slot = slot; seen_mask = mask; return 7; }
int scePadEnterPressMode(int port, int slot);
}
int main() {
    unsigned* info = reinterpret_cast<unsigned*>(norrath_libpad_port_info_view);
    if (scePadEnterPressMode(0, 1) != 0 || calls != 0) return 1;
    info[(0 + 1 * 28 + 16) / 4] = 1;
    if (scePadEnterPressMode(0, 1) != 7 || calls != 1) return 2;
    if (seen_port != 0 || seen_slot != 1 || seen_mask != 0xfff) return 3;
    return 0;
}
