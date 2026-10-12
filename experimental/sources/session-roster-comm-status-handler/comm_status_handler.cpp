// Observed commStatusHandler__FPUci at 0x002cd130, 56 bytes. The first packet
// byte carries a four-bit mask in its low nibble and a set/clear flag in bit
// 7. A set flag ORs the nibble into commSendmask; a clear flag removes those
// bits. The length argument is not read. The word is defined elsewhere.
extern "C" unsigned commSendmask;

extern "C" void commStatusHandler__FPUci(unsigned char* packet, int length) {
    (void)length;
    const unsigned char first = packet[0];
    const unsigned bits = first & 0xfu;
    if (first & 0x80u) {
        commSendmask = commSendmask | bits;
    } else {
        commSendmask = commSendmask & ~bits;
    }
}
