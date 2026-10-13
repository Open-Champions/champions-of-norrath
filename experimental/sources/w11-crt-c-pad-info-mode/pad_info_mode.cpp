/* Observed scePadInfoMode at 0x002d3fa8, 312 bytes (arguments: port, slot,
 * selector, index). PadInfo (0x008dd8d0) holds 28 bytes per slot and 112 per
 * port; word +16 is zero when the port/slot is not open, in which case the
 * result is 0. Otherwise it reads the 128-byte block from
 * scePadGetDmaStr(port, slot). The result is 0 unless byte 114 is 1 and byte
 * 113 is not 2. Then by selector:
 *   1: byte 101, or 0 when it is 243, otherwise shifted right by 4;
 *   2: 0 when byte 100 is 1, else the unsigned halfword at 80 + 2 * byte 105;
 *   3: 0 when byte 100 is 1, else byte 105;
 *   4: 0 when byte 100 is 1; else index -1 returns byte 104, an index below
 *      byte 104 (signed) returns the unsigned halfword at 80 + 2 * index, any
 *      other index returns 0;
 *   anything else: 0.
 * Inferred: the halfwords from offset 80 form a table of modes whose length
 * is byte 104; neither is established by this routine.
 */
extern "C" {
extern unsigned char norrath_libpad_port_info_view[];

unsigned char* scePadGetDmaStr(int port, int slot);

int scePadInfoMode(int port, int slot, int selector, int index) {
    const unsigned* info = reinterpret_cast<const unsigned*>(norrath_libpad_port_info_view + slot * 28 + port * 112);
    if (info[4] == 0) return 0;
    const unsigned char* block = scePadGetDmaStr(port, slot);
    if (block[114] != 1) return 0;
    if (block[113] == 2) return 0;
    switch (selector) {
    case 1:
        if (block[101] == 243) return 0;
        return block[101] >> 4;
    case 2:
        if (block[100] == 1) return 0;
        return *reinterpret_cast<const unsigned short*>(block + 80 + block[105] * 2);
    case 3:
        if (block[100] == 1) return 0;
        return block[105];
    case 4:
        if (block[100] == 1) return 0;
        if (index == -1) return block[104];
        if (!(index < static_cast<int>(block[104]))) return 0;
        return *reinterpret_cast<const unsigned short*>(block + 80 + index * 2);
    default:
        return 0;
    }
}
}
