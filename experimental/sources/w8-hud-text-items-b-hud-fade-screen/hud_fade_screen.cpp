// Observed hudFadeScreen__Fi at 0x001c7db8, 92 bytes.
// The delta is added to the word fadeDownVal (0x00484afc). A negative sum is
// stored as zero and a sum above 128 is stored as 128. Then rectangleDraw__FiiiiiUi
// is called with 0, 0, 640, 448, 6 and the new value shifted left 24 bits (the
// value occupies the top byte of the last argument). The stored value is
// returned. What the arguments of rectangleDraw mean is not established here;
// that routine and fadeDownVal are not defined.
extern "C" int fadeDownVal;
extern "C" void rectangleDraw__FiiiiiUi(int a, int b, int c, int d, int e, unsigned f);

extern "C" int hudFadeScreen__Fi(int delta) {
    int value = fadeDownVal + delta;
    if (value < 0) {
        value = 0;
    } else if (value > 128) {
        value = 128;
    }
    fadeDownVal = value;
    rectangleDraw__FiiiiiUi(0, 0, 640, 448, 6, static_cast<unsigned>(value) << 24);
    return fadeDownVal;
}
