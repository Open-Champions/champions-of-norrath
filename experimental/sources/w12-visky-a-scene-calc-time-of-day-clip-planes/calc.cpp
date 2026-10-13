// Observed: CalcTimeOfDayClipPlanes__7VISceneR21VITerrainProfileBlendRfT2 at 0x01141668 is 280
// bytes. Arguments: the scene (this), a blend record B, and two float references out1 and out2.
// B holds four signed indices i0..i3 at byte offsets 0, 4, 8, 12 and four float weights w0..w3 at
// 16, 20, 24, 28. The scene holds two tables of 40-byte records: table 1 entries start at scene+92
// and table 2 entries at scene+96, and only the float at the start of each addressed entry is read.
//   out1 = t1[i0]*w0 + t1[i1]*w1 + t1[i2]*w2 + t1[i3]*w3
//   out2 = t2[i0]*w0 + t2[i1]*w1 + t2[i2]*w2 + t2[i3]*w3
// Each sum is accumulated left to right and written to its output after every term (so the
// final value lands in the output after four stores); the weights are re-read from B after
// each store. out1 is complete before out2 starts. It returns 0.
// Inferred: a time-of-day blend of near/far clip distances over up to four profile entries.
// Unknown: which table is near or far, the record layout beyond the first float, and the types of
// the record fields beyond what is read here.
extern "C" {
int CalcTimeOfDayClipPlanes__7VISceneR21VITerrainProfileBlendRfT2(const unsigned char* self, const unsigned char* blend, float* out1, float* out2) {
    const float* w = reinterpret_cast<const float*>(blend + 16);
    const int* idx = reinterpret_cast<const int*>(blend);
    *out1 = *reinterpret_cast<const float*>(self + 92 + 40 * idx[0]) * w[0];
    *out1 = *out1 + *reinterpret_cast<const float*>(self + 92 + 40 * idx[1]) * w[1];
    *out1 = *out1 + *reinterpret_cast<const float*>(self + 92 + 40 * idx[2]) * w[2];
    *out1 = *out1 + *reinterpret_cast<const float*>(self + 92 + 40 * idx[3]) * w[3];
    *out2 = *reinterpret_cast<const float*>(self + 96 + 40 * idx[0]) * w[0];
    *out2 = *out2 + *reinterpret_cast<const float*>(self + 96 + 40 * idx[1]) * w[1];
    *out2 = *out2 + *reinterpret_cast<const float*>(self + 96 + 40 * idx[2]) * w[2];
    *out2 = *out2 + *reinterpret_cast<const float*>(self + 96 + 40 * idx[3]) * w[3];
    return 0;
}
}
