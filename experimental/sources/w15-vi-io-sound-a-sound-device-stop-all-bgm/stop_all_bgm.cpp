// Observed: StopAllBgm__13VISoundDevice at 0x01126560, 72 bytes. With the device as this: stores a zero byte at +3436 and a zero
// byte at +3692 (this one is stored whichever way the test below goes). Reads the signed byte at +3948; if it is nonzero it stores
// zero in the first word of sIopFunctionData (0x011edf08), calls SetIopFunction__FUii(14, 1) and stores a zero byte at +3948.
// Inferred: three per-device bgm flags are cleared, and the IOP is told to stop the stream (function 14) only when the third flag
// was set. The first word of sIopFunctionData is the argument slot of that call.
// Unknown: the member names, and the IOP function's contract.
extern "C" {
void SetIopFunction__FUii(unsigned function, int count);
extern int norrath_overlay_sIopFunctionData_11edf08[];

void StopAllBgm__13VISoundDevice(unsigned char* device) {
    device[3436] = 0;
    signed char active = static_cast<signed char>(device[3948]);
    device[3692] = 0;
    if (active != 0) {
        norrath_overlay_sIopFunctionData_11edf08[0] = 0;
        SetIopFunction__FUii(14, 1);
        device[3948] = 0;
    }
}
}
