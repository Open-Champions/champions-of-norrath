// Observed: __13VIPickSegment at 0x010bf470 is 112 bytes. It stores float 0.0 into the three
// floats at each of offsets 0, 12, 24, 36, 64 and 76, stores float 0.0 at offset 88, and stores
// the word -1 at offset 60. Nothing else is written or read. The return register still holds -1
// from that store when the routine returns.
// Inferred: a default constructor that zeroes the segment's two end points, the two vectors
// after them, the direction at 64 and the trailing vector at 76 plus its fourth float at 88, and
// marks the word at 60 as -1 (unset). The return value is incidental and no caller is known to
// use it. Unknown: the names of the fields and what offsets 48, 52, 56 and 92 onward hold; they
// are not written here, so the object's remaining bytes keep their old contents.
extern "C" {
int __13VIPickSegment(unsigned char* self) {
    float* f = reinterpret_cast<float*>(self);
    int* tag = reinterpret_cast<int*>(self + 60);
    f[0] = 0.0f;
    f[1] = 0.0f;
    f[2] = 0.0f;
    *tag = -1;
    f[5] = 0.0f;
    f[3] = 0.0f;
    f[4] = 0.0f;
    f[8] = 0.0f;
    f[6] = 0.0f;
    f[7] = 0.0f;
    f[11] = 0.0f;
    f[9] = 0.0f;
    f[10] = 0.0f;
    f[18] = 0.0f;
    f[16] = 0.0f;
    f[17] = 0.0f;
    f[22] = 0.0f;
    f[19] = 0.0f;
    f[20] = 0.0f;
    f[21] = 0.0f;
    return -1;
}
}
