// Observed soundPan2D__FiP6Point3f at 0x0024f7f8, extent 132. Arguments: a sound id, a Point3 (by
// address) and a float f. An id of -1 does nothing. Otherwise a stack copy of the point is passed
// to pan__FG6Point3ffPfT3 with the floats f * 360.0f and f * 840.0f and the addresses of two
// result floats (left, right in the order pan stores them); then
// soundChangeVolume__Fiff(id, first result, second result) is called. Returns nothing.
// Inferred: a 2-D positional pan helper: pan turns a position into a pair of channel volumes.
// Unknown: what 360 and 840 are (distance scales?), and which result is which channel.
struct Point3 {
    float x, y, z;
};
extern "C" {
void pan__FG6Point3ffPfT3(Point3 point, float a, float b, float* first, float* second);
void soundChangeVolume__Fiff(int id, float first, float second);
void soundPan2D__FiP6Point3f(int id, Point3* point, float f) {
    if (id == -1) return;
    float result[2];
    Point3 copy = *point;
    pan__FG6Point3ffPfT3(copy, f * 360.0f, f * 840.0f, &result[0], &result[1]);
    soundChangeVolume__Fiff(id, result[0], result[1]);
}
}
