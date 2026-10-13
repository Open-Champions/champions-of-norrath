// Observed modelGetBoundingBox__FP12_modelHeaderlPiN52 at 0x0016c820, 336 bytes.
// Arguments: a model header, a 32-bit selection mask and six int pointers (the
// mangling repeats the pointer type for the last five). No result.
//   count = signed byte at header + 18; second = signed byte at header + 38.
//   Boxes start at header + 40 + second * (count * 4 + 4); each is 10 bytes of
//   which the first six are signed bytes: three minima then three maxima.
//   The six outputs start as 100000, 100000, 100000, -100000, -100000, -100000.
//   For box i in 0..count-1, while the mask is nonzero, the box is merged
//   (per-axis minimum into outputs 0..2, maximum into outputs 3..5) only when
//   bit 0 of the mask is set; the mask is shifted right by one after each box.
// A count of zero or less, or a zero mask, leaves the defaults. Inferred: the box
// set selected by the mask is a per-part bounding volume. The unit of the byte
// values is unknown.
struct _modelHeader;

extern "C" void modelGetBoundingBox__FP12_modelHeaderlPiN52(_modelHeader* header, long mask, int* minX, int* minY,
                                                            int* minZ, int* maxX, int* maxY, int* maxZ) {
    const signed char* base = reinterpret_cast<const signed char*>(header);
    int count = base[18];
    int lox = 100000, loy = 100000, loz = 100000;
    int hix = -100000, hiy = -100000, hiz = -100000;
    const signed char* box = base + 40 + base[38] * (count * 4 + 4);
    for (int i = count - 1; i >= 0 && mask != 0; i--, box += 10) {
        if (mask & 1) {
            if (box[0] < lox) lox = box[0];
            if (box[1] < loy) loy = box[1];
            if (box[2] < loz) loz = box[2];
            if (hix < box[3]) hix = box[3];
            if (hiy < box[4]) hiy = box[4];
            if (hiz < box[5]) hiz = box[5];
        }
        mask >>= 1;
    }
    *minX = lox;
    *minY = loy;
    *minZ = loz;
    *maxX = hix;
    *maxY = hiy;
    *maxZ = hiz;
}
