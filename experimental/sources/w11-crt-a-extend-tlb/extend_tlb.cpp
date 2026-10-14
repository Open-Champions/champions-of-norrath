// Observed extendTLB at 0x00445f80, 128 bytes (8 entries of four words), pinned SLUS_205.65: a
// file-local initialised data object. TLBInfo (0x00446000) points at it as its third table, with the
// count 8 at the matching count word; InitTLB32MB hands each entry, word by word, to
// _SetTLBEntry(index, word0, word1, word2, word3).
// Inferred only: the four words follow the layout of an R5900 TLB entry (page mask, entry-high,
// entry-lo0, entry-lo1). The bit fields were not decoded further.
// The words are copied from the executable as data; there is no code in this unit.
extern "C" {
struct Tlb32Entry {
    unsigned word0, word1, word2, word3;
};

Tlb32Entry extendTLB[8] = {
    {0x0007e000, 0x30100000, 0x0000403f, 0x0000503f},
    {0x0007e000, 0x30180000, 0x0000603f, 0x0000703f},
    {0x001fe000, 0x30200000, 0x0000803f, 0x0000c03f},
    {0x001fe000, 0x30400000, 0x0001003f, 0x0001403f},
    {0x001fe000, 0x30600000, 0x0001803f, 0x0001c03f},
    {0x007fe000, 0x30800000, 0x0002003f, 0x0003003f},
    {0x007fe000, 0x31000000, 0x0004003f, 0x0005003f},
    {0x007fe000, 0x31800000, 0x0006003f, 0x0007003f}
};
}
