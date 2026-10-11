// Observed formatter literals clustered at 0x0047ed60. Callers read a fixed
// width, a digit index, or a C string. "Inf" and "NaN" are the three-byte
// spellings used by the formatter, distinct from the longer dtoa spellings.
extern "C" const unsigned char norrath_formatter_space16_view[16] = {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};
extern "C" const unsigned char norrath_formatter_zero16_view[16] = {
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
};
extern "C" const unsigned char norrath_formatter_inf_text_view[] = "Inf";
extern "C" const unsigned char norrath_formatter_nan_text_view[] = "NaN";
extern "C" const unsigned char norrath_formatter_hex_lower_view[] = "0123456789abcdef";
extern "C" const unsigned char norrath_formatter_hex_upper_view[] = "0123456789ABCDEF";
extern "C" const unsigned char norrath_formatter_null_text_view[] = "(null)";
extern "C" const unsigned char norrath_formatter_zero_text_view[] = "0";
extern "C" const unsigned char norrath_formatter_dot_text_view[] = ".";
