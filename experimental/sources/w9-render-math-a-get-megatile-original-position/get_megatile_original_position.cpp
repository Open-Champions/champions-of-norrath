// Observed getMegatileOriginalPosition__FG6Point3 at 0x00265ac8, 724 bytes. Arguments:
// a0 = hidden result pointer (a Point3 of three singles, returned in V0), a1 = address of
// the by-value Point3 p. The scale is the single at 0x00482068 (bits 0x3ab60b60, roughly
// 1/720), a gp-relative constant with no symbol.
//  1. scaled = p * scale, componentwise (x, y, z).
//  2. mt_tiledata (0x00434068, 320 bytes) holds up to 32 ten-byte signed-byte records.
//     The scan starts at record 0 and stops when the byte at record offset 2 is -1 (also
//     tested before record 0), or after 32 records have failed.
//  3. A record matches when float(r[7]) <= scaled.x, float(r[8]) <= scaled.y,
//     scaled.x <= float(r[7] + r[4]) and scaled.y <= float(r[8] + r[3]); a NaN fails.
//  4. On a match: cx = 360 * (2*r[7] + r[4]) and cy = 360 * (2*r[8] + r[3]) as ints, then
//     converted to single. d = (p.x - cx, p.y - cy, p.z). A matrix is set to identity and
//     rotated about axis 2 by the short ((int)((unsigned)r[2] << 30) >> 16) via matRot;
//     matUntransform(out, matrix, d) follows. The result is out with
//     x += float(360 * (2*r[0] + r[6])) and y += float(360 * (2*r[1] + r[5])); z is out.z.
//  5. With no match the result is the original p, unscaled.
// Inferred only: this maps a position inside a rotated megatile back to the tile's original
// layout; the 720-unit tile pitch is read from the constants. The record field roles
// (origin, size, rotation index) are guesses from the arithmetic. Fresh compilation is not
// an original-byte match.
struct Point3 {
    float x, y, z;
};
struct Matrix33 {
    float m[9];
};

extern "C" {
extern signed char mt_tiledata[];
void matIdent__FP8Matrix33(Matrix33*);
void matRot__FP8Matrix33is(Matrix33*, int axis, short angle);
void matUntransform__FP6Point3P8Matrix33T0(Point3* out, const Matrix33* matrix, const Point3* in);

Point3* getMegatileOriginalPosition__FG6Point3(Point3* result, const Point3* argument) {
    const Point3 p = *argument;
    const float scale = __builtin_bit_cast(float, 0x3ab60b60u);
    Point3 scaled;
    scaled.x = p.x * scale;
    scaled.y = p.y * scale;
    scaled.z = p.z * scale;
    if (mt_tiledata[2] == -1) {
        *result = p;
        return result;
    }
    const signed char* record = mt_tiledata;
    for (int tried = 0; tried < 32;) {
        const int x0 = record[7];
        const int y0 = record[8];
        const int width = record[4];
        const int height = record[3];
        if (static_cast<float>(x0) <= scaled.x && static_cast<float>(y0) <= scaled.y &&
            scaled.x <= static_cast<float>(x0 + width) && scaled.y <= static_cast<float>(y0 + height)) {
            const float cx = static_cast<float>(360 * (2 * x0 + width));
            const float cy = static_cast<float>(360 * (2 * y0 + height));
            Point3 local;
            local.x = p.x - cx;
            local.y = p.y - cy;
            local.z = p.z;
            Matrix33 matrix;
            matIdent__FP8Matrix33(&matrix);
            matRot__FP8Matrix33is(&matrix, 2, static_cast<short>(static_cast<int>(static_cast<unsigned>(record[2]) << 30) >> 16));
            Point3 out;
            matUntransform__FP6Point3P8Matrix33T0(&out, &matrix, &local);
            const float ox = static_cast<float>(360 * (2 * record[0] + record[6]));
            const float oy = static_cast<float>(360 * (2 * record[1] + record[5]));
            out.x = out.x + ox;
            out.y = out.y + oy;
            *result = out;
            return result;
        }
        ++tried;
        record += 10;
        if (tried >= 32) break;
        if (record[2] == -1) break;
    }
    *result = p;
    return result;
}
}
