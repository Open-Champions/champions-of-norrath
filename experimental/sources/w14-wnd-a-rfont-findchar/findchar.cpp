// Observed: FindChar__7VIRFontUs at 0x01136180 is 132 bytes. With (this, code) only the low 16 bits of the code argument are used (the original masks them itself).
// The table pointer is the word at +64 and the signed record count the word at +36; records are 12 bytes apart and
// each holds an unsigned 16-bit code at record offset 6. A binary search over the records (low 0, high count-1, probe
// (low+high)>>1, going up when the code is greater than the probe's code and down when it is smaller) returns the
// index of the first probe whose code is equal, or -1 when the range empties. No memory is written.
// Inferred: this is a glyph lookup in a sorted character table of a raster font; the sort order being ascending by
// that code follows from the search direction.
// Unknown: the record layout other than the code, the field names and the integer width of the result.
extern "C" {
int FindChar__7VIRFontUs(const void* self, unsigned argument) {
    const unsigned short code = static_cast<unsigned short>(argument);
    const unsigned char* base = static_cast<const unsigned char*>(self);
    const int count = *reinterpret_cast<const int*>(base + 36);
    const unsigned char* table =
        reinterpret_cast<const unsigned char*>(static_cast<unsigned long>(*reinterpret_cast<const unsigned*>(base + 64)));
    int low = 0;
    int high = count - 1;
    while (low <= high) {
        const int mid = (low + high) >> 1;
        const unsigned short probe = *reinterpret_cast<const unsigned short*>(table + mid * 12 + 6);
        if (probe == code) return mid;
        if (code < probe) high = mid - 1;
        else low = mid + 1;
    }
    return -1;
}
}
