// Host check only; not a build input.
#include "point_in_solid.cpp"
int main() {
    const float sphere[4] = {1, 2, 3, 5};
    const float centre[3] = {1, 2, 3}, edge[3] = {4, 6, 3}, outside[3] = {4, 6, 3.5f}, far[3] = {-10, 2, 3};
    if (!PointInSolid__C8VISphereRC7VIVect3(sphere, centre)) return 1;
    if (!PointInSolid__C8VISphereRC7VIVect3(sphere, edge)) return 2;
    if (PointInSolid__C8VISphereRC7VIVect3(sphere, outside)) return 3;
    if (PointInSolid__C8VISphereRC7VIVect3(sphere, far)) return 4;
    return 0;
}
