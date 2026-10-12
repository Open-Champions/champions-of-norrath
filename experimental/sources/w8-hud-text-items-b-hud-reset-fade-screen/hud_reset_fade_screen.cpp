// Observed hudResetFadeScreen__Fi at 0x001c7da0, 24 bytes.
// A zero argument stores zero in the word named fadeDownVal (0x00484afc). Any
// other argument stores 128. fadeDownVal is not defined here. The register that
// holds 128 afterwards is incidental; the routine returns nothing.
extern "C" int fadeDownVal;

extern "C" void hudResetFadeScreen__Fi(int fade_to_black) {
    if (fade_to_black == 0) {
        fadeDownVal = 0;
        return;
    }
    fadeDownVal = 128;
}
