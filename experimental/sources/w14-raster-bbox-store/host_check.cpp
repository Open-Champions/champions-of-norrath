// Host check only; not a build input.
#include "store.cpp"
#include <cstdio>
int main() {
    float box[6] = {1, 2, 3, 10, 20, 30};   // min (1,2,3), max (10,20,30)
    float out[25];
    for (int i = 0; i < 25; ++i) out[i] = -1.0f;
    static const float want[24] = {1, 2, 3,   10, 2, 3,   10, 2, 30,   1, 2, 30,
                                   1, 20, 3,  10, 20, 3,  10, 20, 30,  1, 20, 30};
    bool ok = Store__C6VIBBoxP7VIVect3(box, out) == 0 && out[24] == -1.0f;
    for (int i = 0; i < 24; ++i) ok = ok && out[i] == want[i];
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
