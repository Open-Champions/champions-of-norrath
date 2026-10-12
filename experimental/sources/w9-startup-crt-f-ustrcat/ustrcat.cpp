// Observed ustrcat__FPUsPc at 0x00191938, 92 bytes. It advances the 16-bit
// destination to its first zero unit, then appends each source byte
// zero-extended (the loop test reads the byte as signed char, the stored value
// is re-read as unsigned), and writes a zero unit at the end. The destination
// is not returned.
extern "C" void ustrcat__FPUsPc(unsigned short* destination, char* source) {
    while (*destination != 0) destination++;
    while (*source != 0) {
        *destination = static_cast<unsigned char>(*source);
        source++;
        destination++;
    }
    *destination = 0;
}
