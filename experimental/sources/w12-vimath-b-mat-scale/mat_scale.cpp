// Observed: matScale__FP8Matrix34fff at 0x00159770 is 112 bytes. Arguments: m, sx (f12), sy (f13),
// sz (f14). Floats 0 to 2 are multiplied by sx, floats 3 to 5 by sy and floats 6 to 8 by sz, in
// place. Floats 9 to 11 are not touched. Nothing is returned.
// Inferred: scales the three rows of the rotation part of a twelve-float matrix.
// Unknown: the Matrix34 type definition and whether the rows are axes or basis vectors.
extern "C" {
void matScale__FP8Matrix34fff(float* m, float sx, float sy, float sz) {
    m[0] = m[0] * sx;
    m[1] = m[1] * sx;
    m[2] = m[2] * sx;
    m[3] = m[3] * sy;
    m[4] = m[4] * sy;
    m[5] = m[5] * sy;
    m[6] = m[6] * sz;
    m[7] = m[7] * sz;
    m[8] = m[8] * sz;
}
}
