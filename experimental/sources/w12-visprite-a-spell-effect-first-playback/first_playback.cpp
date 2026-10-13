// Observed: FirstPlayback__19VISpellEffectSystem at 0x01003108, 16 bytes. With
// the object as this: p is the word at this + 100; the function returns p when
// the first word of p (the node's first link) is non-zero and 0 when it is
// zero. Nothing else is read or written.
// Inferred: this + 100 is the first-pointer of a doubly ended list header
// (the free list used by DeletePlayback sits at this + 112, 12 bytes on) so a
// node whose first link is zero is the end marker, not a playback record.
// Unknown: the list type name.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
char* FirstPlayback__19VISpellEffectSystem(void* self) {
    unsigned node = *reinterpret_cast<unsigned*>(static_cast<char*>(self) + 100);
    unsigned link = *reinterpret_cast<unsigned*>(NORRATH_TRANSLATE(node));
    return link ? NORRATH_TRANSLATE(node) : nullptr;
}
}
