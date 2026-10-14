// Observed: Clear__10VIZoneRoom at 0x0111efb0 is 212 bytes in the pinned SLUS-20565 image. It calls Clear__t7VIArray1Zi on the embedded
// array at +60 and, if that result is not negative, on the one at +68; if either result is negative it returns -1 and changes nothing
// else. Otherwise, if the word at +116 is non-zero it passes that pointer to __builtin_vec_delete. It then resets the record:
//   float 0.0 at +4, +8, +12, +16, +20, +24; word 0 at +0, +32, +36, +40, +48, +76, +80, +84, +88, +92, +96, +112, +116;
//   word -1 at +28, +44, +52, +56, +100, +104, +108.
// It returns 0.
// Inferred: +116 is the data block and +112 its size (Allocate__10VIZoneRoom allocates it); +4..+12 and +16..+24 are two 3-float
// vectors; the -1 words are "none" ids; +60 and +68 are VIArray<int> members (the callee is the template Clear for an int array).
// Unknown: the meaning of each reset field and the callee's behaviour (an import). Nothing here ran on the EE.
extern "C" {
int Clear__t7VIArray1Zi(unsigned char* array);
void __builtin_vec_delete(unsigned block);

int Clear__10VIZoneRoom(unsigned char* self) {
    if (Clear__t7VIArray1Zi(self + 60) < 0) return -1;
    if (Clear__t7VIArray1Zi(self + 68) < 0) return -1;
    unsigned block = *reinterpret_cast<unsigned*>(self + 116);
    if (block != 0) __builtin_vec_delete(block);
    float* a = reinterpret_cast<float*>(self + 4);
    float* b = reinterpret_cast<float*>(self + 16);
    a[0] = 0.0f; a[1] = 0.0f; a[2] = 0.0f;
    b[0] = 0.0f; b[1] = 0.0f; b[2] = 0.0f;
    int* w = reinterpret_cast<int*>(self);
    w[27] = -1;
    w[29] = 0;
    w[0] = 0;
    w[7] = -1;
    w[8] = 0;
    w[9] = 0;
    w[10] = 0;
    w[11] = -1;
    w[12] = 0;
    w[13] = -1;
    w[14] = -1;
    w[19] = 0;
    w[20] = 0;
    w[21] = 0;
    w[22] = 0;
    w[23] = 0;
    w[24] = 0;
    w[25] = -1;
    w[26] = -1;
    w[28] = 0;
    return 0;
}
}
