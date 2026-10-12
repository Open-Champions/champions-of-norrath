// Observed hudClearInfoMessage__Fv at 0x001c7688, 8 bytes.
// It stores zero in the halfword named infoMessageTimer at 0x00484b18. The
// message text and the always flag are not touched. The timer object is not
// defined here.
extern "C" unsigned short infoMessageTimer;

extern "C" void hudClearInfoMessage__Fv() {
    infoMessageTimer = 0;
}
