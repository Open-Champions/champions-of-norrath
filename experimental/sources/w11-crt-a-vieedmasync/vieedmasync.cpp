// Observed VIEEDMASync__Fi at 0x010f9b40, 116 bytes, pinned SLUS_205.65. One int argument.
//   - Argument 1: reads the file-local word First (0x011b00fc; the symbol table has it as
//     First.135, initial value 1). When it is zero, waits for the coprocessor-0 condition
//     (the EE DMA-complete signal): a sync, then a loop of five nops and a branch-on-false
//     back to the first nop. In either case First is then set to zero. So the wait is skipped
//     exactly once, on the first call, and done on every later one.
//   - Any other argument: sceDmaSync(sceDmaGetChan(argument), 0, 0). Nothing is returned.
// The wait loop is written as inline assembly because C++ has no way to test the condition;
// it reproduces the instruction sequence observed (sync; five nops; bc0f back; delay-slot nop).
// Inferred: First means "no transfer has been started yet" (the name comes from the symbol
// table). Unknown: which DMA channel mode 1 is meant to wait on; the condition covers all.
// Nothing was executed on the EE.
extern "C" {
void* sceDmaGetChan(int channel);
int sceDmaSync(void* channel, int mode, int timeout);
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_dma_wait();
#define NORRATH_DMA_WAIT() norrath_host_dma_wait()
#else
#define NORRATH_DMA_WAIT() __asm__ volatile(".set push\n.set noreorder\nsync\n1:\nnop\nnop\nnop\nnop\nnop\nbc0f 1b\nnop\nnop\n.set pop" ::: "memory")
#endif

extern "C" void VIEEDMASync__Fi(int mode) {
    static int First = 1;
    if (mode == 1) {
        if (First == 0) NORRATH_DMA_WAIT();
        First = 0;
    } else {
        sceDmaSync(sceDmaGetChan(mode), 0, 0);
    }
}
