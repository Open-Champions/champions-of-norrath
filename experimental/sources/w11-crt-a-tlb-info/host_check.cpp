// Host contract for TLBInfo. Not an EE plan input. Checks the counts against the tables' own sizes
// and that each pointer refers to the table of the same slot.
#include <cstdio>
#include "tlb_info.cpp"
Tlb32Entry kernelTLB[13]; Tlb32Entry defaultTLB[18]; Tlb32Entry extendTLB[8];
int main() {
    if (TLBInfo.count0 != int(sizeof(kernelTLB) / sizeof(kernelTLB[0]))) return 1;
    if (TLBInfo.count1 != int(sizeof(defaultTLB) / sizeof(defaultTLB[0]))) return 2;
    if (TLBInfo.count2 != int(sizeof(extendTLB) / sizeof(extendTLB[0]))) return 3;
    if (TLBInfo.final_index != 0) return 4;
    if (TLBInfo.table0 != kernelTLB || TLBInfo.table1 != defaultTLB || TLBInfo.table2 != extendTLB) return 5;
    if (TLBInfo.count0 + TLBInfo.count1 + TLBInfo.count2 > 48) return 6;       // InitTLB32MB's overflow limit is 49
    if (sizeof(void*) == 4 && sizeof(Tlb32Info) != 28) return 7;
    std::puts("TLBInfo ok");
    return 0;
}
