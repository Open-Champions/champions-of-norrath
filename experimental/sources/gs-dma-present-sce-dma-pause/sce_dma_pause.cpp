/* Observed sceDmaPause at 0x002d32e8, 152 bytes. DIntr runs first. The word
 * at 0x1000f520 is read. When its bit 16 is clear, that word with bit 16
 * set is stored at 0x1000f590. 0x1000e000 is read and the value is unused.
 * Bit 8 of the caller's first word is cleared. 0x1000f590 then receives the
 * original 0x1000f520 word. A nonzero DIntr result calls EIntr. The return
 * value is the caller's first word from before the bit-8 clear. Fresh
 * compilation is not an original-byte match. Naming 0x1000f520 and
 * 0x1000f590 from the public DMA map is inferred.
 */
extern "C" int DIntr();
extern "C" int EIntr();

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_dma_load32(unsigned address);
extern "C" void norrath_host_dma_store32(unsigned address, unsigned value);
#define NORRATH_LOAD32(address) norrath_host_dma_load32(address)
#define NORRATH_STORE32(address, value) norrath_host_dma_store32((address), (value))
#else
static inline unsigned norrath_dma_load32(unsigned address) {
    return *reinterpret_cast<volatile unsigned*>(static_cast<unsigned long>(address));
}
static inline void norrath_dma_store32(unsigned address, unsigned value) {
    *reinterpret_cast<volatile unsigned*>(static_cast<unsigned long>(address)) = value;
}
#define NORRATH_LOAD32(address) norrath_dma_load32(address)
#define NORRATH_STORE32(address, value) norrath_dma_store32((address), (value))
#endif

extern "C" unsigned sceDmaPause(unsigned* channel) {
    const int disabled = DIntr();
    const unsigned enabler = NORRATH_LOAD32(0x1000f520u);
    if ((enabler & 0x10000u) == 0u)
        NORRATH_STORE32(0x1000f590u, enabler | 0x10000u);
    (void)NORRATH_LOAD32(0x1000e000u);
    const unsigned original = channel[0];
    channel[0] = original & 0xfffffeffu;
    NORRATH_STORE32(0x1000f590u, enabler);
    if (disabled != 0)
        EIntr();
    return original;
}
