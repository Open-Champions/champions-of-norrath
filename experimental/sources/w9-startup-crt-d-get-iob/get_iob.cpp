/* Observed get_iob at 0x002d8a80, 108 bytes (a local routine in the image).
 * It calls _sceFsIobSemaMK (0x002d8998), then WaitSema on _fs_iob_semid
 * (0x00444a00). An index of 32 or more (unsigned) signals the semaphore and
 * returns 0. Otherwise it signals and returns _iob (0x008e19c0) plus the index
 * times 16. The semaphore id is re-read for the signal. The slot contents are
 * not examined, so a slot that new_iob never handed out is returned as well.
 * Unknown: what _sceFsIobSemaMK does beyond its name.
 */
extern "C" {
extern int _fs_iob_semid;
extern unsigned char _iob[];
void _sceFsIobSemaMK();
int WaitSema(int semaphore);
int SignalSema(int semaphore);

unsigned char* get_iob(unsigned index) {
    _sceFsIobSemaMK();
    WaitSema(*static_cast<volatile int*>(&_fs_iob_semid));
    if (index >= 32u) {
        SignalSema(*static_cast<volatile int*>(&_fs_iob_semid));
        return 0;
    }
    unsigned char* const slot = _iob + index * 16u;
    SignalSema(*static_cast<volatile int*>(&_fs_iob_semid));
    return slot;
}
}
