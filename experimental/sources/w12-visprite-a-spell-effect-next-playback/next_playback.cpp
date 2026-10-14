// Observed: NextPlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi
// at 0x01003118, 16 bytes. Only the first two argument registers are used: with
// p the second argument, n is the word at p + 0 (the node's first link); the
// function returns n when the first word of n is non-zero and 0 when it is zero.
// Inferred: same list shape as FirstPlayback__19VISpellEffectSystem; a node
// whose own first link is zero is the end marker. Unknown: what the further
// parameters suggested by the mangled name are; this body ignores them.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
char* NextPlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(void* self, char* playback) {
    (void)self;
    unsigned node = *reinterpret_cast<unsigned*>(playback);
    unsigned link = *reinterpret_cast<unsigned*>(NORRATH_TRANSLATE(node));
    return link ? NORRATH_TRANSLATE(node) : nullptr;
}
}
