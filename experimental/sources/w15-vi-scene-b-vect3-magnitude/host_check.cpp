// Host check only; not a build input.
#include "magnitude.cpp"
int main() {
    const float a[3] = {3.0f, 4.0f, 12.0f}, b[3] = {0, 0, 0}, c[3] = {-1.0f, 2.0f, -2.0f};
    if (Magnitude__C7VIVect3(a) != 13.0f) return 1;
    if (Magnitude__C7VIVect3(b) != 0.0f) return 2;
    if (Magnitude__C7VIVect3(c) != 3.0f) return 3;
    return 0;
}
