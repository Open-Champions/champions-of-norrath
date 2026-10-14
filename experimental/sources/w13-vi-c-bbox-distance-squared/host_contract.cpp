// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "bbox-distance-squared.cpp"

int main() {
    float box[6] = {-1.0f, -2.0f, -3.0f, 1.0f, 2.0f, 3.0f};
    float inside[3] = {0.5f, 0.0f, -2.0f};
    if (DistanceSquared__C6VIBBoxRC7VIVect3(box, inside) != 0.0f) return 1;
    float face[3] = {4.0f, 0.0f, 0.0f};
    if (DistanceSquared__C6VIBBoxRC7VIVect3(box, face) != 9.0f) return 2;
    float corner[3] = {-3.0f, 5.0f, 3.0f + 4.0f};
    if (DistanceSquared__C6VIBBoxRC7VIVect3(box, corner) != 4.0f + 9.0f + 16.0f) return 3;
    float onedge[3] = {1.0f, 2.0f, -3.0f};
    if (DistanceSquared__C6VIBBoxRC7VIVect3(box, onedge) != 0.0f) return 4;
    return 0;
}
