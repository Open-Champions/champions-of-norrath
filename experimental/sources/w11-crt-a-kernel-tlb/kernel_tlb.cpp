// Observed kernelTLB at 0x00445d90, 208 bytes (13 entries of four words), pinned SLUS_205.65: a
// file-local initialised data object. TLBInfo (0x00446000) points at it as its first table, with the
// count 13 at the matching count word; InitTLB32MB hands each entry, word by word, to
// _SetTLBEntry(index, word0, word1, word2, word3).
// Inferred only: the four words follow the layout of an R5900 TLB entry (page mask, entry-high,
// entry-lo0, entry-lo1). The bit fields were not decoded further.
// The words are copied from the executable as data; there is no code in this unit.
extern "C" {
struct Tlb32Entry {
    unsigned word0, word1, word2, word3;
};

Tlb32Entry kernelTLB[13] = {
    {0x00000000, 0x70000000, 0x80000007, 0x00000007},
    {0x00006000, 0xffff8000, 0x00001e1f, 0x00001f1f},
    {0x00000000, 0x10000000, 0x00400017, 0x00400053},
    {0x00000000, 0x10002000, 0x00400097, 0x004000d7},
    {0x00000000, 0x10004000, 0x00400117, 0x00400157},
    {0x00000000, 0x10006000, 0x00400197, 0x004001d7},
    {0x00000000, 0x10008000, 0x00400217, 0x00400257},
    {0x00000000, 0x1000a000, 0x00400297, 0x004002d7},
    {0x00000000, 0x1000c000, 0x00400313, 0x00400357},
    {0x00000000, 0x1000e000, 0x00400397, 0x004003d7},
    {0x0001e000, 0x11000000, 0x00440017, 0x00440415},
    {0x0001e000, 0x12000000, 0x00480017, 0x00480415},
    {0x01ffe000, 0x1e000000, 0x00780017, 0x007c0017}
};
}
