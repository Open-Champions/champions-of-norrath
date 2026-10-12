/* Observed PowerOffCB at 0x002e2920, 120 bytes; it is local in the original
 * and published global here because sceCdInit and sceCdPOffCallback call it.
 * It stores 1 in Init_seq (0x004460a4) (the store is in the delay slot of
 * DIntr, so it lands first), calls DIntr, registers _sceCd_Poff_Intr
 * (0x002e28c8) as the handler for command 0x80000012 through
 * sceSifAddCmdHandler with argument 0, and runs EIntr when DIntr reported
 * enabled interrupts. It then stores 0 in Init_seq, 1 in _icmd_bind
 * (0x004460c0), and returns 1. sceSifAddCmdHandler's result is dropped.
 * Compiled output is not claimed to match the original.
 */
extern "C" {
extern int Init_seq;
extern int _icmd_bind;
int DIntr();
int EIntr();
void _sceCd_Poff_Intr(void* data, void* argument);
int sceSifAddCmdHandler(unsigned command, void (*handler)(void*, void*), void* argument);
}

extern "C" int PowerOffCB() {
    Init_seq = 1;
    const int enabled = DIntr();
    sceSifAddCmdHandler(0x80000012u, _sceCd_Poff_Intr, nullptr);
    if (enabled != 0) EIntr();
    Init_seq = 0;
    _icmd_bind = 1;
    return 1;
}
