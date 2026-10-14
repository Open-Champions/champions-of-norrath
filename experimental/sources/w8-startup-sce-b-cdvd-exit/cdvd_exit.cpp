// Observed cdvd_exit at 0x002e27a8, 160 bytes, local in the original and published
// global because sceCdInit (mode 5) calls it. When cb_thid is nonzero it stores -1
// in sceCdCbfunc_num and signals cb_semid. It then deletes the N-command,
// S-command and R-command semaphores and cb_semid, in that order. With interrupts
// disabled it removes the SIF command handler 0x80000012, and re-enables
// interrupts only when DIntr reported that they had been enabled.
extern "C" {
extern int cb_thid;
extern int cb_semid;
extern int sceCdCbfunc_num;
extern int _sceCd_ncmd_semid;
extern int _sceCd_scmd_semid;
extern int _sceCd_rcmd_semid;

int SignalSema(int semaphore);
int DeleteSema(int semaphore);
int DIntr();
int EIntr();
void sceSifRemoveCmdHandler(unsigned command);

void cdvd_exit() {
    if (cb_thid != 0) {
        sceCdCbfunc_num = -1;
        SignalSema(cb_semid);
    }
    DeleteSema(_sceCd_ncmd_semid);
    DeleteSema(_sceCd_scmd_semid);
    DeleteSema(_sceCd_rcmd_semid);
    DeleteSema(cb_semid);
    const int was_enabled = DIntr();
    sceSifRemoveCmdHandler(0x80000012u);
    if (was_enabled) EIntr();
}
}
