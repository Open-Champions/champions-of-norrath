// Observed VIBufferSwap__FiPUc at 0x01166d80, 68 bytes (VIBufferSwap(int, unsigned char*)), in the overlay
// section of the image. Reverses the bytes of a buffer in place: for each of the first size/2 positions i (the
// count is the signed size shifted right by one, so nothing happens for size <= 1) it exchanges the byte at i with
// the byte at size - 1 - i. Nothing is returned.
// Inferred: it converts a field between big- and little-endian byte order; its callers do that for 4- and 8-byte
// fields when the global VIEndian is not 1.
extern "C" {
void VIBufferSwap__FiPUc(int size, unsigned char* data) {
    const int half = size >> 1;
    unsigned char* low = data;
    unsigned char* high = data + size - 1;
    for (int i = 0; i < half; ++i) {
        const unsigned char first = *high;
        const unsigned char second = *low;
        *low = first;
        *high = second;
        ++low;
        --high;
    }
}
}
