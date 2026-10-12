// Observed _sceFsWaitS at 0x002d8ef8, 48 bytes, a local routine of the file-system
// RPC library. sceOpen calls it with 0 before touching any shared state; the
// argument is not read. It calls _sceFsSemInit (which creates the library lock
// semaphore the first time), then WaitSema on _fs_semid, and returns 0 whatever
// WaitSema returned. Its partner _sceFsSigSema releases the same semaphore. The
// word named flag by the sceOpen unit is unused here; the reason callers pass it
// is unknown.
extern "C" {
extern int _fs_semid;

void _sceFsSemInit();
int WaitSema(int semaphore);

int _sceFsWaitS(int) {
    _sceFsSemInit();
    WaitSema(_fs_semid);
    return 0;
}
}
