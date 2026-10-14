#include <cstdio>
#include <cstring>

extern "C" const unsigned char norrath_dtoa_infinity_text_view[];
extern "C" const unsigned char norrath_dtoa_nan_text_view[];
extern "C" const unsigned char norrath_dtoa_zero_text_view[];

int main() {
    if (std::memcmp(norrath_dtoa_infinity_text_view, "Infinity", 9) != 0) return 1;
    if (norrath_dtoa_infinity_text_view[3] == 0) return 2;
    if (std::memcmp(norrath_dtoa_nan_text_view, "NaN", 4) != 0) return 3;
    if (norrath_dtoa_nan_text_view[3] != 0) return 4;
    if (norrath_dtoa_zero_text_view[0] != '0' || norrath_dtoa_zero_text_view[1] != 0) return 5;
    std::printf("dtoa text cases passed\n");
    return 0;
}
