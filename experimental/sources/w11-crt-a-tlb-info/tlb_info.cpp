// Observed TLBInfo at 0x00446000, 28 bytes, pinned SLUS_205.65: an initialised data object of seven
// words that InitTLB32MB reads. Words 0, 1 and 2 are the entry counts of three tables (13, 18, 8);
// word 3 starts at zero and receives the TLB index reached after the first two tables; words 4, 5 and 6
// point at those tables: kernelTLB (0x00445d90), defaultTLB (0x00445e60) and extendTLB (0x00445f80),
// each defined by its own unit. The counts equal the table sizes (208, 288 and 128 bytes of 16-byte
// entries) and the three tables sit back to back in the executable.
// Inferred only: the roles of the three tables beyond their symbol names.
extern "C" {
struct Tlb32Entry {
    unsigned word0, word1, word2, word3;
};

extern Tlb32Entry kernelTLB[13];
extern Tlb32Entry defaultTLB[18];
extern Tlb32Entry extendTLB[8];

struct Tlb32Info {
    int count0, count1, count2;
    int final_index;
    const Tlb32Entry* table0;
    const Tlb32Entry* table1;
    const Tlb32Entry* table2;
};

Tlb32Info TLBInfo = {13, 18, 8, 0, kernelTLB, defaultTLB, extendTLB};
}
