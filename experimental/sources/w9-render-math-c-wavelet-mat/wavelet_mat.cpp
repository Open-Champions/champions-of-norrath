// Observed: getWaveletMat__9WaveMakerifP8Matrix33 at 0x00155e48, 192 bytes, is the
// function the Wave Maker constructor calls (jal 0x151420) just before its
// matrix-vector product. This file defines the placeholder norrath_wave_maker_get_wavelet_mat
// that the authored constructor uses; arguments are (object, index, fraction, output).
// The object holds a pointer at byte 1892, and the integer at offset 4 of what it
// points to is the wavelet count n. Wavelet records start at object+76, 36 bytes
// (nine floats) apart. The next record is (index + 1) mod n (signed remainder). For
// record words 0, 1, 3 and 4 the output is cur + (next - cur) * fraction. The output
// receives nine floats: out[0] and out[1] from words 0 and 1, out[3] and out[4] from
// words 3 and 4, out[8] = 1.0 and the other four (2, 5, 6, 7) are zero.
// There is no check for n = 0 in the original; a freshly compiled copy traps on a zero divisor.
// Inferred: a 2D rotation/scale basis interpolated between two stored wavelets.
// Unknown: what the record words 2 and 5..8 hold and the wavelet structure's name.
#if defined(NORRATH_HOST_WAVELET)
extern unsigned char* norrath_wavelet_arena;
#endif

extern "C" void norrath_wave_maker_get_wavelet_mat(unsigned char* object, int index, float fraction, float* out) {
    const unsigned block = *reinterpret_cast<const unsigned*>(object + 1892);
#if defined(NORRATH_HOST_WAVELET)
    const int* count_block = reinterpret_cast<const int*>(norrath_wavelet_arena + block);
#else
    const int* count_block = reinterpret_cast<const int*>(static_cast<__UINTPTR_TYPE__>(block));
#endif
    const int count = count_block[1];
    const float* cur = reinterpret_cast<const float*>(object + 76 + index * 36);
    const float* next = reinterpret_cast<const float*>(object + 76 + ((index + 1) % count) * 36);
    out[0] = cur[0] + (next[0] - cur[0]) * fraction;
    out[1] = cur[1] + (next[1] - cur[1]) * fraction;
    out[3] = cur[3] + (next[3] - cur[3]) * fraction;
    out[4] = cur[4] + (next[4] - cur[4]) * fraction;
    out[8] = 1.0f;
    out[5] = 0.0f;
    out[7] = 0.0f;
    out[6] = 0.0f;
    out[2] = 0.0f;
}
