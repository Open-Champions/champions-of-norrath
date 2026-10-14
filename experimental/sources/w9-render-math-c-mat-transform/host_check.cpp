// Host contract for norrath_mat_transform. Not an EE plan input.
#include <cstdio>
extern "C" void norrath_mat_transform(float*, const float*, const float*);
int main() {
    const float a[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    float in[3] = {1, 10, 100};
    float out[3];
    norrath_mat_transform(out, a, in);
    if (out[0] != 1 + 40 + 700 || out[1] != 2 + 50 + 800 || out[2] != 3 + 60 + 900) return 1;
    norrath_mat_transform(in, a, in);   // output aliasing the input
    if (in[0] != out[0] || in[1] != out[1] || in[2] != out[2]) return 2;
    std::printf("mat transform contract passed\n");
    return 0;
}
