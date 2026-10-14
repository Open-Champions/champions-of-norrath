// Observed: ClearAllFireballs__12VIAtmosphere at 0x0106b960 is 200 bytes. Words of the
// atmosphere object (this): w1228 at byte offset 1228, w4460, w4476. It returns at once when
// w1228 is zero or else when w4460 is zero, and, after those checks, when w4476 is zero.
// Otherwise it repeatedly takes RemHead__7VILList(this+4448); for each non-null node it calls
// Destroy__17VIParticleEmitter(node+64) and then Destroy__17VIParticleEmitter(node+1056).
// When the list is empty it stores zero at +4460, runs Init__7VILList on this+4448 and on this+4464,
// and then, for i from 0 to 31, calls AddTail__7VILListP7VILNode(this+4464, w4476 + 2048 * i),
// re-reading w4476 each time. Nothing is returned.
// Inferred: w4476 points to a pool of 32 records of 2048 bytes; each record holds two particle
// emitters (at +64 and +1056) and a list link at +0; the clear tears down active fireballs and
// puts the whole pool back on the free list. Unknown: member names and what +4460 counts.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void Init__7VILList(void* list);
void AddTail__7VILListP7VILNode(void* list, void* node);
unsigned char* RemHead__7VILList(void* list);
void Destroy__17VIParticleEmitter(void* emitter);

void ClearAllFireballs__12VIAtmosphere(unsigned char* self) {
    if (*reinterpret_cast<const unsigned*>(self + 1228) == 0) return;
    if (*reinterpret_cast<const unsigned*>(self + 4460) == 0) return;
    if (*reinterpret_cast<const unsigned*>(self + 4476) == 0) return;
    while (unsigned char* node = RemHead__7VILList(self + 4448)) {
        Destroy__17VIParticleEmitter(node + 64);
        Destroy__17VIParticleEmitter(node + 1056);
    }
    *reinterpret_cast<unsigned*>(self + 4460) = 0;
    Init__7VILList(self + 4448);
    Init__7VILList(self + 4464);
    for (int i = 0; i < 32; ++i)
        AddTail__7VILListP7VILNode(self + 4464, NORRATH_WORD_TO_POINTER(*reinterpret_cast<const unsigned*>(self + 4476)) + 2048 * i);
}
}
