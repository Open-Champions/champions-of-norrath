// Observed: NewSetEmitterMotif__18VISpellEventDriverP12VISpellEvent at 0x0117d7d0 is 452 bytes in the
// pinned SLUS-20565 image. Arguments: (this, record). Returns -1 when no emitter is found, otherwise 0.
// "Chain search" below means: start at the node the word at this+76 points to; a node whose first
// word (its next node) is zero ends the chain and is not examined; a node's event is the word at
// node+116, an event's kind is the int at event+0 and its id the int at event+4.
//   1. emitterNode = the first node whose event has kind 3 and id equal to the int at record+4.
//      Without one, it returns -1.
//   2. a = __builtin_new(184); when non-null it calls __18VISpellActiveEventP12VISpellEventP7VIScene
//      (a, record, the word at this+20). Stores the 64-bit GetTime__7VIScene(scene) at a+176 (also
//      when a is null).
//   3. old = the first node whose event has kind 6 and id equal to record+4 and for which
//      NameHash__C15VIParticleMotif(Motif__14VIMotifBlender(the word at node+128)) equals the int at
//      record+8 (the hash is computed for every kind-6 node with that id until one matches).
//   4. With old: stores the word old+128 at a+128, then calls Factor__C14VIMotifBlender on that blender
//      (before old+128 is cleared), stores zero at old+128 and the float result at a+156,
//      calls Remove__7VILNode(old) and AddTail__7VILListP7VILNode(this+88, old); then, as the last
//      step, AddTail(this+76, a).
//      Without old: p = __builtin_new(28); when non-null calls __14VIMotifBlender(p); stores p at a+128;
//      calls LinkMotifBlender__17VIParticleEmitterUiR14VIMotifBlender(the word at emitterNode+120,
//      the int at record+8, p); stores 0.0f at a+156; AddTail(this+76, a).
//   5. Returns 0.
// Inferred: record type 6 of the event table. It attaches a motif (named by a hash) to the emitter
// started by the earlier record 3 with the same id: the first time it links a fresh blender to the
// emitter; when an active motif event with that id and that hash exists it is retired to the second
// list (+88) and its blender is handed on to the new event, so the motif change continues from the old
// blend factor.
// Unknown: what the hash is a hash of, what Factor returns beyond a float blend factor, and the
// return value's meaning for the caller. Nothing here ran on the EE.
extern "C" {
void* __builtin_new(unsigned size);
void __18VISpellActiveEventP12VISpellEventP7VIScene(void* self, void* event, void* scene);
long long GetTime__7VIScene(void* scene);
void* Motif__14VIMotifBlender(void* blender);
int NameHash__C15VIParticleMotif(void* motif);
float Factor__C14VIMotifBlender(void* blender);
void Remove__7VILNode(void* node);
void AddTail__7VILListP7VILNode(void* list, void* node);
void __14VIMotifBlender(void* blender);
void LinkMotifBlender__17VIParticleEmitterUiR14VIMotifBlender(void* emitter, int index, void* blender);

int NewSetEmitterMotif__18VISpellEventDriverP12VISpellEvent(unsigned char* self, unsigned char* record) {
    unsigned char* node = *reinterpret_cast<unsigned char**>(self + 76);
    unsigned char* next = *reinterpret_cast<unsigned char**>(node);
    unsigned char* emitterNode = 0;
    int id = *reinterpret_cast<int*>(record + 4);
    while (next != 0) {
        unsigned char* event = *reinterpret_cast<unsigned char**>(node + 116);
        if (*reinterpret_cast<int*>(event + 4) == id && *reinterpret_cast<int*>(event) == 3) {
            emitterNode = node;
            break;
        }
        node = next;
        next = *reinterpret_cast<unsigned char**>(node);
    }
    if (emitterNode == 0) {
        return -1;
    }
    unsigned char* a = static_cast<unsigned char*>(__builtin_new(184));
    if (a != 0) {
        __18VISpellActiveEventP12VISpellEventP7VIScene(a, record, *reinterpret_cast<void**>(self + 20));
    }
    *reinterpret_cast<long long*>(a + 176) = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
    int kind = *reinterpret_cast<int*>(record + 4);
    int hash = *reinterpret_cast<int*>(record + 8);
    node = *reinterpret_cast<unsigned char**>(self + 76);
    next = *reinterpret_cast<unsigned char**>(node);
    unsigned char* old = 0;
    while (next != 0) {
        unsigned char* event = *reinterpret_cast<unsigned char**>(node + 116);
        if (*reinterpret_cast<int*>(event) == 6 && *reinterpret_cast<int*>(event + 4) == kind) {
            if (NameHash__C15VIParticleMotif(Motif__14VIMotifBlender(*reinterpret_cast<void**>(node + 128))) == hash) {
                old = node;
                break;
            }
        }
        node = next;
        next = *reinterpret_cast<unsigned char**>(node);
    }
    if (old != 0) {
        *reinterpret_cast<void**>(a + 128) = *reinterpret_cast<void**>(old + 128);
        float factor = Factor__C14VIMotifBlender(*reinterpret_cast<void**>(old + 128));
        *reinterpret_cast<unsigned*>(old + 128) = 0;
        *reinterpret_cast<float*>(a + 156) = factor;
        Remove__7VILNode(old);
        AddTail__7VILListP7VILNode(self + 88, old);
    } else {
        void* blender = __builtin_new(28);
        if (blender != 0) {
            __14VIMotifBlender(blender);
        }
        *reinterpret_cast<void**>(a + 128) = blender;
        LinkMotifBlender__17VIParticleEmitterUiR14VIMotifBlender(*reinterpret_cast<void**>(emitterNode + 120), *reinterpret_cast<int*>(record + 8), blender);
        *reinterpret_cast<float*>(a + 156) = 0.0f;
    }
    AddTail__7VILListP7VILNode(self + 76, a);
    return 0;
}
}
