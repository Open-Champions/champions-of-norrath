/* getStockPitch__FP10_vagHeader at 0x0024f8e0 is 44 bytes. It calls
 * convertEnd__Fi on the word at offset 16 of the header, shifts the result
 * left by 12, and divides by 0xbb80 (48000) as a signed quotient.
 * Inferred from the divisor and the caller that stores the result at offset
 * 32 of the same header: a pitch relative to a 48000 Hz rate in 4.12 fixed
 * point. The meaning of convertEnd's result is not established here.
 */
extern "C" {
int convertEnd__Fi(int value);

int getStockPitch__FP10_vagHeader(void* header) {
    int rate;
    __builtin_memcpy(&rate, static_cast<unsigned char*>(header) + 16, sizeof rate);
    return (convertEnd__Fi(rate) << 12) / 0xbb80;
}
}
