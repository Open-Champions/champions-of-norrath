// Observed memmove at 0x003020ac, 260 bytes. Returns the destination.
// If the source is strictly below the destination and the destination starts
// below source + count (unsigned compares), the bytes are copied from the last
// to the first, so the overlapping tail is read before it is overwritten. In
// every other case, including a zero count, the copy runs forward. The original
// forward path moves 16-byte quadwords while 32 or more bytes remain and both
// pointers are 16-aligned, then 8-byte words, then bytes; only the byte-level
// result is authored here. The count is unsigned.
extern "C" {
void* memmove(void* destination, const void* source, unsigned count) {
    unsigned char* to = static_cast<unsigned char*>(destination);
    const unsigned char* from = static_cast<const unsigned char*>(source);
    if (from < to && to < from + count) {
        while (count != 0u) {
            --count;
            to[count] = from[count];
        }
    } else {
        for (unsigned i = 0; i != count; ++i) to[i] = from[i];
    }
    return destination;
}
}
