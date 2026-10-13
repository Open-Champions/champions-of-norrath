/* Observed _sceFs_Poff_Intr at 0x002d8fe0, 68 bytes (a local routine; sceFsInit
 * registers it as the handler for command 0x80000013 with _sif_FsPoff_Data as the
 * argument). Its second argument points at two words. When the first is nonzero it
 * is a function: the global-pointer register is saved, loaded from sceCbPoff_Gp
 * (0x008e1c80), the function is called with the second word as its argument, and the
 * global pointer is restored. Then sync and the interrupt-enable instruction run.
 * Nothing is returned; the first argument is not used.
 * Inferred: this forwards a power-off notification to a callback that the program
 * registered together with its own global pointer.
 * Unknown: who stores the callback and sceCbPoff_Gp, and the meaning of the first
 * argument. Compiled output is not claimed to match the original.
 */
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_gp;
#define NORRATH_GP_REGISTER (norrath_host_gp)
#define NORRATH_SYNC_EI() (norrath_host_sync_ei())
extern "C" void norrath_host_sync_ei();
#else
register unsigned norrath_gp_register asm("$28");
#define NORRATH_GP_REGISTER (norrath_gp_register)
#define NORRATH_SYNC_EI() asm volatile("sync\n\tei" ::: "memory")
#endif
extern "C" {
extern unsigned sceCbPoff_Gp;

void _sceFs_Poff_Intr(void* command, unsigned* callback_block) {
    typedef void (*Callback)(unsigned);
    const Callback callback = reinterpret_cast<Callback>(static_cast<__UINTPTR_TYPE__>(callback_block[0]));
    if (callback != 0) {
        const unsigned saved_gp = NORRATH_GP_REGISTER;
        NORRATH_GP_REGISTER = sceCbPoff_Gp;
        callback(callback_block[1]);
        NORRATH_GP_REGISTER = saved_gp;
    }
    NORRATH_SYNC_EI();
}
}
