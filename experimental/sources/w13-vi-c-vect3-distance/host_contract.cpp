// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "vect3-distance.cpp"

int main() {
    float a[3] = {1.0f, 2.0f, 3.0f}, b[3] = {4.0f, 6.0f, 3.0f};
    if (Distance__C7VIVect3RC7VIVect3(a, b) != 5.0f) return 1;
    if (Distance__C7VIVect3RC7VIVect3(b, a) != 5.0f) return 2;
    if (Distance__C7VIVect3RC7VIVect3(a, a) != 0.0f) return 3;
    float c[3] = {0.0f, 0.0f, 0.0f}, d[3] = {1.0f, 2.0f, 2.0f};
    if (Distance__C7VIVect3RC7VIVect3(c, d) != 3.0f) return 4;
    return 0;
}
