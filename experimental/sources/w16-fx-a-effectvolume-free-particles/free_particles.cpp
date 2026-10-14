// Observed: FreeParticles__14VIEffectVolume at 0x0114a360, 56 bytes. If the word at this+88 is non-zero it calls __wrap_free with that
// word. In either case it then stores zero at this+88. Nothing is returned.
// Inferred: this+88 is the particle storage that AllocParticles__14VIEffectVolume makes, and zeroing it makes a second call harmless.
// Unknown: the element type and count of that storage.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void __wrap_free(void* pointer);

void FreeParticles__14VIEffectVolume(unsigned char* self) {
    unsigned particles = *reinterpret_cast<unsigned*>(self + 88);
    if (particles != 0) __wrap_free(NORRATH_WORD_TO_POINTER(particles));
    *reinterpret_cast<unsigned*>(self + 88) = 0;
}
}
