// Observed floorPutSaveInfo__FPc at 0x00192e18, 228 bytes. It rebuilds the
// floor records from a saved byte block and returns the number of bytes used.
//
// Observed block layout: a signed word count at +0 (+4 is not read), then
// `count` 12-byte records starting at +8. Each record is five signed halfwords
// then a signed byte: h0, h1, h2 are position components, h3 a float argument,
// h4 an angle-style short, and the byte an integer argument. The function calls
// floorClear, then floorAdd once per record with a point
// (h0 / 4, h1 / 4, h2 / 4 - 3.0), the short h4, the float h3 / 4, the byte, and
// a final 0. The result is 8 + 12 * count for a positive count and 8 otherwise.
// Unknown: what the saved records mean beyond the floorAdd arguments, and
// whether h3 is a size (the callee's float parameter).
struct FloorPoint {
    float x, y, z;
};

extern "C" void floorClear__Fv();
extern "C" void floorAdd__FG6Point3sfii(const FloorPoint* point, short a, float b, int c, int d);

extern "C" int floorPutSaveInfo__FPc(char* data) {
    floorClear__Fv();
    const int count = *reinterpret_cast<int*>(data);
    const char* cursor = data + 8;
    for (int left = count; left > 0; --left) {
        const short* h = reinterpret_cast<const short*>(cursor);
        const signed char tail = cursor[10];
        cursor += 12;
        FloorPoint p;
        p.x = (float)h[0] * 0.25f;
        p.y = (float)h[1] * 0.25f;
        p.z = (float)h[2] * 0.25f - 3.0f;
        floorAdd__FG6Point3sfii(&p, h[4], (float)h[3] * 0.25f, tail, 0);
    }
    return (int)(cursor - data);
}
