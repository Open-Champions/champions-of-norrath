// Host check only; not a build input.
#include "set_outer_offset.cpp"
#include <cstring>
#include <cmath>
int main() {
    alignas(8) char obj[800]; std::memset(obj, 0, sizeof obj);
    float cases[][3] = {{0, 0, 0}, {1, 0, 0}, {0, 2, 0}, {0, 0, 3}, {-0.0f, 0, 0}, {-0.0f, -0.0f, -0.0f}, {NAN, 0, 0}, {0, 0, NAN}, {4, 5, 6}};
    int expect[] = {0, 1, 1, 1, 0, 0, 1, 1, 1};
    for (int i = 0; i < 9; i++) {
        int before = *(int*)(obj + 716);
        SetOuterOffset__20VIParticleAttributesRC7VIVect3(obj, cases[i]);
        if (*(int*)(obj + 692) != expect[i]) return 1 + i;
        if (std::memcmp(obj + 636, cases[i], 12) != 0) return 20 + i;
        if (*(int*)(obj + 716) != before + 1) return 40 + i;
    }
    return 0;
}
