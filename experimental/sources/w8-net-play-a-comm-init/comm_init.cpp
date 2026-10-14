// Observed commInit__Fv at 0x002cd168, 64 bytes. When comm_handle is not -1 it
// raises enableMallocWrapper, calls lgAudClose(comm_handle), then resets
// comm_handle to -1 and clears enableMallocWrapper. When comm_handle is already
// -1 nothing happens. No result is produced.
extern "C" {
extern int comm_handle;
extern unsigned char enableMallocWrapper;
void lgAudClose(int handle);

void commInit__Fv() {
    int handle = comm_handle;
    if (handle != -1) {
        enableMallocWrapper = 1;
        lgAudClose(handle);
        comm_handle = -1;
        enableMallocWrapper = 0;
    }
}
}
