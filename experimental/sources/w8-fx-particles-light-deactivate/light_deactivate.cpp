// lightDeactivate__FP5Light at 0x0013eb28, 52 bytes, read from the pinned
// SLUS-20565 image. Light words: next at 0, previous-link at 4 (as lightAdd
// establishes). If the previous-link is nonzero, the word it addresses takes
// next. If next is nonzero, the word at next+4 takes the previous-link. The
// light's own next and previous-link are then cleared. next is reloaded after
// the first store, so an object whose previous-link points at its own next
// word sees the value it just wrote. Fresh compilation is not an original-byte
// match.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_pointer(unsigned address);
#endif

namespace {
unsigned* word_at(unsigned address) {
#if defined(NORRATH_HOST_CONTRACT)
    return static_cast<unsigned*>(norrath_host_pointer(address));
#else
    return reinterpret_cast<unsigned*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
}
}

extern "C" void lightDeactivate__FP5Light(void* light) {
    unsigned* words = static_cast<unsigned*>(light);
    if (words[1] != 0u) {
        *word_at(words[1]) = words[0];
    }
    if (words[0] != 0u) {
        word_at(words[0])[1] = words[1];
    }
    words[0] = 0u;
    words[1] = 0u;
}
