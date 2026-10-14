// Observed: StoreOutward__C6VIBBoxP7VIPlane at 0x01071808 is 180 bytes. The object (a0) holds six
// floats b[0..5]; the output (a1) is six 16-byte records of four floats and receives
//   record 0: (-1,  0,  0,  b[0])      record 1: ( 1,  0,  0, -b[3])
//   record 2: ( 0, -1,  0,  b[1])      record 3: ( 0,  1,  0, -b[4])
//   record 4: ( 0,  0, -1,  b[2])      record 5: ( 0,  0,  1, -b[5])
// and the function returns 0.
// Inferred: b[0..2] is the minimum corner and b[3..5] the maximum corner; each record is a plane
// with its normal pointing out of the box (normal components first, offset last), so a point p is
// inside when dot(n, p) + d <= 0 for all six. Unknown: the exact VIPlane field names and whether
// the box can be inverted (min above max); nothing here checks that.
extern "C" {
int StoreOutward__C6VIBBoxP7VIPlane(const float* box, float* planes) {
    planes[1] = 0.0f;
    planes[0] = -1.0f;
    planes[2] = 0.0f;
    planes[3] = box[0];
    planes[6] = 0.0f;
    planes[4] = 1.0f;
    planes[5] = 0.0f;
    planes[7] = -box[3];
    planes[10] = 0.0f;
    planes[8] = 0.0f;
    planes[9] = -1.0f;
    planes[11] = box[1];
    planes[14] = 0.0f;
    planes[12] = 0.0f;
    planes[13] = 1.0f;
    planes[15] = -box[4];
    planes[18] = -1.0f;
    planes[16] = 0.0f;
    planes[17] = 0.0f;
    planes[19] = box[2];
    planes[22] = 1.0f;
    planes[21] = 0.0f;
    planes[20] = 0.0f;
    planes[23] = -box[5];
    return 0;
}
}
