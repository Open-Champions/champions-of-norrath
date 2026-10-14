// Observed: PushPortal__11VIClipStackR8VICameraiP7VIVect3 at 0x01098730 is 712 bytes in the pinned SLUS-20565 image. Arguments: the
// stack (this), the camera, a vertex count and a pointer to 12-byte portal vertices. Stack layout as in w12-vimath-a: entry count
// at +0, vector total at +12, plane total at +24, pointer to 16-byte entries at +8, pointer to 12-byte vectors at +20, pointer to
// 16-byte planes at +32. The routine appends one entry at index count and fills it:
//   entry word +4 = plane total, entry word +12 = vector total (both taken before anything is added);
//   plane total slot: the camera's four floats at camera +416 (the first frustum plane) are appended, then the plane total is
//   incremented; vector slot: the camera's three floats at camera +28 (the eye) are appended, then the vector total is incremented.
// Then for each vertex i (previous vertex = vertex i-1, the last one for i = 0), in order:
//   a plane is built by __cl__7VIPlaneRC7VIVect3N21 (a four-float local) from (previous vertex, vertex i, eye) and appended;
//   with d = vertex i - eye, a vector is appended equal to d * (camera float at +4 / dot(d, camera floats at +8..+16)) + eye,
//   computed as stated: dot = (d.x*c8 + d.y*c12) + d.z*c16, scale = camera[+4] / dot, each component of d multiplied by scale and
//   then the eye added.
// Finally entry word +0 = plane total - entry word +4, entry word +8 = vector total - entry word +12, the entry count is incremented
// and the previous entry count (before the increment) is returned. With a count of zero or less only the camera plane and eye are
// pushed. The plane builder's return value is ignored. Capacity of the arrays is not checked.
// Inferred: pushes a portal (visible window) onto the clip stack as the near-plane camera plane plus one side plane per portal edge,
// together with the points where each edge ray meets the camera plane at distance camera[+4].
// Unknown: camera field names; why a vector is recorded per plane; array capacities. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int __cl__7VIPlaneRC7VIVect3N21(float* plane, const float* p0, const float* p1, const float* p2);

int PushPortal__11VIClipStackR8VICameraiP7VIVect3(unsigned char* self, const unsigned char* camera, int count, const float* vertices) {
    int* s = reinterpret_cast<int*>(self);
    const float* cameraPlane = reinterpret_cast<const float*>(camera + 416);
    const float* eye = reinterpret_cast<const float*>(camera + 28);
    const float* axis = reinterpret_cast<const float*>(camera + 8);
    int oldCount = s[0];
    int* entry = reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(s[2]) + oldCount * 16);
    float edge[3];
    float plane[4];
    const float* previous = vertices + (count - 1) * 3;
    entry[1] = s[6];
    entry[3] = s[3];
    {
        float* slot = reinterpret_cast<float*>(NORRATH_WORD_TO_POINTER(s[8]) + s[6] * 16);
        ++s[6];
        slot[0] = cameraPlane[0];
        slot[1] = cameraPlane[1];
        slot[2] = cameraPlane[2];
        slot[3] = cameraPlane[3];
        float* vector = reinterpret_cast<float*>(NORRATH_WORD_TO_POINTER(s[5]) + s[3] * 12);
        ++s[3];
        vector[0] = eye[0];
        vector[1] = eye[1];
        vector[2] = eye[2];
    }
    const float* current = vertices;
    for (int remaining = count; remaining > 0; --remaining) {
        __cl__7VIPlaneRC7VIVect3N21(plane, previous, current, eye);
        previous = current;
        {
            float* slot = reinterpret_cast<float*>(NORRATH_WORD_TO_POINTER(s[8]) + s[6] * 16);
            ++s[6];
            slot[0] = plane[0];
            slot[1] = plane[1];
            slot[2] = plane[2];
            slot[3] = plane[3];
        }
        float* vector = reinterpret_cast<float*>(NORRATH_WORD_TO_POINTER(s[5]) + s[3] * 12);
        edge[0] = current[0] - eye[0];
        edge[1] = current[1] - eye[1];
        edge[2] = current[2] - eye[2];
        current += 3;
        ++s[3];
        float dot = edge[0] * axis[0] + edge[1] * axis[1] + edge[2] * axis[2];
        float scale = *reinterpret_cast<const float*>(camera + 4) / dot;
        vector[0] = edge[0] * scale + eye[0];
        vector[1] = edge[1] * scale + eye[1];
        vector[2] = edge[2] * scale + eye[2];
    }
    entry[0] = s[6] - entry[1];
    entry[2] = s[3] - entry[3];
    s[0] = oldCount + 1;
    return oldCount;
}
}
