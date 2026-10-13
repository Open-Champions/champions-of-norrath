// Host contract for kernelTLB. Not an EE plan input. The table is compared with the internal rules
// of an R5900 TLB entry pair (page size from the mask, the second page directly behind the first,
// both frame numbers and the virtual address aligned to the page pair) and with its size; the
// executable comparison is a separate step (data_check in w11-crt-a-data-check).
#include <cstdio>
#include "kernel_tlb.cpp"
int main() {
    int failures = 0;
    const unsigned count = sizeof(kernelTLB) / sizeof(kernelTLB[0]);
    if (count != 13 || sizeof(Tlb32Entry) != 16) { std::printf("size\n"); return 1; }
    for (unsigned i = 0; i < count; ++i) {
        const Tlb32Entry& e = kernelTLB[i];
        if (e.word2 & 0x80000000u) continue;                       // scratch-pad style entry: no frame pair
        const unsigned page = (e.word0 >> 1) + 0x1000;             // bytes per page from the page mask
        const unsigned pa0 = (e.word2 >> 6) << 12, pa1 = (e.word3 >> 6) << 12;
        if (pa1 - pa0 != page) { std::printf("entry %u: second page not behind the first\n", i); ++failures; }
        if (pa0 % (2 * page) != 0 || e.word1 % (2 * page) != 0) { std::printf("entry %u: not pair aligned\n", i); ++failures; }
        const unsigned alias = e.word1 - pa0;
        if (!(alias == 0 || e.word1 == 0xffff8000u)) { std::printf("entry %u: unexpected virtual offset %08x\n", i, alias); ++failures; }
    }
    if (failures) return 2;
    std::puts("kernelTLB structure ok");
    return 0;
}
