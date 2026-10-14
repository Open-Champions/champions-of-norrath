// Observed: Transform__10VIMatrix44RC7VIVect3fT1 at 0x010999f0 is 436 bytes in the pinned SLUS-20565 image. Arguments: the matrix
// (this), a vector of three angles (a1), a scale (first float argument) and a translation vector (a2). It calls sinf then cosf on
// angle 0, then on angle 1, then on angle 2 (six calls in that order), calling them with the angle read from memory each time.
// With s0 c0 = sin cos of angle 0, s1 c1 of angle 1, s2 c2 of angle 2, and k the scale, it writes the 16 floats:
//   row 0: (c2*c0 + (s2*s1)*s0)*k,  (s2*c1)*k,  ((-c2)*s0 + (s2*s1)*c0)*k,  0
//   row 1: ((-s2)*c0 + (c2*s1)*s0)*k,  (c2*c1)*k,  (s2*s0 + (c2*s1)*c0)*k,  0
//   row 2: (c1*s0)*k,  (-s1)*k,  (c1*c0)*k,  0
//   row 3: translation x, y, z, 1.0f
// and returns nothing. Association of every sum and product is as written.
// Inferred: builds a scaled rotation from Euler angles plus a translation, in the row-vector convention the other VIMatrix44
// routines use (translation in row 3). Which angle is which axis is not established here.
// Unknown: sinf and cosf are imports; the units of the angles (radians is the usual convention), and whether callers rely on
// the unwritten return register. Nothing here ran on the EE.
extern "C" {
float sinf(float);
float cosf(float);

void Transform__10VIMatrix44RC7VIVect3fT1(float* m, const float* angles, float scale, const float* translation) {
    float s0 = sinf(angles[0]);
    float c0 = cosf(angles[0]);
    float s1 = sinf(angles[1]);
    float c1 = cosf(angles[1]);
    float s2 = sinf(angles[2]);
    float c2 = cosf(angles[2]);
    float s2s1 = s2 * s1;
    float c2s1 = c2 * s1;
    m[0] = (c2 * c0 + s2s1 * s0) * scale;
    m[1] = (s2 * c1) * scale;
    m[2] = ((-c2) * s0 + s2s1 * c0) * scale;
    m[4] = ((-s2) * c0 + c2s1 * s0) * scale;
    m[5] = (c2 * c1) * scale;
    m[6] = (s2 * s0 + c2s1 * c0) * scale;
    m[8] = (c1 * s0) * scale;
    m[9] = (-s1) * scale;
    m[10] = (c1 * c0) * scale;
    m[3] = 0.0f;
    m[7] = 0.0f;
    m[11] = 0.0f;
    m[12] = translation[0];
    m[13] = translation[1];
    m[14] = translation[2];
    m[15] = 1.0f;
}
}
