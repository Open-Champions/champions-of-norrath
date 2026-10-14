// Host contract for getWaveletMat__9WaveMakerifP8Matrix33; not an EE input. Pointer words are offsets from a
// host base. The expectation is written separately from the source and compared bit for bit.
#define W11_ACTOR_C_HOST 1
#include "get_wavelet_mat.cpp"
#include <cstdio>
#include <cstring>
unsigned char* w11_actor_c_host_base;
W11ActorCHostSlot w11_actor_c_host_slots[1];
int main() {
    static unsigned char base[8192];
    w11_actor_c_host_base = base;
    int failures = 0;
    unsigned char* obj = base + 64;
    const int counts[] = {1, 2, 3, 5, 0};
    const float ts[] = {0.0f, 1.0f, 0.25f, -0.5f, 3.0f};
    for (int count : counts) {
        std::memset(base, 0, sizeof base);
        *reinterpret_cast<unsigned*>(obj + 1892) = 4096;
        *reinterpret_cast<int*>(base + 4096 + 4) = count;
        for (int e = 0; e < 8; ++e)
            for (int k = 0; k < 9; ++k) *reinterpret_cast<float*>(obj + 76 + 36 * e + 4 * k) = 1.5f * e + 0.37f * k - 2.0f;
        for (int i = 0; i < (count == 0 ? 3 : count); ++i) {
            for (float t : ts) {
                float out[9];
                std::memset(out, 0x7f, sizeof out);
                getWaveletMat__9WaveMakerifP8Matrix33(obj, i, t, out);
                const int j = count == 0 ? i + 1 : (i + 1) % count;
                float want[9] = {0, 0, 0, 0, 0, 0, 0, 0, 1.0f};
                const int pos[4] = {0, 1, 3, 4};
                for (int p : pos) {
                    float a = 1.5f * i + 0.37f * p - 2.0f, b = 1.5f * j + 0.37f * p - 2.0f;
                    want[p] = a + (b - a) * t;
                }
                if (std::memcmp(out, want, sizeof out) != 0) {
                    std::fprintf(stderr, "count=%d i=%d t=%g differs\n", count, i, t);
                    ++failures;
                }
            }
        }
    }
    if (!failures) std::puts("getWaveletMat ok");
    return failures != 0;
}
