// Observed P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf at 0x00181660, extent 248. Arguments: two Point3
// values (each by address of a caller copy), eight unsigned bytes c0..c7 and a float f. Returns
// nothing. lineBuffer (0x006f4880) is 128 records of 36 bytes; nmLines (0x004869fc) counts the
// used ones. When nmLines is 128 nothing happens. Otherwise record nmLines receives the first
// point at +0, the second at +12, the eight bytes at +24..+31 in argument order and f at +32,
// and nmLines is incremented.
// Inferred: queues one line segment (two end points, eight bytes that may be colours or flags, and a
// width or time float) for a later pass, dropped silently when the buffer is full. Unknown: the
// meaning of each byte and of f, and which pass drains the buffer. Nothing here was executed on
// the EE.
struct Point3 {
    float x, y, z;
};
extern "C" {
extern unsigned char lineBuffer[128 * 36];
extern unsigned nmLines;
void P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf(Point3 a, Point3 b, unsigned char c0, unsigned char c1, unsigned char c2,
                                          unsigned char c3, unsigned char c4, unsigned char c5, unsigned char c6,
                                          unsigned char c7, float f) {
    if (nmLines == 128) return;
    unsigned char* record = lineBuffer + nmLines * 36;
    ++nmLines;
    *reinterpret_cast<Point3*>(record) = a;
    *reinterpret_cast<Point3*>(record + 12) = b;
    *reinterpret_cast<float*>(record + 32) = f;
    record[24] = c0;
    record[25] = c1;
    record[26] = c2;
    record[27] = c3;
    record[28] = c4;
    record[29] = c5;
    record[30] = c6;
    record[31] = c7;
}
}
