// Observed: ClipByTop__11VIClipStackiP7VIVect3RiPP7VIVect3 at 0x010990f0 is 684 bytes in the pinned SLUS-20565 image. Arguments: the
// stack (this), a vertex count, a pointer to 12-byte vertices, a pointer to receive the output count and a pointer to receive the
// output vertex pointer. The stack holds an entry count at +0, a pointer to 16-byte entries at +8, a pointer to 16-byte planes at
// +32 and two scratch vertex buffers whose pointers are the words at +40 and +48 (layout seen in w12-vimath-a, plus these two).
// The top entry (index count - 1) gives a plane count n (word +0) and the index of its first plane (word +4).
// For each plane (a, b, c, d) the polygon is clipped in order, using signed distances ((x*a + y*b) + z*c) + d. The previous
// vertex starts as the last input vertex. For each current vertex with distance dc and previous distance dp:
//   dc < 0:  if 0 < dp the intersection point is emitted first; the current vertex is then emitted.
//   dc > 0:  if dp < 0 only the intersection point is emitted.  (dc == 0 or NaN emits nothing.)
// The intersection is (cur - prev) * t + prev per component with t = dp / (dp - dc). Output goes to the first scratch buffer, then
// the second, alternating per plane, each pass reading the previous pass's output. If a pass emits no vertex, *count is set to 0,
// *vertices to 0 and the result is 0. Otherwise (and with n == 0 the input is passed through) *count receives the final vertex count,
// *vertices the vertex pointer, and the result is 1. Nothing bounds the output size.
// Inferred: this is a Sutherland-Hodgman clip of a polygon against the half-spaces d <= 0 kept (negative distance means kept).
// Unknown: the scratch buffer capacity, the meaning of the words at +36 and +44, and the sign convention beyond what is above.
// Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ClipByTop__11VIClipStackiP7VIVect3RiPP7VIVect3(unsigned char* self, int count, const float* in, int* outCount, unsigned* outVertices) {
    const unsigned char* entries = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 8));
    const unsigned char* planes = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 32));
    const int* top = reinterpret_cast<const int*>(entries + (*reinterpret_cast<int*>(self) - 1) * 16);
    int planeCount = top[0];
    const float* plane = reinterpret_cast<const float*>(planes + top[1] * 16);
    float* bufferA = reinterpret_cast<float*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 40)));
    float* bufferB = reinterpret_cast<float*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 48)));
    float* out = bufferA;
    int flip = 1;
    if (planeCount != 0) {
        do {
            int emitted = 0;
            const float* prev = in;
            float prevDistance = 0.0f;
            if (count > 0) {
                prev = in + (count - 1) * 3;
                prevDistance = (prev[0] * plane[0] + prev[1] * plane[1] + prev[2] * plane[2]) + plane[3];
            }
            for (int remaining = count; remaining != 0; --remaining, in += 3) {
                float distance = (in[0] * plane[0] + in[1] * plane[1] + in[2] * plane[2]) + plane[3];
                bool crossing = false;
                bool keep = false;
                if (distance < 0.0f) {
                    keep = true;
                    crossing = 0.0f < prevDistance;
                } else if (0.0f < distance) {
                    crossing = prevDistance < 0.0f;
                }
                if (crossing) {
                    float t = prevDistance / (prevDistance - distance);
                    out[0] = (in[0] - prev[0]) * t + prev[0];
                    out[1] = (in[1] - prev[1]) * t + prev[1];
                    out[2] = (in[2] - prev[2]) * t + prev[2];
                    out += 3;
                    ++emitted;
                }
                if (keep) {
                    out[0] = in[0];
                    out[1] = in[1];
                    out[2] = in[2];
                    out += 3;
                    ++emitted;
                }
                prev = in;
                prevDistance = distance;
            }
            if (emitted == 0) {
                *outCount = 0;
                *outVertices = 0;
                return 0;
            }
            if (flip != 0) {
                in = bufferA;
                out = bufferB;
            } else {
                in = bufferB;
                out = bufferA;
            }
            flip = 1 - flip;
            count = emitted;
            plane += 4;
        } while (--planeCount != 0);
    }
    *outCount = count;
    *outVertices = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(in));
    return 1;
}
}
