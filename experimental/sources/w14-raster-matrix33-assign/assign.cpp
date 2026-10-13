// Observed: __as__10VIMatrix33RC10VIMatrix33 at 0x0108adb0 is 96 bytes in the pinned SLUS-20565 image. It copies nine floats from the
// source (a1) to the destination (a0) in three groups of three (offsets 0, 12 and 24 of each) and returns the destination.
// Inferred: the assignment operator of a 3x3 float matrix (36 bytes); self-assignment is harmless because each float is read before
// it is written.
// Unknown: the class size beyond 36 bytes (this routine does not touch more). Nothing here ran on the EE.
extern "C" {
float* __as__10VIMatrix33RC10VIMatrix33(float* self, const float* other) {
    self[0] = other[0];
    self[1] = other[1];
    self[2] = other[2];
    self[3] = other[3];
    self[4] = other[4];
    self[5] = other[5];
    self[6] = other[6];
    self[7] = other[7];
    self[8] = other[8];
    return self;
}
}
