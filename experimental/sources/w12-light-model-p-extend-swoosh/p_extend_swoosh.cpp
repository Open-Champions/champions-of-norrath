// Observed P_ExtendSwoosh__FiG6Point3T1f at 0x00183ed0, extent 428. Arguments: an int handle, two
// Point3 values (each by address of a caller copy; copied to the stack first) a and b, and a float f.
// Returns nothing. trail (0x00780b00) is a table of 1040-byte records; the record is the one for
// the low byte of the handle. Record words used: the signed halfword at +1024 (the handle it was
// started with), the byte at +1028 (tail), the byte at +1029 (head), the unsigned halfword at
// +1026 (n). The first 1024 bytes are a ring of 128 entries of four halfwords.
//  1. Nothing happens when the handle is 0, when the halfword at +1024 differs from the whole handle,
//     or when the ring has fewer than two free entries: (head + 1) & 127 equals tail, or
//     (head + 2) & 127 equals tail.
//  2. Entry head receives (int)(2 * a.x), (int)(2 * a.y), (int)(2 * a.z) as halfwords at +0, +2, +4
//     and (n + 1) at +6; head becomes (head + 1) & 127.
//  3. The new entry head receives (int)(2 * b.x), (int)(2 * b.y), (int)(2 * b.z) and (int)(f * 255.0f)
//     (truncated; stored as halfwords); head becomes (head + 1) & 127 again.
// Inferred: appends a segment of a swoosh (sword-trail) ribbon: two points stored at half-unit
// precision, with f as a 0..1 intensity scaled to a byte-sized value and n+1 a sequence marker.
// Unknown: how the renderer reads the entries, what the +6 halfword of the first entry is used for,
// and where n (+1026) is updated. Nothing here was executed on the EE.
struct Point3 {
    float x, y, z;
};
typedef short __attribute__((may_alias)) i16;
typedef unsigned short __attribute__((may_alias)) u16;
template <class T> static inline T& at(unsigned char* base, int offset) { return *reinterpret_cast<T*>(base + offset); }
extern "C" {
extern unsigned char trail[66560];
void P_ExtendSwoosh__FiG6Point3T1f(int handle, const Point3* aArg, const Point3* bArg, float f) {
    const Point3 a = *aArg;
    const Point3 b = *bArg;
    if (handle == 0) return;
    unsigned char* record = trail + (handle & 0xff) * 1040;
    if (at<i16>(record, 1024) != handle) return;
    const unsigned head = record[1029];
    const unsigned tail = record[1028];
    if (((head + 1) & 0x7f) == tail) return;
    if (((head + 2) & 0x7f) == tail) return;
    const int ax = static_cast<int>(a.x + a.x);
    const int ay = static_cast<int>(a.y + a.y);
    const int az = static_cast<int>(a.z + a.z);
    const int bx = static_cast<int>(b.x + b.x);
    const int by = static_cast<int>(b.y + b.y);
    const int bz = static_cast<int>(b.z + b.z);
    const int scaled = static_cast<int>(f * 255.0f);
    at<i16>(record, record[1029] * 8 + 0) = static_cast<short>(ax);
    at<i16>(record, record[1029] * 8 + 2) = static_cast<short>(ay);
    at<i16>(record, record[1029] * 8 + 4) = static_cast<short>(az);
    at<u16>(record, record[1029] * 8 + 6) = static_cast<unsigned short>(at<u16>(record, 1026) + 1);
    record[1029] = (record[1029] + 1) & 0x7f;
    at<i16>(record, record[1029] * 8 + 0) = static_cast<short>(bx);
    at<i16>(record, record[1029] * 8 + 2) = static_cast<short>(by);
    at<i16>(record, record[1029] * 8 + 4) = static_cast<short>(bz);
    at<i16>(record, record[1029] * 8 + 6) = static_cast<short>(scaled);
    record[1029] = (record[1029] + 1) & 0x7f;
}
}
