// Observed: _8VIString$ConversionTable1252 at 0x0118e658 is 64 bytes: 32 little-endian 16-bit
// values. str8to16 indexes it with (byte - 0x80) for input bytes 0x80..0x9f and uses the byte
// itself for every other value. Entries, in index order (0x80 first), are listed below.
// Inferred: the Windows-1252 style upgrade of the 0x80..0x9f range to 16-bit units. Most entries
// match the published Windows-1252 mapping; where that mapping has no character the table holds
// 0xfffe. Two entries differ from the published mapping: index 2 (byte 0x82) holds 0x20a1 and index
// 30 (byte 0x9e) holds 0xfffe. They are kept as observed.
// Unknown: whether the data is read-only in the original (it is only read by observed code); the
// section it lives in is not established from the image, so const is an assumption.
extern "C" {
extern const unsigned short _8VIString$ConversionTable1252[32];
const unsigned short _8VIString$ConversionTable1252[32] = {
    0xfffe, 0xfffe, 0x20a1, 0x0192, 0x201e, 0x2026, 0x2020, 0x2021,
    0x02c6, 0x2030, 0x0160, 0x2039, 0x0152, 0xfffe, 0xfffe, 0xfffe,
    0xfffe, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
    0x02dc, 0x2122, 0x0161, 0x203a, 0x0153, 0xfffe, 0xfffe, 0x0178,
};
}
