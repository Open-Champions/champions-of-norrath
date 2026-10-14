// Observed: CalcMaxWidth__7VIRFontiPUc at 0x01136208 is 68 bytes in the pinned SLUS-20565 image. It stores 0 at offset 24 of the font, then
// for each of the first a1 bytes of a2 (as unsigned bytes) replaces the word at +24 with that byte if the byte is larger (a signed
// compare against the current word, which is never negative). The count a1 is signed: zero or negative leaves +24 at 0. It returns 0.
// Inferred: the byte array holds one width per character code and +24 becomes the largest width (the name says max width). This is the
// first helper Init__7VIRFont... runs, with its `size` argument as the count.
// Unknown: what the widths measure, and the array's origin. Nothing here ran on the EE.
extern "C" {
int CalcMaxWidth__7VIRFontiPUc(unsigned char* self, int count, unsigned char* widths) {
    *reinterpret_cast<int*>(self + 24) = 0;
    for (int i = 0; i < count; ++i) {
        int w = widths[i];
        int cur = *reinterpret_cast<int*>(self + 24);
        *reinterpret_cast<int*>(self + 24) = w < cur ? cur : w;
    }
    return 0;
}
}
