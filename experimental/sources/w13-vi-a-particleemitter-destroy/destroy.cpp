// Observed: Destroy__17VIParticleEmitter at 0x010b8ac0 is 124 bytes. If the word at +80 of the
// emitter is non-zero: it calls Remove__7VILNode(emitter), then repeatedly calls
// RemHead__7VILList(emitter + 160) and, for each non-null result node, calls
// AddTail__7VILListP7VILNode(*(word at +80) + 64, node) (the +80 word is re-read for every
// node). After the list is empty it stores zero at +80 and at +84, calls
// Identity__10VIMatrix44(emitter + 96) and stores zero at +172. In every case it finally
// calls UnlinkAllMotifBlenders__17VIParticleEmitter(emitter). Nothing is returned.
// Inferred: +80 points at an owning object (a particle system or pool) that has a list at
// +64; destroying detaches the emitter from its current list, hands the nodes in its own
// list at +160 back to that owner, and resets the emitter's matrix (+96).
// Unknown: what the owner at +80 is, what +84 and +172 are, and the node ownership details.
extern "C" {
void Remove__7VILNode(void* node);
void* RemHead__7VILList(void* list);
void AddTail__7VILListP7VILNode(void* list, void* node);
void Identity__10VIMatrix44(void* matrix);
void UnlinkAllMotifBlenders__17VIParticleEmitter(void* emitter);

// A host check defines NORRATH_HOST_ARENA because a 64-bit host cannot keep the owner
// pointer in a 4-byte word; the word then holds an arena offset.
#ifdef NORRATH_HOST_ARENA
unsigned char* norrath_host_pointer(unsigned address);
#define OWNER_OF(word) norrath_host_pointer(word)
#else
#define OWNER_OF(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

void Destroy__17VIParticleEmitter(unsigned char* emitter) {
    if (*reinterpret_cast<unsigned*>(emitter + 80) != 0) {
        Remove__7VILNode(emitter);
        void* node;
        while ((node = RemHead__7VILList(emitter + 160)) != 0) {
            AddTail__7VILListP7VILNode(OWNER_OF(*reinterpret_cast<unsigned*>(emitter + 80)) + 64, node);
        }
        *reinterpret_cast<unsigned*>(emitter + 80) = 0;
        *reinterpret_cast<unsigned*>(emitter + 84) = 0;
        Identity__10VIMatrix44(emitter + 96);
        *reinterpret_cast<unsigned*>(emitter + 172) = 0;
    }
    UnlinkAllMotifBlenders__17VIParticleEmitter(emitter);
}
}
