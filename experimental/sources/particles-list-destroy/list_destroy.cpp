// listDestroy__FP8ListHead at 0x00194300, 144 bytes, read from the pinned
// SLUS-20565 image. 228 direct calls in main .text. Fresh compilation is
// not an original-byte match. No list has been destroyed on the EE.
//
// The head argument is one word, the first node. Each node is addressed as
// three words: next at +0, previous at +4, and the paired node at +8.
// freeNodes at gp-3972 (0x00486a6c) is the free-list head. An empty head
// still loads freeNodes and then returns without storing it.
//
// A nonzero previous is written with the captured next, and the next word
// is loaded again after that store. A nonzero next is written with the
// previous word, loaded again after the previous store. The paired node is
// loaded after that write. The pair is unlinked the same way, pushed onto
// the free list, and then the node itself is pushed. The walk continues
// from the next word captured before those writes. freeNodes is stored
// only after the last node.
extern "C" {
extern unsigned char* freeNodes;

#if defined(NORRATH_PARTICLES_HOST)
extern unsigned long long norrath_particles_host_base;
#endif

static unsigned read_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}

static void write_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}

static unsigned char* from_word(unsigned word) {
    if (word == 0u) return nullptr;
#if defined(NORRATH_PARTICLES_HOST)
    return reinterpret_cast<unsigned char*>(norrath_particles_host_base + word);
#else
    return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(word));
#endif
}

static unsigned to_word(const void* pointer) {
    if (pointer == nullptr) return 0u;
#if defined(NORRATH_PARTICLES_HOST)
    return static_cast<unsigned>(reinterpret_cast<unsigned long long>(pointer) - norrath_particles_host_base);
#else
    return static_cast<unsigned>(reinterpret_cast<unsigned long>(pointer));
#endif
}

void listDestroy__FP8ListHead(void* head) {
    unsigned node = read_word(static_cast<unsigned char*>(head));
    unsigned free_list = to_word(freeNodes);
    if (node == 0u) return;
    do {
        unsigned char* current = from_word(node);
        const unsigned captured_next = read_word(current);
        const unsigned previous = read_word(current + 4);
        unsigned fresh_next = captured_next;
        if (previous != 0u) {
            write_word(from_word(previous), captured_next);
            fresh_next = read_word(current);
        }
        if (fresh_next != 0u) {
            const unsigned fresh_previous = read_word(current + 4);
            write_word(from_word(fresh_next) + 4, fresh_previous);
        }
        unsigned twin = read_word(current + 8);
        if (twin != 0u) {
            unsigned char* paired = from_word(twin);
            const unsigned twin_previous = read_word(paired + 4);
            if (twin_previous != 0u) {
                const unsigned twin_next = read_word(paired);
                write_word(from_word(twin_previous), twin_next);
            }
            const unsigned fresh_twin_next = read_word(paired);
            if (fresh_twin_next != 0u) {
                const unsigned fresh_twin_previous = read_word(paired + 4);
                write_word(from_word(fresh_twin_next) + 4, fresh_twin_previous);
            }
            write_word(paired, free_list);
            free_list = twin;
        }
        write_word(current, free_list);
        free_list = node;
        node = captured_next;
    } while (node != 0u);
    freeNodes = from_word(free_list);
}
}
