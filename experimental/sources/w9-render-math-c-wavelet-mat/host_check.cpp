// Host contract for norrath_wave_maker_get_wavelet_mat. Not an EE plan input.
// Build with -DNORRATH_HOST_WAVELET: the stored 32-bit address is an offset into an arena.
#include <cstdio>
alignas(8) unsigned char arena[64];
unsigned char* norrath_wavelet_arena = arena;
extern "C" void norrath_wave_maker_get_wavelet_mat(unsigned char*, int, float, float*);
int main() {
    alignas(8) static unsigned char obj[2048];
    *reinterpret_cast<int*>(arena + 4) = 3;
    *reinterpret_cast<unsigned*>(obj + 1892) = 0u;
    auto rec = [&](int i) { return reinterpret_cast<float*>(obj + 76 + i * 36); };
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 9; ++j) rec(i)[j] = float(10 * i + j);
    float out[9];
    for (float& f : out) f = -5.0f;
    norrath_wave_maker_get_wavelet_mat(obj, 1, 0.5f, out);
    if (out[0] != 15.0f || out[1] != 16.0f || out[3] != 18.0f || out[4] != 19.0f) return 1;
    if (out[8] != 1.0f || out[2] != 0 || out[5] != 0 || out[6] != 0 || out[7] != 0) return 2;
    norrath_wave_maker_get_wavelet_mat(obj, 2, 0.5f, out);   // wraps to record 0
    if (out[0] != 10.0f || out[4] != 14.0f) return 3;
    std::printf("wavelet mat contract passed\n");
    return 0;
}
