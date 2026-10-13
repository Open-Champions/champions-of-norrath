// Host check only; not a build input. Magnitude is stubbed: it records the vector and returns 42.
#include "init.cpp"
#include <cstdio>
static float seen[3];
static int calls;
extern "C" float Magnitude__C7VIVect3(const float* v) { ++calls; seen[0] = v[0]; seen[1] = v[1]; seen[2] = v[2]; return 42.0f; }
int main() {
    float box[6] = {-2, 0, 4, 6, 10, 8};
    float sphere[5] = {9, 9, 9, 9, 55};
    int r = Init__8VISphereRC6VIBBox(sphere, box);
    bool ok = r == 0 && calls == 1 && sphere[0] == 2.0f && sphere[1] == 5.0f && sphere[2] == 6.0f && sphere[3] == 42.0f && sphere[4] == 55.0f &&
              seen[0] == 4.0f && seen[1] == 5.0f && seen[2] == 2.0f;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
