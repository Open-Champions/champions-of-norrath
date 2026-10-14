// Observed _t24__default_alloc_template2b0i0$_S_free_list at 0x011f2910: 64 bytes (a weak object in the overlay image,
// pinned SLUS_205.65), all zero in the image. The only authored user (clone__Q2t12basic_string...Rep) indexes it with
// ((capacity + 23) >> 3) - 1 for blocks of at most 128 bytes, which gives indices 0..15, so the object is read as 16
// 32-bit list heads, one per eight-byte block size from 8 to 128 bytes. Each head is the address of the first free
// block or zero; a free block's first word is the address of the next one.
// Inferred: the object is the small-block allocator's free-list table; the sizes follow from the index formula.
// Unknown: other users of the table; nothing was executed on the EE.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
struct w15_free_block;
#if defined(__mips__)
static_assert(sizeof(w15_free_block*) == 4, "list heads are 32-bit words on the target");
#endif

w15_free_block* w15_small_block_free_list[16] __asm__("_t24__default_alloc_template2b0i0$_S_free_list")
    NORRATH_LOADED(".data") = {};
}
