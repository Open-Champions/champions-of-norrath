// Observed: ProcessWaitEvent__18VISpellEventDriver at 0x01181970 is 852 bytes in the pinned
// SLUS-20565 image. Argument: this. w = the word at this+32 (the active wait event); ev = the word at
// w+116 (its script record); the wait kind is the unsigned int at ev+4. Returns -1 when w is zero;
// every other path returns 0. Kinds above 4 do nothing. "Finish" below means: when the word at this+32
// is non-zero, ReleaseActor__7VIScenei(the word at node+16, the int at node+140) when that int is not
// -1, the same for node+144, _$_17VIParticleEmitter(it, 3) for the non-zero words at node+120 and
// node+124, _$_14VIMotifBlender(it, 3) for the one at node+128 and __builtin_delete(node); then the word
// at this+32 is set to zero. Elapsed times are the low 32 bits of a 64-bit difference taken as a signed
// int. scene = the word at this+20; id = the int at this+24.
//   0  (wait for the action): with id not -1 and sprite = GetActorSpritePtr__C7VIScenei(scene, id)
//      non-null and the int at sprite+52 equal to 5 and the int at this+52 not -1, finish only when
//      IsActionComplete__9VICSpritei(sprite, the int at this+52) is non-zero; in every other case
//      (id -1, no sprite, another sprite kind, no action) it finishes at once.
//   1  (wait for a time): finish when the int at ev+8 is below GetTime__7VIScene(scene) minus the 64-bit
//      value at w+176.
//   2  and 4 (wait for the effect): e = the word at this+16; finish when the int at e+64 (kind 2) or
//      e+68 (kind 4) is below GetTime(scene) minus the 64-bit value at e+56.
//   3  (wait for a trigger or a time): with id not -1 and a non-null sprite whose int at +52 is 5: slot =
//      the int at this + 56 + 4 * (the int at ev+8); when slot is not -1 and HasTripped__9VIHSpritei
//      (sprite, slot) is non-zero it finishes; otherwise it finishes when the int at ev+12 is below
//      GetTime(scene) minus the 64-bit value at w+176. With id -1, no sprite or another sprite kind it
//      finishes at once.
// Inferred: the "wait" record (type 1 of the event table) that Process starts at +32: the driver does not
// advance its timeline until the wait is over; kind 0 waits for the sprite action of NewSetAction or
// NewPlayAttack (+52), kind 3 for one of the four named triggers (+56..+68) or a timeout, kinds 2 and 4
// for the effect driver's own timer (+56/+64/+68 of the effect driver).
// Unknown: the meaning of the effect driver's +56, +64 and +68 beyond the timer role, and whether the
// returned -1 is read anywhere. Nothing here ran on the EE.
extern "C" {
void* GetActorSpritePtr__C7VIScenei(void* scene, int actor);
int IsActionComplete__9VICSpritei(void* sprite, int action);
int HasTripped__9VIHSpritei(void* sprite, int trigger);
long long GetTime__7VIScene(void* scene);
void ReleaseActor__7VIScenei(void* scene, int actor);
void _$_17VIParticleEmitter(void* emitter, int flags);
void _$_14VIMotifBlender(void* blender, int flags);
void __builtin_delete(void* pointer);
}

static inline void finishWait(unsigned char* self) {
    unsigned char* node = *reinterpret_cast<unsigned char**>(self + 32);
    if (node != 0) {
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
    *reinterpret_cast<unsigned*>(self + 32) = 0;
}

extern "C" int ProcessWaitEvent__18VISpellEventDriver(unsigned char* self) {
    unsigned char* w = *reinterpret_cast<unsigned char**>(self + 32);
    if (w == 0) {
        return -1;
    }
    unsigned kind = *reinterpret_cast<unsigned*>(*reinterpret_cast<unsigned char**>(w + 116) + 4);
    if (kind > 4) {
        return 0;
    }
    switch (kind) {
    case 0: {
        int id = *reinterpret_cast<int*>(self + 24);
        if (id != -1) {
            unsigned char* sprite = static_cast<unsigned char*>(GetActorSpritePtr__C7VIScenei(*reinterpret_cast<void**>(self + 20), id));
            if (sprite != 0 && *reinterpret_cast<int*>(sprite + 52) == 5) {
                int action = *reinterpret_cast<int*>(self + 52);
                if (action != -1 && IsActionComplete__9VICSpritei(sprite, action) == 0) {
                    return 0;
                }
            }
        }
        finishWait(self);
        return 0;
    }
    case 1: {
        long long now = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
        w = *reinterpret_cast<unsigned char**>(self + 32);
        if (*reinterpret_cast<int*>(*reinterpret_cast<unsigned char**>(w + 116) + 8) < static_cast<int>(now - *reinterpret_cast<long long*>(w + 176))) {
            finishWait(self);
        }
        return 0;
    }
    case 2: {
        long long now = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
        unsigned char* effect = *reinterpret_cast<unsigned char**>(self + 16);
        if (*reinterpret_cast<int*>(effect + 64) < static_cast<int>(now - *reinterpret_cast<long long*>(effect + 56))) {
            finishWait(self);
        }
        return 0;
    }
    case 3: {
        int id = *reinterpret_cast<int*>(self + 24);
        if (id != -1) {
            unsigned char* sprite = static_cast<unsigned char*>(GetActorSpritePtr__C7VIScenei(*reinterpret_cast<void**>(self + 20), id));
            if (sprite != 0 && *reinterpret_cast<int*>(sprite + 52) == 5) {
                w = *reinterpret_cast<unsigned char**>(self + 32);
                int slot = *reinterpret_cast<int*>(self + 56 + 4 * *reinterpret_cast<int*>(*reinterpret_cast<unsigned char**>(w + 116) + 8));
                if (slot != -1 && HasTripped__9VIHSpritei(sprite, slot) != 0) {
                    finishWait(self);
                    return 0;
                }
                long long now = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
                w = *reinterpret_cast<unsigned char**>(self + 32);
                if (*reinterpret_cast<int*>(*reinterpret_cast<unsigned char**>(w + 116) + 12) < static_cast<int>(now - *reinterpret_cast<long long*>(w + 176))) {
                    finishWait(self);
                }
                return 0;
            }
        }
        finishWait(self);
        return 0;
    }
    default: {
        long long now = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
        unsigned char* effect = *reinterpret_cast<unsigned char**>(self + 16);
        if (*reinterpret_cast<int*>(effect + 68) < static_cast<int>(now - *reinterpret_cast<long long*>(effect + 56))) {
            finishWait(self);
        }
        return 0;
    }
    }
}
