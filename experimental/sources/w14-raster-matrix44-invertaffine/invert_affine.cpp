// Observed: InvertAffine__10VIMatrix44RC10VIMatrix44 at 0x010d0ab0 is 164 bytes in the pinned SLUS-20565 image. The destination is
// this and the argument is the source matrix, four rows of four floats (rows r0, r1, r2 and the translation row t). It uses VU0 macro
// mode only, with no loop or branch, and returns nothing. In order, with u x v meaning the cross product of the VU0 operand pair
// shown (xyz lanes only, each component computed as one product minus another):
//   c0 = r1 x r2, c1 = r0 x r2, c2 = r0 x r1, with component k of a cross product (a x b) = a[j]*b[l] - b[j]*a[l] for the
//   cyclic index triples (j,l) = (y,z), (z,x), (x,y);
//   det = (r0.x*c0.x + r0.y*c0.y) + r0.z*c0.z;   q = 1.0f / det   (a divide, no check for zero);
//   output row 0 = ((0 + c0.x), (0 - c1.x), (0 + c2.x)) * q, w = 0
//   output row 1 = ((0 + c0.y), (0 - c1.y), (0 + c2.y)) * q, w = 0
//   output row 2 = ((0 + c0.z), (0 - c1.z), (0 + c2.z)) * q, w = 0
//   output row 3 = ((0 - o0.x*t.x) - o1.x*t.y) - o2.x*t.z, and the same for y and z, where o0..o2 are the three output rows
//   after scaling, with w = 1.0f. The "0 +" and "0 -" terms are real additions and subtractions of +0.0f.
// Inferred: this is the inverse of a matrix whose last column is (0, 0, 0, 1), using the adjugate of the 3x3 part and then the
// inverse translation -t * R^-1, in the row-vector convention of the other VIMatrix44 routines. Rows 0..2 of the output are the
// transpose of the cofactor matrix over det, with the middle cofactor negated. The source's w lanes are not read as data.
// Unknown: behaviour for a singular matrix beyond "the divide is not checked" (the VU0 divide-by-zero flag is not modelled here), and
// hardware VU0 rounding (the private interpreter models round-to-nearest). Nothing here ran on the EE.
extern "C" {
void InvertAffine__10VIMatrix44RC10VIMatrix44(float* out, const float* in) {
    const float* r0 = in;
    const float* r1 = in + 4;
    const float* r2 = in + 8;
    const float* t = in + 12;
    float c0x = r1[1] * r2[2] - r2[1] * r1[2];
    float c0y = r1[2] * r2[0] - r2[2] * r1[0];
    float c0z = r1[0] * r2[1] - r2[0] * r1[1];
    float c1x = r0[1] * r2[2] - r2[1] * r0[2];
    float c1y = r0[2] * r2[0] - r2[2] * r0[0];
    float c1z = r0[0] * r2[1] - r2[0] * r0[1];
    float c2x = r0[1] * r1[2] - r1[1] * r0[2];
    float c2y = r0[2] * r1[0] - r1[2] * r0[0];
    float c2z = r0[0] * r1[1] - r1[0] * r0[1];
    float det = (r0[0] * c0x + r0[1] * c0y) + r0[2] * c0z;
    float q = 1.0f / det;
    float o0x = (0.0f + c0x) * q, o0y = (0.0f - c1x) * q, o0z = (0.0f + c2x) * q;
    float o1x = (0.0f + c0y) * q, o1y = (0.0f - c1y) * q, o1z = (0.0f + c2y) * q;
    float o2x = (0.0f + c0z) * q, o2y = (0.0f - c1z) * q, o2z = (0.0f + c2z) * q;
    float o3x = (0.0f - o0x * t[0]) - o1x * t[1];
    float o3y = (0.0f - o0y * t[0]) - o1y * t[1];
    float o3z = (0.0f - o0z * t[0]) - o1z * t[1];
    o3x = o3x - o2x * t[2];
    o3y = o3y - o2y * t[2];
    o3z = o3z - o2z * t[2];
    out[0] = o0x; out[1] = o0y; out[2] = o0z; out[3] = 0.0f;
    out[4] = o1x; out[5] = o1y; out[6] = o1z; out[7] = 0.0f;
    out[8] = o2x; out[9] = o2y; out[10] = o2z; out[11] = 0.0f;
    out[12] = o3x; out[13] = o3y; out[14] = o3z; out[15] = 1.0f;
}
}
