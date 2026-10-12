// Observed sceCdPOffCallback at 0x002e2848, 128 bytes. When _icmd_bind
// (0x004460c0) is negative it first calls PowerOffCB. It then disables
// interrupts with DIntr, reads the previous word at sceCdPoffCbfunc
// (0x008e8148), stores the new function there, the new data argument at
// sceCdPoffCbdata (0x008e8150) and the caller's global-pointer register at
// sceCdCbPoff_Gp (0x008e814c), and calls EIntr only when DIntr returned
// nonzero. The previous function word is returned.
extern "C" {
extern int _icmd_bind;
unsigned sceCdPoffCbfunc;
unsigned sceCdCbPoff_Gp;
unsigned sceCdPoffCbdata;
void PowerOffCB();
int DIntr();
int EIntr();
#if defined(NORRATH_HOST_CONTRACT)
extern unsigned norrath_host_gp;
#endif
unsigned sceCdPOffCallback(unsigned callback, unsigned data) {
#if defined(NORRATH_HOST_CONTRACT)
    const unsigned gp = norrath_host_gp;
#else
    register unsigned gp asm("$28");
#endif
    if (_icmd_bind < 0) PowerOffCB();
    const int was_enabled = DIntr();
    const unsigned previous = sceCdPoffCbfunc;
    sceCdPoffCbdata = data;
    sceCdPoffCbfunc = callback;
    sceCdCbPoff_Gp = gp;
    if (was_enabled != 0) EIntr();
    return previous;
}
}
