// engineReboot at 0x00191ad0, 132 bytes, in SLUS_205.65.
// Observed call order, all with the arguments shown: soundQuiet(); netFree(true);
// DisableIntc(2); RemoveIntcHandler(2, vblankHID); drawRemoveInterrupts(); texTerminateThread();
// scePadEnd(); UnpreparePowerOff(); sceSifExitCmd(); LoadExecPS2(path, 0, 0), where the path is
// the 21-character text cdrom0:\SLUS_205.65;1 at 0x004643e8 (the executable's own file name on
// the disc). If LoadExecPS2 returns, execution falls into a branch to itself and never leaves.
// vblankHID is the word at 0x004843ec. The function takes no arguments and never returns.
// Unknown: whether the loop body is empty in the source (the image has no-ops before the
// branch), and the return values of the called functions, none of which are used.
extern "C" int vblankHID;
extern "C" void soundQuiet__Fv();
extern "C" void netFree__Fb(bool flag);
extern "C" void DisableIntc(int cause);
extern "C" void RemoveIntcHandler(int cause, int handler);
extern "C" void drawRemoveInterrupts__Fv();
extern "C" void texTerminateThread__Fv();
extern "C" void scePadEnd();
extern "C" void UnpreparePowerOff__Fv();
extern "C" void sceSifExitCmd();
extern "C" void LoadExecPS2(const char* path, int argc, char** argv);

extern "C" void engineReboot__Fv() {
    soundQuiet__Fv();
    netFree__Fb(true);
    DisableIntc(2);
    RemoveIntcHandler(2, vblankHID);
    drawRemoveInterrupts__Fv();
    texTerminateThread__Fv();
    scePadEnd();
    UnpreparePowerOff__Fv();
    sceSifExitCmd();
    LoadExecPS2("cdrom0:\\SLUS_205.65;1", 0, 0);
    for (;;) {
    }
}
