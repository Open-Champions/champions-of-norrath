// Host check only; not a build input.
#include "bound.cpp"
#include <cstdio>
int main() {
    float box[7] = {9, 9, 9, 9, 9, 9, 55};
    float p[3] = {1, 2, 3};
    Bound__6VIBBoxiRC7VIVect3(box, 1, p);
    bool ok = box[0] == 1 && box[1] == 2 && box[2] == 3 && box[3] == 1 && box[4] == 2 && box[5] == 3 && box[6] == 55.0f;
    float q[3] = {-5, 7, 3};
    Bound__6VIBBoxiRC7VIVect3(box, 0, q);
    ok = ok && box[0] == -5 && box[1] == 2 && box[2] == 3 && box[3] == 1 && box[4] == 7 && box[5] == 3;
    float s[3] = {0, 0, 10};
    Bound__6VIBBoxiRC7VIVect3(box, 0, s);
    ok = ok && box[0] == -5 && box[1] == 0 && box[2] == 3 && box[3] == 1 && box[4] == 7 && box[5] == 10;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
