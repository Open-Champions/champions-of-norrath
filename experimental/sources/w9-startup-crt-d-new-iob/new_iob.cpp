/* Observed new_iob at 0x002d89f8, 136 bytes (a local routine in the image).
 * It calls _sceFsIobSemaMK (0x002d8998), then WaitSema on _fs_iob_semid
 * (0x00444a00, read after the first call). It scans the thirty-two 16-byte
 * slots of _iob (0x008e19c0) from the first, stopping at the first slot whose
 * word at offset 4 is zero. That slot gets 0x10000000 stored at offset 4, the
 * semaphore is signalled (the store is made before the signal call), and the
 * slot address is returned. With no free slot the semaphore is signalled and 0
 * is returned. The semaphore id is re-read for the signal.
 * Unknown: what _sceFsIobSemaMK does beyond its name and what 0x10000000 means
 * to the library; neither is authored here. Other words of a slot are untouched.
 */
extern "C" {
extern int _fs_iob_semid;
extern unsigned char _iob[];
void _sceFsIobSemaMK();
int WaitSema(int semaphore);
int SignalSema(int semaphore);

static unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}

unsigned char* new_iob() {
    _sceFsIobSemaMK();
    WaitSema(*static_cast<volatile int*>(&_fs_iob_semid));
    unsigned char* slot = _iob;
    unsigned char* const end = _iob + 512;
    while (slot < end) {
        if (load_word(slot + 4) == 0u) {
            slot[4] = 0x00;
            slot[5] = 0x00;
            slot[6] = 0x00;
            slot[7] = 0x10;
            SignalSema(*static_cast<volatile int*>(&_fs_iob_semid));
            return slot;
        }
        slot += 16;
    }
    SignalSema(*static_cast<volatile int*>(&_fs_iob_semid));
    return 0;
}
}
