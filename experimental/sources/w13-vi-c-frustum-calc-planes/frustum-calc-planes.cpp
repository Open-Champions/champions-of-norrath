// Observed: CalcPlanes__9VIFrustum at 0x01017098, 812 bytes. The frustum holds eight
// corner points of three floats each at +0, +12, ..., +84 (call them c0..c7) followed by
// six 16-byte planes at +96, +112, ..., +176. The function builds each plane by calling the
// plane's three-point setter, __cl__7VIPlaneRC7VIVect3N21 (plane, p1, p2, p3), with the
// points as copies of the corners, in this order:
//   +96  : c1, c2, c3        +112 : c4, c7, c6        +128 : c0, c3, c7
//   +144 : c5, c6, c2        +160 : c5, c1, c0        +176 : c2, c6, c7
// (p1, p2, p3 listed in call order). The result is zero. The image also copies one more
// corner into an unused fourth stack temporary before each call; that has no effect and
// is not reproduced.
// Inferred: the corner numbering (near face c0..c3, far face c4..c7) is a guess from the
// grouping and is not used here. Unknown: member names and the plane convention.
extern "C" {
void __cl__7VIPlaneRC7VIVect3N21(void* plane, const float* p1, const float* p2, const float* p3);

int CalcPlanes__9VIFrustum(void* frustum) {
    unsigned char* bytes = static_cast<unsigned char*>(frustum);
    const float* corner[8];
    for (int i = 0; i < 8; ++i) corner[i] = reinterpret_cast<const float*>(bytes + i * 12);
    __cl__7VIPlaneRC7VIVect3N21(bytes + 96, corner[1], corner[2], corner[3]);
    __cl__7VIPlaneRC7VIVect3N21(bytes + 112, corner[4], corner[7], corner[6]);
    __cl__7VIPlaneRC7VIVect3N21(bytes + 128, corner[0], corner[3], corner[7]);
    __cl__7VIPlaneRC7VIVect3N21(bytes + 144, corner[5], corner[6], corner[2]);
    __cl__7VIPlaneRC7VIVect3N21(bytes + 160, corner[5], corner[1], corner[0]);
    __cl__7VIPlaneRC7VIVect3N21(bytes + 176, corner[2], corner[6], corner[7]);
    return 0;
}
}
