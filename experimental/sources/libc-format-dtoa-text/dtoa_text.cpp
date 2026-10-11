// Observed dtoa literals at 0x0047f3e0. "Infinity" is eight letters, so a
// caller that stops at a zero fourth byte treats "NaN" as three letters and
// "Infinity" as eight. The zero text's end pointer is one byte past '0'.
extern "C" const unsigned char norrath_dtoa_infinity_text_view[] = "Infinity";
extern "C" const unsigned char norrath_dtoa_nan_text_view[] = "NaN";
extern "C" const unsigned char norrath_dtoa_zero_text_view[] = "0";
