// Observed gameLineGetNextPoint__FP17gameLinePathStateiP6Point3 at 0x00279c10, 80 bytes in
// SLUS_205.65. Arguments: the path state, a signed step, and an output Point3 (three floats).
// The state's word at +28 is a line number (an index into the table gameLines, 0x008d57a8)
// and the word at +32 a current point index. The routine adds the step to the point index,
// reduces it by the line's point count (record +4) with a signed remainder, and copies the
// first 12 bytes of that point (points pointer at record +20, 64 bytes per point) to the
// output. The state itself is not written.
// Inferred: same line record reading as gameLineGetVert; the output is a position.
// Unknown: behaviour for a zero count (the image divides without a check).
extern unsigned char* gameLines[100];

struct Point3Words { float x, y, z; };

extern "C" void gameLineGetNextPoint__FP17gameLinePathStateiP6Point3(
    unsigned char* state, int step, Point3Words* out) {
    unsigned char* record = gameLines[*reinterpret_cast<int*>(state + 28)];
    const int at = *reinterpret_cast<int*>(state + 32) + step;
    const int count = *reinterpret_cast<int*>(record + 4);
    unsigned char* points = *reinterpret_cast<unsigned char**>(record + 20);
    *out = *reinterpret_cast<Point3Words*>(points + (at % count) * 64);
}
