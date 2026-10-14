// Observed makeUnscrambleTable__Fv at 0x0016d410, 268 bytes. It fills the 256
// bytes of unscrambleTable (0x006e9080) in order by three nested counters: a in
// 0..7, b in 0..7, c in 0..3, with n = ((a << 5) & 0xc0) +
// 4 * ((b & 1) | ((a & 1) << 1) | ((b & 6) << 1)) + c. Each entry is built from
// bit fields of n in two small words (lo and hi) and stored as lo + (hi << 4):
//   s  = n >> 2
//   t  = ((s & 2) >> 1) | ((n & 1) << 1)
//   lo = ((s & 1) | ((s & 0xc) >> 1) | ((n & 2) << 2)) ^ ((((n >> 6) & 1) ^ (t >> 1)) << 2)
//   hi = (((n >> 6) & 3) << 2) + t
// The two words are kept in two stack slots in the image; they are plain locals here.
// Unknown: what the table is used for (the name says unscramble) and which data it
// is applied to.
extern "C" unsigned char unscrambleTable[256];

extern "C" void makeUnscrambleTable__Fv() {
    int out = 0;
    for (int a = 0; a < 8; ++a) {
        int high = (a & 1) << 1;
        int base = (a << 5) & 0xc0;
        for (int b = 0; b < 8; ++b) {
            int start = base + (((b & 1) | high | ((b & 6) << 1)) << 2);
            for (int c = 0; c < 4; ++c) {
                int n = start + c;
                int s = n >> 2;
                int t = ((s & 2) >> 1) | ((n & 1) << 1);
                int fold = n >> 6;
                int lo = ((s & 1) | ((s & 0xc) >> 1) | ((n & 2) << 2)) ^ ((((fold & 1) ^ (t >> 1))) << 2);
                int hi = ((fold & 3) << 2) + t;
                unscrambleTable[out++] = static_cast<unsigned char>(lo + (hi << 4));
            }
        }
    }
}
