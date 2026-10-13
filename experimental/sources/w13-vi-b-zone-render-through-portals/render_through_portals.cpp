// Observed: RenderThroughPortals__6VIZoneiR15VISceneRendVars at 0x011166e0, 628 bytes. With
// Z the zone (this), `room` the int argument and V the render-vars argument:
//  - R = the 120-byte room record at index `room` of the array at Z word +96. If R word +76
//    (the portal count n) is not positive, return 0. Portal records are 32 bytes each at the
//    array at R word +92: floats a, b, c, d at +0..+12, vertex count at +20, target room at +24,
//    flags at +28. The portals' vertices are consecutive 12-byte entries starting at R word +96.
//  - For each portal (the vertex pointer advances by 12 * count after every portal, taken or
//    skipped; n is re-read from R each time): skip it if flag bit 0 is set; compute
//    dist = dot(eye, abc) + d with the eye at (V word +12) + 28 and skip the portal if dist < 0;
//    let near = dist < 0.02f.
//  - If not near, ClipByTop__11VIClipStackiP7VIVect3RiPP7VIVect3(V word +20, count, vertices,
//    &outCount, &outVertices); a zero result skips the portal.
//  - Let T be the target room's record. If T word +0 has bit 0 clear: when not near,
//    PushPortal__11VIClipStackR8VICameraiP7VIVect3(clip, camera, outCount, outVertices) (a
//    negative result returns -1) and GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane(clip, V+24,
//    V+28, V+32, V+36); then set bit 0 of the portal's flags, call
//    RenderRoom__6VIZoneiR15VISceneRendVars(Z, target, V) (negative returns -1), when not near
//    Pop__11VIClipStack(clip) (negative returns -1) followed by the same GetTop call, and clear
//    bit 0 of the flags again (the portal array pointer is re-read before each flag update).
//  - If T word +0 has bit 0 set: when V word +40 is zero, call
//    RenderByFrustum__7VIWorldR15VISceneRendVars(V word +8, V) and return -1 if it returns nonzero.
//  - After the last portal return 0.
// Inferred: a portal-based visibility walk: flag bit 0 on a portal marks it as being on the
// current path (preventing cycles), the clip stack narrows the view through each portal the eye
// is not touching (dist >= 0.02), and rooms whose own bit 0 is set fall back to a frustum test
// of the world.
// Unknown: the member names, the meaning of bit 0 in the room record, the return types of the
// callees (int assumed), and the role of V word +40.
#if defined(NORRATH_HOST_CONTRACT)
// On the 64-bit host a pointer field is modelled as a 32-bit offset into an arena.
extern "C" unsigned char* norrath_host_load_pointer(const void* field);
#define NORRATH_LOAD_POINTER(field) norrath_host_load_pointer(field)
#else
#define NORRATH_LOAD_POINTER(field) (*reinterpret_cast<unsigned char* const*>(field))
#endif

extern "C" {
int ClipByTop__11VIClipStackiP7VIVect3RiPP7VIVect3(void* clipStack, int count, void* vertices, int* outCount, void** outVertices);
int PushPortal__11VIClipStackR8VICameraiP7VIVect3(void* clipStack, void* camera, int count, void* vertices);
int GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane(void* clipStack, int* a, void** b, int* c, void** d);
int RenderRoom__6VIZoneiR15VISceneRendVars(void* zone, int room, void* vars);
int Pop__11VIClipStack(void* clipStack);
int RenderByFrustum__7VIWorldR15VISceneRendVars(void* world, void* vars);

int RenderThroughPortals__6VIZoneiR15VISceneRendVars(void* zone, int roomIndex, void* vars) {
    unsigned char* z = static_cast<unsigned char*>(zone);
    unsigned char* v = static_cast<unsigned char*>(vars);
    unsigned char* room = NORRATH_LOAD_POINTER(z + 96) + roomIndex * 120;
    if (*reinterpret_cast<int*>(room + 76) <= 0) return 0;
    unsigned char* vertices = NORRATH_LOAD_POINTER(room + 96);
    int i = 0;
    do {
        unsigned char* portal = NORRATH_LOAD_POINTER(room + 92) + i * 32;
        int flags = *reinterpret_cast<int*>(portal + 28);
        int count = *reinterpret_cast<int*>(portal + 20);
        int target = *reinterpret_cast<int*>(portal + 24);
        do {
            if (flags & 1) break;
            const float* eye = reinterpret_cast<const float*>(NORRATH_LOAD_POINTER(v + 12) + 28);
            const float* plane = reinterpret_cast<const float*>(portal);
            float distance = eye[0] * plane[0] + eye[1] * plane[1] + eye[2] * plane[2] + plane[3];
            if (distance < 0.0f) break;
            bool near = distance < 0.02f;
            int outCount;
            void* outVertices;
            if (!near) {
                if (ClipByTop__11VIClipStackiP7VIVect3RiPP7VIVect3(NORRATH_LOAD_POINTER(v + 20), count, vertices, &outCount, &outVertices) == 0) break;
            }
            unsigned char* targetRoom = NORRATH_LOAD_POINTER(z + 96) + target * 120;
            if ((*reinterpret_cast<int*>(targetRoom) ^ 1) & 1) {
                if (!near) {
                    if (PushPortal__11VIClipStackR8VICameraiP7VIVect3(NORRATH_LOAD_POINTER(v + 20), NORRATH_LOAD_POINTER(v + 12), outCount, outVertices) < 0) return -1;
                    GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane(NORRATH_LOAD_POINTER(v + 20), reinterpret_cast<int*>(v + 24), reinterpret_cast<void**>(v + 28), reinterpret_cast<int*>(v + 32), reinterpret_cast<void**>(v + 36));
                }
                portal = NORRATH_LOAD_POINTER(room + 92) + i * 32;
                *reinterpret_cast<int*>(portal + 28) |= 1;
                if (RenderRoom__6VIZoneiR15VISceneRendVars(zone, target, vars) < 0) return -1;
                if (!near) {
                    if (Pop__11VIClipStack(NORRATH_LOAD_POINTER(v + 20)) < 0) return -1;
                    GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane(NORRATH_LOAD_POINTER(v + 20), reinterpret_cast<int*>(v + 24), reinterpret_cast<void**>(v + 28), reinterpret_cast<int*>(v + 32), reinterpret_cast<void**>(v + 36));
                }
                portal = NORRATH_LOAD_POINTER(room + 92) + i * 32;
                *reinterpret_cast<int*>(portal + 28) &= ~1;
            } else {
                if (*reinterpret_cast<int*>(v + 40) == 0 && RenderByFrustum__7VIWorldR15VISceneRendVars(NORRATH_LOAD_POINTER(v + 8), vars) != 0) return -1;
            }
        } while (false);
        vertices += count * 12;
        ++i;
    } while (i < *reinterpret_cast<int*>(room + 76));
    return 0;
}
}
