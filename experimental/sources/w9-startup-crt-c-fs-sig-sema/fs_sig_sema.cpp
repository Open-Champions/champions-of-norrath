// Observed _sceFsSigSema at 0x002d8f28, 12 bytes, a local routine of the file-system
// RPC library. It is a tail call: SignalSema(_fs_semid), so the caller receives
// SignalSema's result unchanged. sceOpen ignores that result. It is the release
// that matches _sceFsWaitS.
extern "C" {
extern int _fs_semid;

int SignalSema(int semaphore);

int _sceFsSigSema() {
    return SignalSema(_fs_semid);
}
}
