/* Observed _sceCd_Poff_Intr at 0x002e28c8, 84 bytes; it is local in the
 * original and published global here because PowerOffCB takes its address.
 * It ignores both incoming arguments. When sceCdPoffCbfunc (0x008e8148) is
 * nonzero and Init_seq (0x004460a4) is zero, it saves gp, loads gp from
 * sceCdCbPoff_Gp (0x008e814c), calls the stored function with the word at
 * sceCdPoffCbdata (0x008e8150) as its argument, and restores gp. The original
 * also leaves the function pointer itself in the second argument register at
 * the call; that is not reproduced. sceCdPOffCallback at 0x002e2848 stores the
 * three words; it is not defined here. Compiled output is not claimed to match
 * the original.
 */
extern "C" {
extern int Init_seq;
extern void (*sceCdPoffCbfunc)(void*);
extern unsigned sceCdCbPoff_Gp;
extern void* sceCdPoffCbdata;
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_swap_gp(unsigned value);
#define NORRATH_SWAP_GP(value) norrath_host_swap_gp(value)
#else
static inline unsigned norrath_swap_gp(unsigned value) {
    unsigned previous;
    asm volatile("move %0,$gp\n\tmove $gp,%1" : "=&r"(previous) : "r"(value) : "memory");
    return previous;
}
#define NORRATH_SWAP_GP(value) norrath_swap_gp(value)
#endif

extern "C" void _sceCd_Poff_Intr(void*, void*) {
    void (*const callback)(void*) = sceCdPoffCbfunc;
    if (callback == nullptr || Init_seq != 0) return;
    const unsigned saved = NORRATH_SWAP_GP(sceCdCbPoff_Gp);
    callback(sceCdPoffCbdata);
    NORRATH_SWAP_GP(saved);
}
