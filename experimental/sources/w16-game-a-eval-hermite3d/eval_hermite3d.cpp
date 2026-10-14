// Observed evalHermite3D__FfP6Point3N41 at 0x00155fc0, 304 bytes, no calls. Arguments: f12 = t, a0..a3 = four vectors of
// three singles (call them P0, P1, M0, M1 by position only), t0 = output vector (the mangling N41 repeats the first
// Point3 pointer four more times, so there are five pointers).
//  1. t2 = t*t, t3 = t2*t.
//  2. Weights: w0 = (t3 + t3 - t2*3) + 1,  w1 = t3*-2 + t2*3,  w2 = (t3 - (t2 + t2)) + t,  w3 = t3 - t2.
//  3. out[i] = ((a0[i]*w0 + a1[i]*w1) + a2[i]*w2) + a3[i]*w3 for i = 0..2, summed left to right, each product rounded.
//     The output is written last, after all four inputs are read, so an output aliasing an input is safe.
// Inferred only: these are the cubic Hermite basis weights (positions a0, a1 and tangents a2, a3).
// The original also spills the products to stack temporaries; no caller can observe that.
struct Point3 {
    float x, y, z;
};

extern "C" void evalHermite3D__FfP6Point3N41(float t, const Point3* a0, const Point3* a1, const Point3* a2,
                                             const Point3* a3, Point3* out) {
    const float t2 = t * t;
    const float t3 = t2 * t;
    const float three_t2 = t2 * 3.0f;
    const float w0 = ((t3 + t3) - three_t2) + 1.0f;
    const float w1 = t3 * -2.0f + three_t2;
    const float w2 = (t3 - (t2 + t2)) + t;
    const float w3 = t3 - t2;
    const float x = ((a0->x * w0 + a1->x * w1) + a2->x * w2) + a3->x * w3;
    const float y = ((a0->y * w0 + a1->y * w1) + a2->y * w2) + a3->y * w3;
    const float z = ((a0->z * w0 + a1->z * w1) + a2->z * w2) + a3->z * w3;
    out->z = z;
    out->x = x;
    out->y = y;
}
