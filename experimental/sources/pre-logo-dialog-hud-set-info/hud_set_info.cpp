// Observed hudSetInfoMessage__FPUsiic at 0x001c7600, 132 bytes.
// The fourth argument is reduced to its low eight bits. A player other than
// -1 returns when netPlayerIsLocal is zero. A zero low byte returns when the
// interfaceOn byte is nonzero. Otherwise ustrcpy copies the text into
// infoMessage at 0x007d0798, the low half of the duration is stored in
// infoMessageTimer, and the low byte is stored in infoMessageAlways.
// Those four symbols and ustrcpy are not defined here.
extern "C" int netPlayerIsLocal__Fi(int player);
extern "C" signed char interfaceOn;
extern "C" unsigned short infoMessage[];
extern "C" unsigned short infoMessageTimer;
extern "C" signed char infoMessageAlways;
extern "C" void ustrcpy__FPUsT0(unsigned short* dest, unsigned short* src);

extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* text, int duration, int player, int extra) {
    const signed char flag = static_cast<signed char>(extra);
    if (player != -1 && netPlayerIsLocal__Fi(player) == 0) return;
    if (flag == 0 && interfaceOn != 0) return;
    ustrcpy__FPUsT0(infoMessage, text);
    infoMessageTimer = static_cast<unsigned short>(duration);
    infoMessageAlways = flag;
}
