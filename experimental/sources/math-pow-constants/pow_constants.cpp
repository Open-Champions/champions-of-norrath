// Observed: ELF objects bp, dp_h and dp_l at 0x47d3b0, 0x47d3b8 and 0x47d3c0 are
// 8 bytes each. The bytes are the image at those addresses. The float atanf
// neighbor ivln2_l ends at 0x47d434 and two_over_pi begins at 0x47d438, leaving
// four bytes 0x7f7fffff with no symbol of their own. powf's reconstruction reads
// one float there.
// Inferred: the three objects are norrath_animation_math_view_47d3b0/47d3b8/47d3c0.
// The four-byte word is norrath_animation_math_view_47d434.
// Unknown: the original identifier of the unnamed four-byte word, and whether any
// other routine reads past those four bytes.
extern "C" {
unsigned char norrath_animation_math_view_47d3b0[8] = {
    0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0xc0, 0x3f,
};
unsigned char norrath_animation_math_view_47d3b8[8] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x15, 0x3f,
};
unsigned char norrath_animation_math_view_47d3c0[8] = {
    0x00, 0x00, 0x00, 0x00, 0xdc, 0xcf, 0xd1, 0x35,
};
unsigned char norrath_animation_math_view_47d434[4] = {
    0xff, 0xff, 0x7f, 0x7f,
};
}
