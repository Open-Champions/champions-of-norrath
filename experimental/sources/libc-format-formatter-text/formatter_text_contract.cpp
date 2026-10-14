#include <cstdio>
#include <cstring>

extern "C" const unsigned char norrath_formatter_space16_view[];
extern "C" const unsigned char norrath_formatter_zero16_view[];
extern "C" const unsigned char norrath_formatter_inf_text_view[];
extern "C" const unsigned char norrath_formatter_nan_text_view[];
extern "C" const unsigned char norrath_formatter_hex_lower_view[];
extern "C" const unsigned char norrath_formatter_hex_upper_view[];
extern "C" const unsigned char norrath_formatter_null_text_view[];
extern "C" const unsigned char norrath_formatter_zero_text_view[];
extern "C" const unsigned char norrath_formatter_dot_text_view[];

int main() {
    for (int i = 0; i < 16; ++i) {
        if (norrath_formatter_space16_view[i] != ' ') return 1;
        if (norrath_formatter_zero16_view[i] != '0') return 2;
    }
    if (std::memcmp(norrath_formatter_inf_text_view, "Inf", 4) != 0) return 3;
    if (std::memcmp(norrath_formatter_nan_text_view, "NaN", 4) != 0) return 4;
    if (std::memcmp(norrath_formatter_hex_lower_view, "0123456789abcdef", 17) != 0) return 5;
    if (std::memcmp(norrath_formatter_hex_upper_view, "0123456789ABCDEF", 17) != 0) return 6;
    if (std::strcmp(reinterpret_cast<const char*>(norrath_formatter_null_text_view), "(null)") != 0) return 7;
    if (norrath_formatter_zero_text_view[0] != '0' || norrath_formatter_zero_text_view[1] != 0) return 8;
    if (norrath_formatter_dot_text_view[0] != '.' || norrath_formatter_dot_text_view[1] != 0) return 9;
    if (norrath_formatter_hex_lower_view[10] != 'a' || norrath_formatter_hex_upper_view[15] != 'F') return 10;
    std::printf("formatter text cases passed\n");
    return 0;
}
