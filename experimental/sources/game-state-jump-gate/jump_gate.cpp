// Observed jumpGateJumping__Fv at 0x00284830, 8 bytes.
// It returns the word jumping at 0x00486030. That symbol is 4 bytes.
// The load is the delay of the return. Nothing here was executed.
extern "C" int jumping;

extern "C" int jumpGateJumping__Fv() {
    return jumping;
}
