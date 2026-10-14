// Observed cmd_sem_init at 0x002e26f0, 184 bytes.
// If _sceCd_ncmd_semid, _sceCd_scmd_semid, and _sceCd_rcmd_semid are all other
// than -1, it returns. Otherwise it creates four semaphores. The first three
// use initial count 1 and maximum 1. The fourth uses initial count 1 and
// maximum 0, and is stored to cb_semid. _sceCd_c_cb_sem is then cleared.
extern "C" {
extern int _sceCd_ncmd_semid;
extern int _sceCd_scmd_semid;
extern int _sceCd_rcmd_semid;
extern int cb_semid;
extern int _sceCd_c_cb_sem;
int CreateSema(void* parameters);
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
void cmd_sem_init() {
    if (_sceCd_ncmd_semid != -1 && _sceCd_scmd_semid != -1 && _sceCd_rcmd_semid != -1) return;
    unsigned char workspace[24];
    store_word(workspace + 20, 0u);
    store_word(workspace + 4, 1u);
    store_word(workspace + 8, 1u);
    _sceCd_ncmd_semid = CreateSema(workspace);
    _sceCd_scmd_semid = CreateSema(workspace);
    _sceCd_rcmd_semid = CreateSema(workspace);
    store_word(workspace + 8, 0u);
    cb_semid = CreateSema(workspace);
    _sceCd_c_cb_sem = 0;
}
}
