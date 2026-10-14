// Host check only; not a build input.
#include "contains.cpp"
int main() {
    const float box[6] = {0, 0, 0, 2, 4, 6};
    const float inside[3] = {1, 2, 3}, corner[3] = {2, 4, 6}, origin[3] = {0, 0, 0};
    if (!Contains__C6VIBBoxRC7VIVect3(box, inside)) return 1;
    if (!Contains__C6VIBBoxRC7VIVect3(box, corner)) return 2;
    if (!Contains__C6VIBBoxRC7VIVect3(box, origin)) return 3;
    for (int lane = 0; lane < 3; ++lane) {
        float low[3] = {1, 2, 3}, high[3] = {1, 2, 3};
        low[lane] = -0.5f;
        high[lane] = 7.0f;
        if (Contains__C6VIBBoxRC7VIVect3(box, low)) return 4 + lane;
        if (Contains__C6VIBBoxRC7VIVect3(box, high)) return 7 + lane;
    }
    return 0;
}
