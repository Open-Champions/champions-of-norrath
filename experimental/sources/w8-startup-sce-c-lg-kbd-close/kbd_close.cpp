// Observed lgKbdClose at 0x002e73f8, 152 bytes. The handle minus one is the
// slot index. An index above the signed word at offset 0x40 of GLibData
// (0x008ebdd8), or below zero, returns 0x80000004 without touching anything.
// Otherwise WaitSema runs on the semaphore word at offset 0x2c, the word at
// offset 0x58 is decremented, the word at offset 0x1c of the slot (slots are
// 272 bytes each starting at offset 0x88) becomes 1 if it was 4, SignalSema
// runs on the same semaphore, and 0 is returned. The meaning of the slot
// states and the other GLibData fields is unknown.
extern "C" {
extern unsigned char GLibData[];
int norrath_player_wait_semaphore(int semaphore);
int norrath_player_signal_semaphore(int semaphore);
static int load_int(const unsigned char* slot) {
    return static_cast<int>(static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
                            (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24));
}
static void store_int(unsigned char* slot, int value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
unsigned lgKbdClose(int handle) {
    const int index = handle - 1;
    if (load_int(GLibData + 0x40) < index || index < 0) return 0x80000004u;
    norrath_player_wait_semaphore(load_int(GLibData + 0x2c));
    unsigned char* slot = GLibData + 0x88 + index * 272;
    store_int(GLibData + 0x58, load_int(GLibData + 0x58) - 1);
    if (load_int(slot + 0x1c) == 4) store_int(slot + 0x1c, 1);
    norrath_player_signal_semaphore(load_int(GLibData + 0x2c));
    return 0u;
}
}
