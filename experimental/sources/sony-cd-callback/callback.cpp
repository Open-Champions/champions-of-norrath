// Observed _sceCd_cd_callback at 0x002e23f8, 160 bytes.
// The first word of the argument is stored to sceCdCbfunc_num and copied to
// sceCdCbfunc_number. Value 11 clears sceCdCbfunc_num and _sceCd_c_cb_sem.
// Any other value signals _sceCd_ncmd_semid through iSignalSema. If cb_thid and
// sceCdCbfunc are both nonzero it also signals cb_semid; otherwise it clears
// _sceCd_c_cb_sem. sceCdCbfunc_num is cleared on that path as well.
extern "C" {
extern int sceCdCbfunc_num;
extern int sceCdCbfunc_number;
extern int _sceCd_c_cb_sem;
extern int _sceCd_ncmd_semid;
extern int cb_thid;
extern int cb_semid;
extern unsigned sceCdCbfunc;
int iSignalSema(int semaphore);
static unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}
void _sceCd_cd_callback(void* argument) {
    const unsigned value = load_word(static_cast<const unsigned char*>(argument));
    sceCdCbfunc_num = static_cast<int>(value);
    sceCdCbfunc_number = sceCdCbfunc_num;
    if (sceCdCbfunc_num == 11) {
        sceCdCbfunc_num = 0;
        _sceCd_c_cb_sem = 0;
        return;
    }
    iSignalSema(_sceCd_ncmd_semid);
    if (cb_thid != 0 && sceCdCbfunc != 0u) iSignalSema(cb_semid);
    else _sceCd_c_cb_sem = 0;
    sceCdCbfunc_num = 0;
}
}
