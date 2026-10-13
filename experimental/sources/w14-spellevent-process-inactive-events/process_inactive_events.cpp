// Observed: ProcessInactiveEvents__18VISpellEventDriver at 0x01181cc8 is 296 bytes in the pinned
// SLUS-20565 image. Argument: this. Always returns 0. It walks the chain whose head node is the word
// at this+88 (the second list; each node's first word is the next node and the chain ends at a node
// whose first word is zero, which is not visited). For each visited node n:
//   - when the word at n+120 (an emitter) is non-zero and ParticleCount__C17VIParticleEmitter of it is
//     above zero: if the int at (the word at n+116)+0 is 3 it calls ProcessCreateEmitter__18VISpell
//     EventDriverP18VISpellActiveEventb(this, n, 0); the node stays;
//   - otherwise it calls Remove__7VILNode(n) and releases it: when the word at n+140 is not -1,
//     ReleaseActor__7VIScenei(the word at n+16, that value); the same for n+144; when the word at
//     n+120 is non-zero _$_17VIParticleEmitter(it, 3); the same for n+124; when the word at n+128 is
//     non-zero _$_14VIMotifBlender(it, 3); then __builtin_delete(n). The next node is taken before
//     any of that.
// Inferred: the second list holds events that were ended (NewSetEmitterMotif moves an old motif event
// there): their emitters are kept until the last particle is gone, and a still-running emitter event
// of type 3 is stepped once more with the flag 0; a finished one is destroyed.
// Unknown: what the flag 0 to ProcessCreateEmitter does exactly (see that function), and what
// ParticleCount counts. Nothing here ran on the EE.
extern "C" {
int ParticleCount__C17VIParticleEmitter(void* emitter);
void ProcessCreateEmitter__18VISpellEventDriverP18VISpellActiveEventb(void* driver, void* node, int flag);
void Remove__7VILNode(void* node);
void ReleaseActor__7VIScenei(void* scene, int actor);
void _$_17VIParticleEmitter(void* emitter, int flags);
void _$_14VIMotifBlender(void* blender, int flags);
void __builtin_delete(void* pointer);

int ProcessInactiveEvents__18VISpellEventDriver(unsigned char* self) {
    unsigned char* node = *reinterpret_cast<unsigned char**>(self + 88);
    unsigned char* next = *reinterpret_cast<unsigned char**>(node);
    while (next != 0) {
        void* emitter = *reinterpret_cast<void**>(node + 120);
        if (emitter != 0 && ParticleCount__C17VIParticleEmitter(emitter) > 0) {
            if (**reinterpret_cast<int**>(node + 116) == 3) {
                ProcessCreateEmitter__18VISpellEventDriverP18VISpellActiveEventb(self, node, 0);
            }
        } else {
            Remove__7VILNode(node);
            if (*reinterpret_cast<int*>(node + 140) != -1) {
                ReleaseActor__7VIScenei(*reinterpret_cast<void**>(node + 16), *reinterpret_cast<int*>(node + 140));
            }
            if (*reinterpret_cast<int*>(node + 144) != -1) {
                ReleaseActor__7VIScenei(*reinterpret_cast<void**>(node + 16), *reinterpret_cast<int*>(node + 144));
            }
            if (*reinterpret_cast<void**>(node + 120) != 0) {
                _$_17VIParticleEmitter(*reinterpret_cast<void**>(node + 120), 3);
            }
            if (*reinterpret_cast<void**>(node + 124) != 0) {
                _$_17VIParticleEmitter(*reinterpret_cast<void**>(node + 124), 3);
            }
            if (*reinterpret_cast<void**>(node + 128) != 0) {
                _$_14VIMotifBlender(*reinterpret_cast<void**>(node + 128), 3);
            }
            __builtin_delete(node);
        }
        node = next;
        next = *reinterpret_cast<unsigned char**>(node);
    }
    return 0;
}
}
