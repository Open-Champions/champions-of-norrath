// Observed InitTLBFunctions at 0x002dfbe0. The final query uses the constant
// identifier 3 and stores the service result in _kExecArg.
extern "C" {
void kernel_tlb_setup(unsigned first, unsigned second);
void kernel_tlb_copy(unsigned destination, const void* source, unsigned bytes);
unsigned kernel_tlb_get_entry(unsigned identifier);
int FlushCache(int mode);
extern unsigned kernel_tlb_sysentry[];
extern unsigned char tlbsrc[];
extern unsigned _kExecArg;
void InitTLBFunctions() {
    kernel_tlb_setup(kernel_tlb_sysentry[0], kernel_tlb_sysentry[1]);
    // Observed destination 0x80075000 and 816 bytes, the tlbsrc symbol span.
    kernel_tlb_copy(0x80075000u, tlbsrc, 816);
    FlushCache(0);
    FlushCache(2);
    kernel_tlb_setup(kernel_tlb_sysentry[2], kernel_tlb_sysentry[3]);
    kernel_tlb_setup(kernel_tlb_sysentry[4], kernel_tlb_sysentry[5]);
    unsigned* cursor = kernel_tlb_sysentry + 6;
    int gate = 3;
    do {
        unsigned identifier = *cursor;
        ++gate;
        unsigned entry = kernel_tlb_get_entry(identifier);
        kernel_tlb_setup(*cursor, entry);
        cursor += 2;
    } while (gate < 8);
    _kExecArg = kernel_tlb_get_entry(3);
}
}
