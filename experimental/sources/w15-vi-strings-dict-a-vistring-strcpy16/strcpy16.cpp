// Observed: strcpy16__8VIStringPUsUiPCUs at 0x0101aac8 is 100 bytes. Arguments: destination of
// 16-bit units (a0), a limit (a1, unsigned) and a source of 16-bit units (a2). The limit is compared
// against byte offsets: a unit is copied only while the source unit is non-zero and
// 2 * (units copied + 1) < limit. A zero unit is then written at the destination cursor, always,
// including for limits 0..2. The return value is the original destination pointer.
// Inferred: the limit is the destination size in bytes (not units), with one unit kept for the
// terminator.
// Unknown: whether callers pass byte sizes consistently; the name is the image's own.
extern "C" {
unsigned short* strcpy16__8VIStringPUsUiPCUs(unsigned short* dst, unsigned int limit, const unsigned short* src) {
    unsigned short* out = dst;
    unsigned int bytes = 2;
    while (*src != 0 && bytes < limit) {
        *out++ = *src++;
        bytes += 2;
    }
    *out = 0;
    return dst;
}
}
