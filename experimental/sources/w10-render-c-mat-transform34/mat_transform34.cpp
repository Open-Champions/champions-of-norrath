// Observed matTransform__FP6Point3P8Matrix34T0 at 0x00159ae0, 184 bytes (compiled
// size may differ). Arguments: destination Point3, Matrix34, source Point3.
// The three input floats are copied to a local first, so the destination may alias
// the source. With the matrix as twelve floats m[0..11] the result is
//   out.x = ((m[0]*x + m[3]*y) + m[6]*z) + m[9]
//   out.y = ((m[1]*x + m[4]*y) + m[7]*z) + m[10]
//   out.z = ((m[2]*x + m[5]*y) + m[8]*z) + m[11]
// i.e. the nine-float part applied as in the Matrix33 variant, then the translation
// row added last. Nothing is returned.
// Inferred: Matrix34 is twelve contiguous floats with the translation last.
// Unknown: the engine's row/column convention behind the name.
struct Point3 {
    float x, y, z;
};
struct Matrix34 {
    float m[12];
};

extern "C" void matTransform__FP6Point3P8Matrix34T0(Point3* out, const Matrix34* matrix, const Point3* in) {
    const Point3 p = *in;
    const float* m = matrix->m;
    out->x = m[0] * p.x + m[3] * p.y + m[6] * p.z + m[9];
    out->y = m[1] * p.x + m[4] * p.y + m[7] * p.z + m[10];
    out->z = m[2] * p.x + m[5] * p.y + m[8] * p.z + m[11];
}
