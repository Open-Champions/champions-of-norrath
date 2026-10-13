/* Observed sceGsPutDrawEnv at 0x002d2888, 224 bytes. The argument is a packet whose first
 * doubleword has a quadword count in its low 15 bits. Using the DMA channel 2 registers
 * (control at 0x1000a000, memory address at 0x1000a010, quadword count at 0x1000a020):
 *   1. If bit 8 of the control word (0x100) is set, it re-reads that word up to 16,777,217
 *      times waiting for the bit to clear. When the counter that started at zero and is
 *      incremented after each test passes 0x1000000 first, it prints
 *      "sceGsPutDrawEnv: DMA Ch.2 does not terminate\r\n" with scePrintf and returns -1.
 *   2. Otherwise the count register gets (first doubleword & 0x7fff) + 1, the memory
 *      address register gets the packet address with the top nibble cleared (mask
 *      0x0fffffff) and bit 31 set when the packet address has 0x70000000 in its top bits,
 *      and the control register gets 0x101. The result is 0.
 * Inferred: channel 2 is the path to the graphics synthesizer; 0x70000000 marks scratchpad
 * addresses, which the DMA controller addresses with bit 31; 0x101 starts the transfer.
 * Unknown: whether the control word read before the loop is affected by other agents, and
 * the exact timing of the wait.
 */
#ifndef W12_GS_DMA_BASE
#define W12_GS_DMA_BASE 0x1000a000u
#endif

extern "C" {
int scePrintf(const char* format, ...);

int sceGsPutDrawEnv(unsigned long long* packet) {
    volatile unsigned* const control = reinterpret_cast<volatile unsigned*>(W12_GS_DMA_BASE);
    volatile unsigned* const address = reinterpret_cast<volatile unsigned*>(W12_GS_DMA_BASE + 0x10);
    volatile unsigned* const count = reinterpret_cast<volatile unsigned*>(W12_GS_DMA_BASE + 0x20);
    if (*control & 0x100u) {
        unsigned waited = 0;
        for (;;) {
            const unsigned seen = waited++;
            if (seen > 0x1000000u) {
                scePrintf("sceGsPutDrawEnv: DMA Ch.2 does not terminate\r\n");
                return -1;
            }
            if ((*control & 0x100u) == 0) break;
        }
    }
    const unsigned packet_address = static_cast<unsigned>(reinterpret_cast<unsigned long>(packet));
    *count = static_cast<unsigned>(static_cast<int>(*packet & 0x7fff) + 1);
    if ((packet_address & 0x70000000u) == 0x70000000u)
        *address = (packet_address & 0x0fffffffu) | 0x80000000u;
    else
        *address = packet_address & 0x0fffffffu;
    *control = 0x101;
    return 0;
}
}
