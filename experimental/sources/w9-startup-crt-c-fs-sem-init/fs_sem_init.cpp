// Observed _sceFsSemInit at 0x002d8ea8, 76 bytes, a local routine of the file-system
// RPC library, called by _sceFsWaitS. Also defines _fs_semid, the library-wide
// lock semaphore id (observed: a .data word at 0x004449fc whose image value is -1).
//
// When _fs_semid is not -1 it does nothing. Otherwise it builds a six-word
// semaphore parameter block on the stack, writes word 1 = 1, word 2 = 1 and
// word 5 = 0 (words 0, 3 and 4 are left as the stack held them), calls CreateSema
// with it and stores the result in _fs_semid, whatever it is. Words 1 and 2 are
// the same as in sceOpen's per-call semaphore except that word 2 is 1 here; the
// meaning of the individual parameter words is not established by this unit.
extern "C" {
int _fs_semid = -1;

int CreateSema(void* parameters);

void _sceFsSemInit() {
    if (_fs_semid != -1)
        return;
    int parameters[6];
    parameters[5] = 0;
    parameters[1] = 1;
    parameters[2] = 1;
    _fs_semid = CreateSema(parameters);
}
}
