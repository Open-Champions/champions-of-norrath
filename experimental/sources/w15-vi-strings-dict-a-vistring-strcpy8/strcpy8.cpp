// Observed: strcpy8__8VIStringPcUiPCc at 0x0101aa68 is 100 bytes. Arguments: destination (a0),
// capacity (a1, unsigned) and source string (a2). Source characters are copied one at a time while
// the source character is non-zero and (copied + 1) < capacity; then a zero byte is written at the
// destination cursor, always, even when the capacity is 0 or 1 (a zero is then written at the
// first destination byte). The return value is the original destination pointer.
// Inferred: a bounded copy that reserves one byte for the terminator, capacity counted in bytes.
// Unknown: whether callers rely on the terminator write for capacity 0.
extern "C" {
char* strcpy8__8VIStringPcUiPCc(char* dst, unsigned int capacity, const char* src) {
    char* out = dst;
    unsigned int used = 1;
    while (*src != 0 && used < capacity) {
        *out++ = *src++;
        ++used;
    }
    *out = 0;
    return dst;
}
}
