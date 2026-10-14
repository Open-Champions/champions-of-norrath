// Observed: Intersects__C6VIRectRC6VIRect at 0x01114eb8 is 92 bytes. Each rectangle is
// four 32-bit signed integers at offsets 0, 4, 8, 12. With A = this and B = the
// argument it returns 0 as soon as one of these holds, in this order:
//   B[8] < A[0];  A[8] < B[0];  B[12] < A[4];  A[12] < B[4].
// Otherwise it returns 1. Comparisons are signed and strict, so rectangles that touch
// at an edge intersect.
// Inferred: offsets 0 and 4 are the low corner and 8 and 12 the high corner of an
// inclusive integer rectangle (x then y). The member names are unknown.
extern "C" {
int Intersects__C6VIRectRC6VIRect(const int* a, const int* b) {
    if (b[2] < a[0]) return 0;
    if (a[2] < b[0]) return 0;
    if (b[3] < a[1]) return 0;
    return a[3] >= b[1];
}
}
