// Observed lgCodecEnumerate at 0x002eb280, 52 bytes, and its table g_codecs at
// 0x00447f90, 40 bytes of initialised data (two 20-byte records).
//
// The argument is a record index. Index 0 or 1 returns the address of that record
// and anything else, including a negative index, returns 0.
//
// Record words as stored in the image:
//   record 0: 10, name "PCM", 0x0001f400, 0x140, 0x140
//   record 1: 0x12c, name "GSM", 0x000032c8, 0x140, 0x21
// The first word is what commSystemInit__Fv hands to lgCodecOpen when the name
// equals "GSM", and the name pointer sits at offset 4. The meaning of the last
// three words is not established, so they keep positional names.
extern "C" {
struct LgCodecRecord {
    int identifier;
    const char* name;
    unsigned word_2;
    unsigned word_3;
    unsigned word_4;
};

LgCodecRecord g_codecs[2] = {
    {10, "PCM", 0x0001f400u, 0x140u, 0x140u},
    {0x12c, "GSM", 0x000032c8u, 0x140u, 0x21u},
};

LgCodecRecord* lgCodecEnumerate(int index) {
    if (index < 0 || index > 1) return 0;
    return &g_codecs[index];
}
}
