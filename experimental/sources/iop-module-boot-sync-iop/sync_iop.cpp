// Observed sceSifSyncIop at 0x002dd4e0, 80 bytes. It reads sceSifGetReg(4).
// When bit 0x40000 of that word is clear it returns 0. Otherwise it calls
// sceResetttyinit, sceSifExecNotifyHandler(1, 1), sceSifExecNotifyHandler(0, 1),
// and returns 1.
extern "C" int sceSifGetReg(int index);
extern "C" void sceResetttyinit();
extern "C" void sceSifExecNotifyHandler(int index, int value);

extern "C" int sceSifSyncIop() {
    const int reg = sceSifGetReg(4);
    if ((reg & 0x40000) == 0)
        return 0;
    sceResetttyinit();
    sceSifExecNotifyHandler(1, 1);
    sceSifExecNotifyHandler(0, 1);
    return 1;
}
