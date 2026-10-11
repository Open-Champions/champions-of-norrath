#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" const unsigned char norrath_dtoa_log_estimate_coefficients_view[];

int main() {
    double values[3];
    std::memcpy(values, norrath_dtoa_log_estimate_coefficients_view, sizeof values);
    const double expected[] = {
        0x1.287a7636f4360p-2,
        0x1.68a288b60c8b2p-3,
        0x1.34413509f79fbp-2
    };
    for (int index = 0; index < 3; ++index) {
        if (std::memcmp(values + index, expected + index, sizeof(double)) != 0) return 1;
    }
    std::printf("dtoa coefficient cases passed\n");
    return 0;
}
