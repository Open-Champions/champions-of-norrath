/* Observed eh_context_static at 0x002fa3b8, 100 bytes, pinned SLUS_205.65. It is the
 * local routine that eh_context_initialize installs as get_eh_context. The first call:
 *   1. stores 1 to the flag word at 0x008eaa80, the word just after a 224-byte block;
 *   2. zeroes the 224-byte block at 0x008ea9a0 (the original calls memset for it);
 *   3. stores the address 0x008eaa88 in the block's word at offset 4 (0x008ea9a4).
 *      That address is 232 bytes into the block, 8 bytes past the flag word.
 * It returns the block address in every call. A later call sees the flag set and
 * does only the return.
 *
 * The layout below keeps the observed offsets (flag at 224, pointer target at 232).
 * The absolute addresses in the image come from the link, so this unit uses its own
 * static storage. The pointer word is stored as a void* (32 bits on the EE). Unknown:
 * what the 8 bytes at offset 232 hold, and whether anything else reads the block.
 */
namespace {

struct EhContextStatic {
    unsigned char block[224];
    int initialized;
    alignas(8) unsigned char target[8];
};

EhContextStatic eh_context_storage;

}  // namespace

extern "C" void* eh_context_static() {
    EhContextStatic& context = eh_context_storage;
    if (context.initialized == 0) {
        context.initialized = 1;
        for (unsigned index = 0; index < sizeof context.block; ++index) context.block[index] = 0;
        void* target = context.target;
        __builtin_memcpy(context.block + 4, &target, sizeof target);
    }
    return context.block;
}
