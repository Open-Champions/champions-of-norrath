// Observed: Purge__17VIParticleEmitter at 0x010b8d38 is 84 bytes. It calls RemHead__7VILList(emitter + 160) until the result is null.
// Each non-null node goes to AddTail__7VILListP7VILNode(*(word at +80) + 64, node); the +80 word is re-read for every node. After
// the list is empty it stores zero in the word at +172. Unlike Destroy__17VIParticleEmitter it does not call Remove__7VILNode, does
// not clear +80 or +84, and does not touch the matrix. The +80 word is not tested for zero. Nothing is returned.
// Inferred: +80 points at an owner with a list at +64 that takes back the emitter's nodes; the same loop appears in Destroy.
// Unknown: what the owner is, and what +172 holds.
extern "C" {
void* RemHead__7VILList(void* list);
void AddTail__7VILListP7VILNode(void* list, void* node);

#ifdef NORRATH_HOST_ARENA
unsigned char* norrath_host_pointer(unsigned address);
#define OWNER_OF(word) norrath_host_pointer(word)
#else
#define OWNER_OF(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

void Purge__17VIParticleEmitter(unsigned char* emitter) {
    void* node;
    while ((node = RemHead__7VILList(emitter + 160)) != 0) {
        AddTail__7VILListP7VILNode(OWNER_OF(*reinterpret_cast<unsigned*>(emitter + 80)) + 64, node);
    }
    *reinterpret_cast<unsigned*>(emitter + 172) = 0;
}
}
