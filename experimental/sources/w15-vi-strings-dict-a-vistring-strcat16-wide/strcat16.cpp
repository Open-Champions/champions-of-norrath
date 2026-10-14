// Observed: strcat16__8VIStringPUsUiPCUs at 0x0101adb8 is 160 bytes. Arguments: destination of
// 16-bit units (a0), a limit (a1, unsigned) and a source of 16-bit units (a2). The destination's
// current length is measured (units before its first zero), then source units are appended at that
// position while the source unit is non-zero and (byte offset of the write cursor from the start)
// + 2 < limit. A zero unit is always written at the final cursor. The return value is the original
// destination pointer. There is no clamp when the destination is already longer than the limit.
// Inferred: the append counterpart of the bounded 16-bit copy; the limit is the destination size
// in bytes.
// Unknown: behaviour callers expect for an over-long destination.
extern "C" {
unsigned short* strcat16__8VIStringPUsUiPCUs(unsigned short* dst, unsigned int limit, const unsigned short* src) {
    unsigned short* out = dst;
    while (*out != 0) ++out;
    while (*src != 0 && static_cast<unsigned int>(reinterpret_cast<unsigned char*>(out) - reinterpret_cast<unsigned char*>(dst)) + 2 < limit) {
        *out++ = *src++;
    }
    *out = 0;
    return dst;
}
}
