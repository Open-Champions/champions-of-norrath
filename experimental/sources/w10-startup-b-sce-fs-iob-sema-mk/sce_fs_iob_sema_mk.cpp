/* Observed _sceFsIobSemaMK at 0x002d8998, 92 bytes (a local routine in the image).
 * When _fs_iob_semid (0x00444a00) holds -1: a six-word block is built on the stack
 * with words 1 and 2 set to 1 and word 5 set to 0 (words 0, 3 and 4 are not
 * written), CreateSema is called with it and its result stored in _fs_iob_semid,
 * then CreateSema is called again with the same block and its result stored in
 * _fs_fsq_semid (0x00444a04). Any other value of _fs_iob_semid does nothing.
 * Nothing is returned. new_iob and sceFsInit call it before waiting on the I/O
 * semaphore.
 * Unknown: what the stack words 0, 3 and 4 hold when CreateSema reads them, and
 * what each of the six fields means (the kernel structure is not authored here).
 * Compiled output is not claimed to match the original.
 */
extern "C" {
extern int _fs_iob_semid;
extern int _fs_fsq_semid;
int CreateSema(void* parameters);

void _sceFsIobSemaMK() {
    if (_fs_iob_semid != -1) return;
    int parameters[6];
    parameters[5] = 0;
    parameters[1] = 1;
    parameters[2] = 1;
    _fs_iob_semid = CreateSema(parameters);
    _fs_fsq_semid = CreateSema(parameters);
}
}
