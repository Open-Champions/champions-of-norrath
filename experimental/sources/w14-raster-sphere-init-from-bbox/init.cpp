// Observed: Init__8VISphereRC6VIBBox at 0x01005108 is 176 bytes in the pinned SLUS-20565 image. The sphere is this (centre x, y, z
// at +0, +4, +8, radius at +12); the argument is a box holding a minimum corner at +0 and a maximum corner at +12. Each centre
// component is (min + max) * 0.5. A local three-float vector (max.x - c.x, max.y - c.y, max.z - c.z) is passed to
// Magnitude__C7VIVect3 and its float result is stored at +12. The routine returns zero in the integer result register.
// Inferred: the bounding sphere of an axis-aligned box. The radius is the half diagonal. The zero result is an int status.
// Unknown: the sphere's size beyond 16 bytes, and Magnitude__C7VIVect3's body (an import; the name suggests the vector length).
// Nothing here ran on the EE.
extern "C" {
float Magnitude__C7VIVect3(const float* v);

int Init__8VISphereRC6VIBBox(float* sphere, const float* box) {
    const float* low = box;
    const float* high = box + 3;
    float cx = (low[0] + high[0]) * 0.5f;
    float cy = (low[1] + high[1]) * 0.5f;
    float cz = (low[2] + high[2]) * 0.5f;
    sphere[0] = cx;
    sphere[1] = cy;
    sphere[2] = cz;
    float half[3];
    half[2] = high[2] - cz;
    half[0] = high[0] - cx;
    half[1] = high[1] - cy;
    sphere[3] = Magnitude__C7VIVect3(half);
    return 0;
}
}
