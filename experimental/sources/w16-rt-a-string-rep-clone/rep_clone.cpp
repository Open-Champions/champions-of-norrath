// Observed clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep at
// 0x011f6cc8 (a weak template instantiation of the old library's character-string representation copy). The symbol
// table gives 284 bytes, but the code continues to 0x011f6eb4 (492 bytes) with the allocator inlined; the next symbol
// starts at 0x011f6eb8. The argument is a representation record: words length (0), capacity (4), reference count (8),
// shared flag (12), then the characters from byte 16. The result is the address of the new record's characters
// (record + 16). The copy:
//   * capacity starts at 16 and doubles until length + 1 <= capacity; the block size is capacity + 16;
//   * a block of at most 128 bytes comes from the small-block free lists (_S_free_list, one word per eight-byte size,
//     index ((capacity + 23) >> 3) - 1): a non-empty list gives its head and advances to that block's first word; an
//     empty one calls _S_chunk_alloc (0x011f3318) for the size rounded up to eight bytes with a count of 20 and takes
//     the first block, linking the other count - 1 blocks into the list (the last block's link is zero; a count of 1
//     links nothing and leaves the list untouched);
//   * a larger block comes from __wrap_malloc (0x001449a8); while it returns null, the handler word
//     (_malloc_alloc_oom_handler) is read: zero writes "out of memory" to cerr, then endl, then exit(1); otherwise the
//     handler is called and the allocation repeated;
//   * the new record gets the capacity, a reference count of 1 and a shared flag of 0; a nonzero length is copied from
//     byte 16 to byte 16 with memcpy; the length word is then read again from the source and stored.
// Inferred (not named in the image): the roles of the four header words and of the handler word. Unknown: the bodies of
// _S_chunk_alloc and the stream routines. Addresses are 32-bit words; a host check supplies an address base.
#if defined(W16_HOST_CHECK)
extern unsigned char* w16_host_arena;
static inline void* at(unsigned word) { return word == 0 ? nullptr : w16_host_arena + word; }
static inline unsigned to_word(const void* p) { return p == nullptr ? 0 : static_cast<unsigned>(static_cast<const unsigned char*>(p) - w16_host_arena); }
#else
static inline void* at(unsigned word) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word)); }
static inline unsigned to_word(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
#endif

namespace {
struct Rep {
    unsigned length;
    unsigned capacity;
    int references;
    int shared;
};
}

extern "C" {
extern unsigned free_lists[16] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");
extern void (*oom_handler)() __asm__("_t23__malloc_alloc_template1i0$__malloc_alloc_oom_handler");
unsigned chunk_alloc(unsigned size, int* count) __asm__("_S_chunk_alloc__t24__default_alloc_template2b0i0UiRi");
extern char error_stream[] __asm__("cerr");
void* __wrap_malloc(unsigned size);
void* __ls__7ostreamPCc(void* stream, const char* text);
void* endl__FR7ostream(void* stream);
void exit(int status);
void* memcpy(void* to, const void* from, unsigned length);

char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(Rep* source) {
    unsigned capacity = 16;
    unsigned size = capacity + 16;
    if (capacity < source->length + 1) {
        do {
            capacity <<= 1;
        } while (capacity < source->length + 1);
        size = capacity + 16;
    }

    unsigned block;
    if (size > 128) {
        block = to_word(__wrap_malloc(size));
        while (block == 0) {
            if (oom_handler == nullptr) {
                endl__FR7ostream(__ls__7ostreamPCc(error_stream, "out of memory"));
                exit(1);
            }
            oom_handler();
            block = to_word(__wrap_malloc(size));
        }
    } else {
        const unsigned index = ((capacity + 23) >> 3) - 1;
        unsigned* list = &free_lists[index];
        const unsigned head = *list;
        if (head != 0) {
            *list = *static_cast<unsigned*>(at(head));
            block = head;
        } else {
            const unsigned rounded = (capacity + 23) & ~7u;
            int count = 20;
            block = chunk_alloc(rounded, &count);
            if (count != 1) {
                unsigned current = block + rounded;
                *list = current;
                for (int i = 1;; ++i) {
                    const unsigned following = current + rounded;
                    if (count - 1 == i) {
                        *static_cast<unsigned*>(at(current)) = 0;
                        break;
                    }
                    *static_cast<unsigned*>(at(current)) = following;
                    current = following;
                }
            }
        }
    }

    Rep* rep = static_cast<Rep*>(at(block));
    rep->capacity = capacity;
    rep->references = 1;
    rep->shared = 0;
    char* characters = reinterpret_cast<char*>(rep + 1);
    if (source->length != 0) memcpy(characters, reinterpret_cast<const char*>(source + 1), source->length);
    rep->length = source->length;
    return characters;
}
}
