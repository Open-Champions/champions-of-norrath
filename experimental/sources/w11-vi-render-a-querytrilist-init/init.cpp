// Observed: Init__14VIQueryTriListiRC6VIBBoxRC7VIVect3f at 0x010d1048 is 184 bytes.
// Arguments: object, an int stored at offset 4, a bounding-box reference, a vector
// reference, and a float. It stores zero at offset 0, then calls in order
// Allocate__14VIQueryTriList(this), Init__9VIClipperi(this+12, 9) and
// Volume__14VIQueryTriListRC6VIBBox(this, box). A negative result from any of
// them returns -1 at once. Otherwise it copies the three floats of the vector to
// offset 28, calls cosf(angle * k) with k the float bits 0x3c8efa34 (pi/180
// truncated in the last place), stores the result at offset 40, and returns 0.
// Inferred: the float argument is an angle in degrees and offset 40 holds the
// cosine of it; the other three calls are the setup of the helper clipper.
extern "C" {
int Allocate__14VIQueryTriList(void* object);
int Init__9VIClipperi(void* clipper, int count);
int Volume__14VIQueryTriListRC6VIBBox(void* object, const void* box);
float cosf(float value);

int Init__14VIQueryTriListiRC6VIBBoxRC7VIVect3f(void* object, int count, const void* box, const void* vector, float angle) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<int*>(bytes + 4) = count;
    *reinterpret_cast<int*>(bytes + 0) = 0;
    if (Allocate__14VIQueryTriList(object) < 0) return -1;
    if (Init__9VIClipperi(bytes + 12, 9) < 0) return -1;
    if (Volume__14VIQueryTriListRC6VIBBox(object, box) < 0) return -1;
    const float* source = static_cast<const float*>(vector);
    float* target = reinterpret_cast<float*>(bytes + 28);
    target[0] = source[0];
    target[1] = source[1];
    target[2] = source[2];
    *reinterpret_cast<float*>(bytes + 40) = cosf(angle * __builtin_bit_cast(float, 0x3c8efa34u));
    return 0;
}
}
