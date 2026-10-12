/* Observed __wrap_memalign at 0x001449e0, 56 bytes. The byte at
 * enableMallocWrapper (0x00483b11) selects the path. Zero tail-calls the
 * library memalign with both incoming arguments. Nonzero tail-calls
 * memAlloc__FiPc with the second argument as the size and the string
 * "fake malloc" as the label; the first argument (the alignment) is then
 * not used. Compiled output is not claimed to match the original.
 */
extern "C" {
extern unsigned char enableMallocWrapper;
void* memAlloc__FiPc(int bytes, char* label);
void* memalign(unsigned alignment, unsigned size);
}

extern "C" void* __wrap_memalign(unsigned alignment, unsigned size) {
    if (enableMallocWrapper == 0) return memalign(alignment, size);
    return memAlloc__FiPc(static_cast<int>(size), const_cast<char*>("fake malloc"));
}
