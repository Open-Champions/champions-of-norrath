// Observed: Unlock__12VIPrimBuffer at 0x011012c0 is 204 bytes. If the word at offset 12 is zero it
// returns -1 and changes nothing. Otherwise it selects on the word at offset 8 and stores floats:
//   8 == 2 or 4: float at 60 = 2^-7 (bits 0x3c000000)
//   8 == 5:      float at 60 = bits 0x3c010204 (1/127 rounded to float)
//   8 == 8:      floats at 60 and at 56 = 2^-7
//   any other:   float at 60 = 1.0f
// then clears the word at 12 and returns 0.
// Inferred: offset 12 is the lock flag set by Lock__12VIPrimBuffer; the stored float at 60 is a
// scale chosen by a per-buffer mode word. Unknown: what the values 2, 4, 5 and 8 mean.
extern "C" {
int Unlock__12VIPrimBuffer(unsigned char* self) {
    int* lock = reinterpret_cast<int*>(self + 12);
    if (*lock == 0)
        return -1;
    float* f = reinterpret_cast<float*>(self);
    switch (*reinterpret_cast<int*>(self + 8)) {
    case 2:
    case 4:
        f[15] = 0.0078125f;
        break;
    case 5:
        f[15] = 1.0f / 127.0f;
        break;
    case 8:
        f[15] = 0.0078125f;
        f[14] = 0.0078125f;
        break;
    default:
        f[15] = 1.0f;
        break;
    }
    *lock = 0;
    return 0;
}
}
