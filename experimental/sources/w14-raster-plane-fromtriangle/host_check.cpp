// Host check only; not a build input. Normalize is stubbed: it records its argument and scales by 1/2 when "ok".
#include "from_triangle.cpp"
#include <cstdio>
static int stub_result, calls;
static float seen[3];
extern "C" int Normalize__7VIVect3(float* v) {
    ++calls;
    for (int i = 0; i < 3; ++i) { seen[i] = v[i]; if (stub_result) v[i] *= 0.5f; }
    return stub_result;
}
int main() {
    float p0[3] = {1, 0, 2}, p1[3] = {2, 0, 2}, p2[3] = {2, 3, 2};
    float plane[5] = {7, 7, 7, 7, 55};
    stub_result = 1;
    int r = __cl__7VIPlaneRC7VIVect3N21(plane, p0, p1, p2);
    // e1 = (1,0,0), e2 = (0,3,0); e2 x e1 = (0*0-0*0, 0*1-0*0, 0*0-3*1) = (0,0,-3); halved by the stub
    bool ok = r == 1 && calls == 1 && seen[0] == 0.0f && seen[1] == 0.0f && seen[2] == -3.0f &&
              plane[0] == 0.0f && plane[1] == 0.0f && plane[2] == -1.5f && plane[4] == 55.0f;
    // d = -(0*1 + 0*0 + -1.5*2) = 3
    ok = ok && plane[3] == 3.0f;
    stub_result = 0;
    r = __cl__7VIPlaneRC7VIVect3N21(plane, p0, p1, p2);
    ok = ok && r == 0 && plane[2] == -3.0f && plane[3] == 0.0f && calls == 2;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
