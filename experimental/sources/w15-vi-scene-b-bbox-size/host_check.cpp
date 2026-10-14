// Host check only; not a build input.
#include "size.cpp"
int main() {
    const float box[6] = {-2, 1, 4, 6, 10, 5.5f};
    float out[4] = {9, 9, 9, 9};
    if (Size__C6VIBBox(out, box) != out) return 1;
    if (out[0] != 8.0f || out[1] != 9.0f || out[2] != 1.5f || out[3] != 9.0f) return 2;
    return 0;
}
