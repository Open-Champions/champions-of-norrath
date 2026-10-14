// Observed clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep at 0x011f6cc8,
// pinned SLUS_205.65 (weak, so a template instantiation of the old library's character-string representation
// copy). The symbol table gives 284 bytes, but the code continues to 0x011f6eb4 (492 bytes) with the allocator
// inlined; the next symbol starts at 0x011f6eb8. The argument is a representation record: words length (0),
// capacity (4), reference count (8), shared flag (12), then length bytes of characters from byte 16. It returns the
// address of the new record's characters (record + 16). The copy:
//   * capacity starts at 16 and doubles until it exceeds the length (length + 1 <= capacity); the block size is
//     capacity + 16;
//   * a block of at most 128 bytes comes from the small-block free lists (_S_free_list, one pointer per eight-byte
//     size, index ((capacity + 23) >> 3) - 1): a non-empty list gives its head and advances to its link; an empty one
//     calls _S_chunk_alloc (0x011f3318) for the size rounded up to eight bytes with a count of 20 and takes the
//     first block, linking the rest (the count returned may be smaller; a count of 1 gives no extra blocks) into the
//     list, the last one's link being zero;
//   * a larger block comes from __wrap_malloc (0x001449a8); on failure the old library's handler word is consulted:
//     with no handler "out of memory" is written to cerr followed by a call to endl and exit(1) ends the program;
//     otherwise the handler is called and the allocation retried until it succeeds;
//   * the new record's capacity is stored, its reference count set to 1 and its shared flag to 0; a nonzero length is
//     copied with memcpy from byte 16 to byte 16; the length word is then read again from the source and stored.
// Inferred (not named in the image): the roles of the four header words and of the handler word. Unknown: the bodies of
// _S_chunk_alloc and the stream routines; nothing was executed on the EE.
extern "C" {
struct w15_string_rep {
    unsigned length;
    unsigned capacity;
    int references;
    int shared;
};
struct w15_free_block {
    w15_free_block* next;
};

extern w15_free_block* w15_free_list[16] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");
extern void (*w15_oom_handler)() __asm__("_t23__malloc_alloc_template1i0$__malloc_alloc_oom_handler");
w15_free_block* w15_chunk_alloc(unsigned size, int& count) __asm__("_S_chunk_alloc__t24__default_alloc_template2b0i0UiRi");
extern char w15_cerr[] __asm__("cerr");
void* __wrap_malloc(unsigned size);
void* __ls__7ostreamPCc(void* stream, const char* text);
void* endl__FR7ostream(void* stream);
void exit(int status);
void* memcpy(void* to, const void* from, unsigned length);

char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(w15_string_rep* source) {
    unsigned capacity = 16;
    unsigned size = capacity + 16;
    if (capacity < source->length + 1) {
        do {
            capacity <<= 1;
        } while (capacity < source->length + 1);
        size = capacity + 16;
    }

    unsigned char* block;
    if (size > 128) {
        block = static_cast<unsigned char*>(__wrap_malloc(size));
        while (block == nullptr) {
            if (w15_oom_handler == nullptr) {
                endl__FR7ostream(__ls__7ostreamPCc(w15_cerr, "out of memory"));
                exit(1);
            }
            w15_oom_handler();
            block = static_cast<unsigned char*>(__wrap_malloc(size));
        }
    } else {
        const unsigned index = ((capacity + 23) >> 3) - 1;
        w15_free_block** list = &w15_free_list[index];
        w15_free_block* head = *list;
        if (head != nullptr) {
            *list = head->next;
            block = reinterpret_cast<unsigned char*>(head);
        } else {
            const unsigned rounded = (capacity + 23) & ~7u;
            int count = 20;
            unsigned char* chunk = reinterpret_cast<unsigned char*>(w15_chunk_alloc(rounded, count));
            block = chunk;
            if (count != 1) {
                w15_free_block* current = reinterpret_cast<w15_free_block*>(chunk + rounded);
                *list = current;
                for (int i = 1;; ++i) {
                    w15_free_block* following = reinterpret_cast<w15_free_block*>(reinterpret_cast<unsigned char*>(current) + rounded);
                    if (count - 1 == i) {
                        current->next = nullptr;
                        break;
                    }
                    current->next = following;
                    current = following;
                }
            }
        }
    }

    w15_string_rep* rep = reinterpret_cast<w15_string_rep*>(block);
    rep->capacity = capacity;
    rep->references = 1;
    rep->shared = 0;
    char* characters = reinterpret_cast<char*>(rep + 1);
    if (source->length != 0) memcpy(characters, reinterpret_cast<const char*>(source + 1), source->length);
    rep->length = source->length;
    return characters;
}
}
