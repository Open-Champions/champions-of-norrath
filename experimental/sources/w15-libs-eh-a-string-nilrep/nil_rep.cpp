// Observed _t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep at 0x011f6cb8:
// 16 bytes (a weak object in the overlay image, pinned SLUS_205.65), the shared empty representation of the old
// library's character string. The image holds four 32-bit words: 0, 0, 1, 0. The callers treat them as
// length (0), capacity (4), reference count (8) and shared flag (12), with the characters (none) starting at +16.
// Inferred: the field roles, from how clone__Q2t12basic_string...Rep and the realm-interface constructor use the
// words. Unknown: why the reference count starts at 1; nothing was executed on the EE.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
struct w15_nil_rep {
    unsigned length;
    unsigned capacity;
    int references;
    int shared;
};
static_assert(sizeof(w15_nil_rep) == 16, "representation header size");

w15_nil_rep w15_nil_rep_char __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep")
    NORRATH_LOADED(".data") = {0, 0, 1, 0};
}
