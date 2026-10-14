// Observed textureDrawBorder__FiiiiiUiPcii at 0x00172728, 52 bytes.
// It forwards its nine arguments to textureSmoothBorder__FiiiiiUiiii, with the
// first argument reduced by 1, the second by 4, the third raised by 2, the
// fourth raised by 8, and the seventh (the character pointer) replaced by the
// constant 2. The fifth, sixth, eighth and ninth arguments pass through.
extern "C" void textureSmoothBorder__FiiiiiUiiii(int a, int b, int c, int d, int e, unsigned f,
                                                  int g, int h, int i);

extern "C" void textureDrawBorder__FiiiiiUiPcii(int a, int b, int c, int d, int e, unsigned f,
                                                char* g, int h, int i) {
    textureSmoothBorder__FiiiiiUiiii(a - 1, b - 4, c + 2, d + 8, e, f, 2, h, i);
}
