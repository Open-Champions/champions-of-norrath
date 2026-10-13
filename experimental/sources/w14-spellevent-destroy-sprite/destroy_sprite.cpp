// Observed: NewDestroySprite__18VISpellEventDriverP12VISpellEvent at 0x0117ea08 is 240 bytes in the pinned SLUS-20565 image. Arguments: (this, record). Always
// returns 0. Searching: starting at the node the word at this+76 points to, it walks the chain of
// nodes (first word of a node is the next node; a node whose first word is zero ends the chain and is
// not examined). A node matches when its event pointer (word at node+116) has the int at +4 equal to
// the int at record+4 and the int at +0 equal to 9. With no match nothing else happens. For the
// first match: Remove__7VILNode(node); then when the word at node+140 is not -1,
// ReleaseActor__7VIScenei(the word at node+16, that value); the same for node+144; then when the
// word at node+120 is non-zero _$_17VIParticleEmitter(it, 3); the same for node+124; when node+128 is
// non-zero _$_14VIMotifBlender(it, 3); finally __builtin_delete(node).
// Inferred: handles record type 9 (sprite) of the driver's event table: it ends the earlier active
// event of that type with the same id and releases everything the event owned (the same sequence the
// driver destructor uses for its records).
// Unknown: the meaning of the id at record+4 beyond matching, what +140/+144 are besides handles
// released on the scene. Nothing here ran on the EE.
extern "C" {
void Remove__7VILNode(void* node);
void ReleaseActor__7VIScenei(void* scene, int actor);
void _$_17VIParticleEmitter(void* emitter, int flags);
void _$_14VIMotifBlender(void* blender, int flags);
void __builtin_delete(void* pointer);

int NewDestroySprite__18VISpellEventDriverP12VISpellEvent(unsigned char* self, unsigned char* record) {
    unsigned char* node = *reinterpret_cast<unsigned char**>(self + 76);
    unsigned char* next = *reinterpret_cast<unsigned char**>(node);
    unsigned char* found = 0;
    int id = *reinterpret_cast<int*>(record + 4);
    while (next != 0) {
        unsigned char* event = *reinterpret_cast<unsigned char**>(node + 116);
        if (*reinterpret_cast<int*>(event + 4) == id && *reinterpret_cast<int*>(event) == 9) {
            found = node;
            break;
        }
        node = next;
        next = *reinterpret_cast<unsigned char**>(node);
    }
    if (found != 0) {
        Remove__7VILNode(found);
        if (*reinterpret_cast<int*>(found + 140) != -1) {
            ReleaseActor__7VIScenei(*reinterpret_cast<void**>(found + 16), *reinterpret_cast<int*>(found + 140));
        }
        if (*reinterpret_cast<int*>(found + 144) != -1) {
            ReleaseActor__7VIScenei(*reinterpret_cast<void**>(found + 16), *reinterpret_cast<int*>(found + 144));
        }
        if (*reinterpret_cast<void**>(found + 120) != 0) {
            _$_17VIParticleEmitter(*reinterpret_cast<void**>(found + 120), 3);
        }
        if (*reinterpret_cast<void**>(found + 124) != 0) {
            _$_17VIParticleEmitter(*reinterpret_cast<void**>(found + 124), 3);
        }
        if (*reinterpret_cast<void**>(found + 128) != 0) {
            _$_14VIMotifBlender(*reinterpret_cast<void**>(found + 128), 3);
        }
        __builtin_delete(found);
    }
    return 0;
}
}
