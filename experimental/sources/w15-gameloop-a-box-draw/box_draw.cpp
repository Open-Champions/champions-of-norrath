// Observed boxDraw__FiiiiiUii at 0x0016ee60, 376 bytes, 96-byte frame. Arguments are, in order, x0, y0, x1, y1 (four
// ints), a fifth int, an unsigned colour and a flag. It draws four one-pixel-wide edges of the box through
// rectangleDraw__FiiiiiUi(left, top, right, bottom, fifth, colour), always passing the fifth int and the colour on.
// Flag zero: top (x0, y0, x1, y0+1), bottom (x0, y1-1, x1, y1), left (x0, y0, x0+1, y1), right (x1-1, y0, x1, y1);
// the four rectangles overlap at the corners.
// Flag nonzero: the same box with its four corner pixels left out: top (x0+1, y0, x1-1, y0+1), bottom (x0+1, y1-1,
// x1-1, y1), left (x0, y0+1, x0+1, y1-1), right (x1-1, y0+1, x1, y1-1). The last call is a tail call.
// Inferred: the flag asks for a box with cut corners; the rectangle arguments are corner pairs (right and bottom not
// included in the size, as the +1 and -1 offsets suggest).
// Unknown: what the fifth int is (it is only forwarded), and the exact rectangleDraw semantics.
extern "C" {
void rectangleDraw__FiiiiiUi(int x0, int y0, int x1, int y1, int fifth, unsigned color);

void boxDraw__FiiiiiUii(int x0, int y0, int x1, int y1, int fifth, unsigned color, int flag) {
    if (flag) {
        rectangleDraw__FiiiiiUi(x0 + 1, y0, x1 - 1, y0 + 1, fifth, color);
        rectangleDraw__FiiiiiUi(x0 + 1, y1 - 1, x1 - 1, y1, fifth, color);
        rectangleDraw__FiiiiiUi(x0, y0 + 1, x0 + 1, y1 - 1, fifth, color);
        rectangleDraw__FiiiiiUi(x1 - 1, y0 + 1, x1, y1 - 1, fifth, color);
    } else {
        rectangleDraw__FiiiiiUi(x0, y0, x1, y0 + 1, fifth, color);
        rectangleDraw__FiiiiiUi(x0, y1 - 1, x1, y1, fifth, color);
        rectangleDraw__FiiiiiUi(x0, y0, x0 + 1, y1, fifth, color);
        rectangleDraw__FiiiiiUi(x1 - 1, y0, x1, y1, fifth, color);
    }
}
}
