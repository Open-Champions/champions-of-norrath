// Observed basefix.0 at 0x47ef70, 34 bytes in .rodata: seventeen signed
// halfwords 10, 1, 2, ..., 16. The scanf integer path indexes this table
// by the current base when the next digit is 1 through 7, or 8/9 with a
// further range check: base 0 becomes 10, bases 1 through 16 stay
// themselves, so %i input starting with a nonzero digit scans as decimal
// while octal and hexadecimal bases are preserved. Bytes below are the
// image at that address, little-endian.
// Observed "e%ld" with its NUL at 0x47ef98, 5 bytes in .rodata. The scanf
// float path rewrites an overflowing exponent field through sprintf with
// this format and the computed long correction. Six zero pad bytes sit
// between the two objects, and the format is followed by three zero pad
// bytes before an unrelated pointer table.
// Inferred: they are norrath_player_scan_base_normalization_view and
// norrath_player_scan_exponent_format_view.
// Unknown: placement at the original addresses and section identity with
// .rodata are not claimed by these relocatable objects.
extern "C" const unsigned char norrath_player_scan_base_normalization_view[34] = {
    0x0a, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00,
    0x06, 0x00, 0x07, 0x00, 0x08, 0x00, 0x09, 0x00, 0x0a, 0x00, 0x0b, 0x00,
    0x0c, 0x00, 0x0d, 0x00, 0x0e, 0x00, 0x0f, 0x00, 0x10, 0x00,
};
extern "C" const unsigned char norrath_player_scan_exponent_format_view[5] = {
    0x65, 0x25, 0x6c, 0x64, 0x00,
};
