// Observed lightSetSecondaryDirectional__FG6Point3Ps at 0x0013e8a0, extent 132.
// (direction, color). The 12-byte Point3 is passed by pointer to a caller copy; it
// is copied to a local, then the four halfwords at color are copied to charDirect2
// (0x00486860), then matTransform34Vec__FP6Point3P8Matrix34T0 is called with output charDirectDir2
// (0x004f40e0), matrix worldCameraMatrix (0x0037ef00) and the local copy as input. After
// the call the float at byte offset 8 of charDirectDir2 is negated in place.
// Inferred: the direction is rotated into camera space and its z is flipped.
// charDirect2, charDirectDir2 and worldCameraMatrix are defined by other packages.
struct Point3 {
    float x, y, z;
};

extern "C" {
extern unsigned char charDirect2[8];
extern unsigned char charDirectDir2[16];
extern unsigned char worldCameraMatrix[];
void matTransform34Vec__FP6Point3P8Matrix34T0(unsigned char* out, unsigned char* matrix, unsigned char* in);

void lightSetSecondaryDirectional__FG6Point3Ps(Point3* direction, short* color) {
    Point3 local = *direction;
    short* dest = reinterpret_cast<short*>(charDirect2);
    dest[0] = color[0];
    dest[1] = color[1];
    dest[2] = color[2];
    dest[3] = color[3];
    matTransform34Vec__FP6Point3P8Matrix34T0(charDirectDir2, worldCameraMatrix, reinterpret_cast<unsigned char*>(&local));
    float* z = reinterpret_cast<float*>(charDirectDir2 + 8);
    *z = -*z;
}
}
