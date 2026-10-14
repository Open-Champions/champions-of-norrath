// Observed: Store__C6VIBBoxP7VIVect3 at 0x01071718 is 232 bytes in the pinned SLUS-20565 image. The box holds a minimum corner
// (L0, L1, L2) at +0 and a maximum corner (H0, H1, H2) at +12. The routine writes the eight corners as consecutive 12-byte vectors to
// the array at a1, in this order, and returns zero:
//   (L0,L1,L2) (H0,L1,L2) (H0,L1,H2) (L0,L1,H2) (L0,H1,L2) (H0,H1,L2) (H0,H1,H2) (L0,H1,H2)
// Inferred: this is the same corner order Transform__C6VIBBoxRC10VIMatrix44R6VIBBox uses for its eight transformed corners
// (see w13-vi-c). The array is 96 bytes.
// Unknown: whether the output may alias the box (the original interleaves loads and stores; the authored version does not claim
// the aliased result). Nothing here ran on the EE.
extern "C" {
int Store__C6VIBBoxP7VIVect3(const float* box, float* out) {
    float l0 = box[0], l1 = box[1], l2 = box[2];
    float h0 = box[3], h1 = box[4], h2 = box[5];
    out[0] = l0;  out[1] = l1;  out[2] = l2;
    out[3] = h0;  out[4] = l1;  out[5] = l2;
    out[6] = h0;  out[7] = l1;  out[8] = h2;
    out[9] = l0;  out[10] = l1; out[11] = h2;
    out[12] = l0; out[13] = h1; out[14] = l2;
    out[15] = h0; out[16] = h1; out[17] = l2;
    out[18] = h0; out[19] = h1; out[20] = h2;
    out[21] = l0; out[22] = h1; out[23] = h2;
    return 0;
}
}
