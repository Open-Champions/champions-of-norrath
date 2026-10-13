// Observed modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf at 0x0016cd68,
// extent 252. Arguments: model header, a Matrix34, an index, an output Point3 pointer
// and an output float pointer. Returns nothing.
//
// Header use: the byte at header+36 times 16 is added to the header address; the word
// at +4 of that spot is a count. The entries start 32 bytes past it. If the index is
// not below the count (signed compare) the point is (0,0,0) and the float is 0.0.
// Otherwise the entry is at 32 + 8 * index + 16 * (index / 45) bytes (signed division;
// every 45 eight-byte entries are followed by 16 more bytes). Its first three halfwords
// are scaled by 1/16 to give the point, and the halfword at +6 converted to float is
// the float output. The point goes through matTransform(out, matrix, &point).
// Inferred: the name suggests the position and size of a shadow; not otherwise shown.
// Unknown: what the 16-byte gaps hold.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" {
Point3* matTransform__FP6Point3P8Matrix34T0(Point3*, const Matrix34*, const Point3*);

void modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(const unsigned char* header, const Matrix34* matrix,
                                                               int index, Point3* outPoint, float* outValue) {
    const unsigned char* table = header + header[36] * 16;
    const unsigned char* entry = table + 32 + index * 8;
    Point3 point;
    if (index >= *reinterpret_cast<const int*>(table + 4)) {
        point.x = 0.0f;
        point.y = 0.0f;
        point.z = 0.0f;
        matTransform__FP6Point3P8Matrix34T0(outPoint, matrix, &point);
        *outValue = 0.0f;
        return;
    }
    entry += (index / 45) * 16;
    const short* h = reinterpret_cast<const short*>(entry);
    point.x = h[0] * 0.0625f;
    point.y = h[1] * 0.0625f;
    point.z = h[2] * 0.0625f;
    matTransform__FP6Point3P8Matrix34T0(outPoint, matrix, &point);
    *outValue = static_cast<float>(h[3]);
}
}
