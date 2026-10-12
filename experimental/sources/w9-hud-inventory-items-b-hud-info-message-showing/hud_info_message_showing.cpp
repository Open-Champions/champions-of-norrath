// Observed hudInfoMessageShowing__Fv at 0x001c7690, 16 bytes.
// It returns 1 when the halfword infoMessageTimer (0x00484b18) is nonzero and
// 0 otherwise. The timer is not defined here; w8-hud-text-items-b owns it.
extern "C" unsigned short infoMessageTimer;

extern "C" int hudInfoMessageShowing__Fv() {
    return infoMessageTimer != 0;
}
