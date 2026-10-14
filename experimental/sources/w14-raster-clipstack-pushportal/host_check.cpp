// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
// The plane builder is stubbed: it records its three point arguments and writes the plane (n, n + 1, n + 2, n + 3) for call n.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "push_portal.cpp"
#include <cstdio>
static int calls;
static float seen[4][9];
extern "C" int __cl__7VIPlaneRC7VIVect3N21(float* plane, const float* p0, const float* p1, const float* p2) {
    for (int i = 0; i < 3; ++i) { seen[calls][i] = p0[i]; seen[calls][3 + i] = p1[i]; seen[calls][6 + i] = p2[i]; }
    for (int i = 0; i < 4; ++i) plane[i] = 100.0f * (calls + 1) + i;
    ++calls;
    return 1;
}
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
static float& F(unsigned off) { return *reinterpret_cast<float*>(arena + off); }
int main() {
    // stack at 0 with one existing entry, 2 planes and 3 vectors already used; arrays at 0x100 (entries), 0x200 (vectors), 0x400 (planes)
    W(0) = 1; W(8) = arena_base + 0x100; W(12) = 3; W(20) = arena_base + 0x200; W(24) = 2; W(32) = arena_base + 0x400;
    unsigned char* camera = arena + 0x1000;
    float* cf = reinterpret_cast<float*>(camera);
    cf[1] = 4.0f;                                 // +4: distance
    cf[2] = 0.0f; cf[3] = 0.0f; cf[4] = 1.0f;      // +8..+16: axis (0, 0, 1)
    cf[7] = 1.0f; cf[8] = 2.0f; cf[9] = 3.0f;      // +28: eye
    cf[104] = 0.5f; cf[105] = 0.25f; cf[106] = 0.125f; cf[107] = -2.0f;   // +416: camera plane
    float verts[9] = {2, 2, 7, 4, 2, 7, 4, 6, 7};  // z - eye.z = 4 for all
    int r = PushPortal__11VIClipStackR8VICameraiP7VIVect3(arena, camera, 3, verts);
    bool ok = r == 1 && W(0) == 2 && W(12) == 7 && W(24) == 6;
    ok = ok && W(0x110) == 4 && W(0x114) == 2 && W(0x118) == 4 && W(0x11c) == 3;    // entry 1: 4 planes from 2, 4 vectors from 3
    ok = ok && F(0x400 + 32) == 0.5f && F(0x400 + 44) == -2.0f;                       // camera plane at index 2
    ok = ok && F(0x400 + 48) == 100.0f && F(0x400 + 60) == 103.0f && F(0x400 + 64) == 200.0f && F(0x400 + 80 + 12) == 303.0f;
    ok = ok && F(0x200 + 36) == 1.0f && F(0x200 + 40) == 2.0f && F(0x200 + 44) == 3.0f;   // eye as vector 3
    // vertex 0: d = (1, 0, 4); dot = 4; scale = 4/4 = 1; vector = d*1 + eye = (2, 2, 7)
    ok = ok && F(0x200 + 48) == 2.0f && F(0x200 + 52) == 2.0f && F(0x200 + 56) == 7.0f;
    // vertex 2: d = (3, 4, 4); scale = 1 -> (4, 6, 7)
    ok = ok && F(0x200 + 72) == 4.0f && F(0x200 + 76) == 6.0f && F(0x200 + 80) == 7.0f;
    // the plane builder saw (previous, current, eye): first call previous is the last vertex
    ok = ok && calls == 3 && seen[0][0] == 4 && seen[0][1] == 6 && seen[0][3] == 2 && seen[0][4] == 2 && seen[0][6] == 1 &&
         seen[1][0] == 2 && seen[1][3] == 4 && seen[2][0] == 4 && seen[2][1] == 2 && seen[2][3] == 4 && seen[2][4] == 6;
    // a second portal with no vertices pushes only the camera plane and the eye
    r = PushPortal__11VIClipStackR8VICameraiP7VIVect3(arena, camera, 0, verts);
    ok = ok && r == 2 && W(0) == 3 && W(0x120) == 1 && W(0x128) == 1 && W(24) == 7 && W(12) == 8 && calls == 3;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
