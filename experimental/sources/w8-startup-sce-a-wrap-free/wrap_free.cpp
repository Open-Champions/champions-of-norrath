/* Observed __wrap_free at 0x00144ac0, 44 bytes. The byte at enableMallocWrapper
 * (0x00483b11) selects the release path. Nonzero tail-calls memFree__FPv with
 * the incoming pointer; zero tail-calls the library free with it. Neither
 * result is used. Compiled output is not claimed to match the original.
 */
extern "C" {
extern unsigned char enableMallocWrapper;
void memFree__FPv(void* block);
void free(void* block);
}

extern "C" void __wrap_free(void* block) {
    if (enableMallocWrapper != 0) {
        memFree__FPv(block);
        return;
    }
    free(block);
}
