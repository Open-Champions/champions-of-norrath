// Observed: Reset__14VIQueryTriListRC6VIBBoxRC7VIVect3f at 0x010d1100 is 132 bytes. Arguments:
// list (a0), a box (a1), a vector of three floats (a2) and a float angle (f12). It stores
// zero at +0, then calls Volume__14VIQueryTriListRC6VIBBox(list, box). If that returns a
// negative value the function returns -1 without further stores. Otherwise it copies the
// three floats of the vector to +28, +32 and +36, stores cosf(angle * 0.017453292f) at +40
// (the multiplier is the float 0x3c8efa34, pi / 180) and returns 0.
// Inferred: the query keeps a triangle count at +0 (reset to zero), an axis or direction
// vector at +28 and the cosine of a cone half-angle given in degrees at +40.
// Unknown: what Volume computes (apart from "negative means failure") and the other fields.
extern "C" {
int Volume__14VIQueryTriListRC6VIBBox(void* list, const void* box);
float cosf(float x);

int Reset__14VIQueryTriListRC6VIBBoxRC7VIVect3f(unsigned char* list, const void* box, const float* axis, float angle) {
    *reinterpret_cast<int*>(list) = 0;
    if (Volume__14VIQueryTriListRC6VIBBox(list, box) < 0) {
        return -1;
    }
    float* dst = reinterpret_cast<float*>(list + 28);
    dst[0] = axis[0];
    dst[1] = axis[1];
    dst[2] = axis[2];
    *reinterpret_cast<float*>(list + 40) = cosf(angle * 0.017453292f);
    return 0;
}
}
