// Observed defaultTLB at 0x00445e60, 288 bytes (18 entries of four words), pinned SLUS_205.65: a
// file-local initialised data object. TLBInfo (0x00446000) points at it as its second table, with the
// count 18 at the matching count word; InitTLB32MB hands each entry, word by word, to
// _SetTLBEntry(index, word0, word1, word2, word3).
// Inferred only: the four words follow the layout of an R5900 TLB entry (page mask, entry-high,
// entry-lo0, entry-lo1). The bit fields were not decoded further.
// The words are copied from the executable as data; there is no code in this unit.
extern "C" {
struct Tlb32Entry {
    unsigned word0, word1, word2, word3;
};

Tlb32Entry defaultTLB[18] = {
    {0x0007e000, 0x00080000, 0x0000201f, 0x0000301f},
    {0x0007e000, 0x00100000, 0x0000401f, 0x0000501f},
    {0x0007e000, 0x00180000, 0x0000601f, 0x0000701f},
    {0x001fe000, 0x00200000, 0x0000801f, 0x0000c01f},
    {0x001fe000, 0x00400000, 0x0001001f, 0x0001401f},
    {0x001fe000, 0x00600000, 0x0001801f, 0x0001c01f},
    {0x007fe000, 0x00800000, 0x0002001f, 0x0003001f},
    {0x007fe000, 0x01000000, 0x0004001f, 0x0005001f},
    {0x007fe000, 0x01800000, 0x0006001f, 0x0007001f},
    {0x0007e000, 0x20080000, 0x00002017, 0x00003017},
    {0x0007e000, 0x20100000, 0x00004017, 0x00005017},
    {0x0007e000, 0x20180000, 0x00006017, 0x00007017},
    {0x001fe000, 0x20200000, 0x00008017, 0x0000c017},
    {0x001fe000, 0x20400000, 0x00010017, 0x00014017},
    {0x001fe000, 0x20600000, 0x00018017, 0x0001c017},
    {0x007fe000, 0x20800000, 0x00020017, 0x00030017},
    {0x007fe000, 0x21000000, 0x00040017, 0x00050017},
    {0x007fe000, 0x21800000, 0x00060017, 0x00070017}
};
}
