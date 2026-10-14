// Observed: Size__C6VIBBox at 0x01072248, 56 bytes, in the pinned SLUS-20565 image. Arguments: a result buffer (the hidden return pointer of a
// three-float value) and the box, six consecutive floats. The three result lanes are box[3]-box[0], box[4]-box[1] and box[5]-box[2]; the result
// pointer is the return value.
// Inferred: the box holds three minimum values followed by three maximum values and the result is the extent along each axis.
// Unknown: whether lanes can be inverted (maximum below minimum); no clamping is performed.
extern "C" {
float* Size__C6VIBBox(float* result, const float* box) {
    result[0] = box[3] - box[0];
    result[1] = box[4] - box[1];
    result[2] = box[5] - box[2];
    return result;
}
}
