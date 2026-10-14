// Observed: Clear__16VIParticleSystemR12VIDictionary at 0x010b7e30 is 256 bytes.
// Step 1: take the word at offset 52 as a node pointer and follow the first word
// of each node (next) until a node whose next is zero. For each node before that
// one whose word at 80 is nonzero, call Init__7VILList on node+160 and store zero
// at node+172, 80 and 84. Step 2: Init__7VILList on this+52 and this+64. Step 3:
// for index 0 to the signed count at 76 (rechecked each time), AddTail__7VILListP7VILNode
// (this+64, *(this+80) + 104*index). Step 4: ReleaseAllParticleDefinitions__16VIParticleSystemR12VIDictionary
// (this, dictionary). Step 5: if the word at 80 is nonzero pass it to
// __builtin_vec_delete and zero it; zero the word at 76 in both cases. Returns 0.
// Inferred: the object at 80 is an array of 104-byte records from vector new
// (see Init); the nodes in step 1 are the active particle-definition users.
extern "C" {
void Init__7VILList(void* list);
void AddTail__7VILListP7VILNode(void* list, void* node);
int ReleaseAllParticleDefinitions__16VIParticleSystemR12VIDictionary(void* object, void* dictionary);
void __builtin_vec_delete(void* pointer);

static unsigned char* at(unsigned int word) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
}
static unsigned int& word_at(unsigned char* base, int offset) {
    return *reinterpret_cast<unsigned int*>(base + offset);
}

int Clear__16VIParticleSystemR12VIDictionary(void* object, void* dictionary) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    unsigned char* node = at(word_at(bytes, 52));
    unsigned int next = word_at(node, 0);
    if (next != 0) {
        do {
            if (word_at(node, 80) != 0) {
                Init__7VILList(node + 160);
                word_at(node, 172) = 0;
                next = word_at(node, 0);
                word_at(node, 80) = 0;
                word_at(node, 84) = 0;
            }
            node = at(next);
            next = word_at(node, 0);
        } while (next != 0);
    }
    Init__7VILList(bytes + 52);
    Init__7VILList(bytes + 64);
    for (int index = 0; index < static_cast<int>(word_at(bytes, 76)); ++index) {
        AddTail__7VILListP7VILNode(bytes + 64, at(word_at(bytes, 80)) + index * 104);
    }
    ReleaseAllParticleDefinitions__16VIParticleSystemR12VIDictionary(object, dictionary);
    if (word_at(bytes, 80) != 0) {
        __builtin_vec_delete(at(word_at(bytes, 80)));
        word_at(bytes, 80) = 0;
    }
    word_at(bytes, 76) = 0;
    return 0;
}
}
