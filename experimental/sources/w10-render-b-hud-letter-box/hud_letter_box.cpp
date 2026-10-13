// Observed hudLetterBox__Fv at 0x001c7d48, extent 84. No arguments, no result.
// Two calls to rectangleDraw__FiiiiiUi(x, y, third, fourth, list, color), both with
// x = 0, third = 640, list = 7 and color = 0x80000000:
//   1. y = 0, fourth = 58
//   2. y = 390, fourth = engineScreenSlide * 2 + 448 (engineScreenSlide is read
//      after the first call returns)
// Inferred: two bars at the top and bottom of the 640-wide screen; whether the
// last two numeric arguments are widths or heights depends on rectangleDraw, which
// another unit defines, and the fourth number is not proven to be a height.
extern "C" {
extern int engineScreenSlide;
void rectangleDraw__FiiiiiUi(int x, int y, int third, int fourth, int list, unsigned color);

void hudLetterBox__Fv() {
    rectangleDraw__FiiiiiUi(0, 0, 640, 58, 7, 0x80000000u);
    rectangleDraw__FiiiiiUi(0, 390, 640, engineScreenSlide * 2 + 448, 7, 0x80000000u);
}
}
