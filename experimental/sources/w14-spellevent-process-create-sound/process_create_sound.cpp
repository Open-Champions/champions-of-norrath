// Observed: ProcessCreateSound__18VISpellEventDriverP18VISpellActiveEventb at 0x0117f330 is 692 bytes
// in the pinned SLUS-20565 image. Arguments: (this, node, flag). Always returns 0. ev = the word at
// node+116 (the script record). Nothing at all happens unless flag is exactly 1.
//   A. When the int at this+24 is not -1: src = that int; when bit 0x10000 of the int at ev+12 is set,
//      src = the int at (the word at this+16)+8; when bit 0x20000 is set, src = the int at
//      (the word at this+16)+12 (this one wins when both are set). With src not -1 and sprite =
//      GetActorSpritePtr__C7VIScenei(the word at this+20, src) non-null: it builds a 4x4 float matrix
//      m (sixteen zeros on the stack; its destructor _$_10VIMatrix44(m, 2) runs at the end) and a
//      matrix33 object (constructor __10VIMatrix33(p), destructor _$_10VIMatrix33(p, 2)). When the int
//      at sprite+52 is 5 and the unsigned halfword at ev+12 is not 0xffff, it calls
//      CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44(sprite, that
//      halfword, src, scene, &m); otherwise GetActorTransform__7VISceneiR10VIMatrix44(scene, src, &m).
//      It copies the upper-left 3x3 of m (floats 0 1 2 4 5 6 8 9 10) into the matrix33 object, takes
//      the three floats m[12..14] as a position and calls SetActorLocation__7VISceneiRC7VIVect3(scene,
//      the int at node+140, &position).
//   B. Then: when the int at ev+16 is zero it returns. Otherwise t = GetTime__7VIScene(scene) and when
//      the int at ev+16 is below the signed low 32 bits of (t - the 64-bit value at node+176) it
//      removes the node: Remove__7VILNode(node); ReleaseActor__7VIScenei(the word at node+16, the
//      int at node+140) when that int is not -1; the same for node+144; _$_17VIParticleEmitter(it, 3)
//      for the non-zero words at node+120 and node+124; _$_14VIMotifBlender(it, 3) for the one at
//      node+128; __builtin_delete(node).
// Inferred: the per-frame step of a sound event (record type 11): it moves the sound's own scene actor
// to the world position of the caster, target or a slot of the caster's sprite, and ends the event once
// its duration (ev+16) has passed. ev+12 low halfword is a sprite slot id with 0xffff meaning the actor
// origin; bits 0x10000 and 0x20000 select caster or target instead of this driver's own actor.
// Unknown: the flag's meaning when it is not 1 (no caller in the table passes another value), what
// the 3x3 copy is used for (nothing here reads it afterwards; the callee names suggest a rotation),
// and the units of ev+16. Nothing here ran on the EE.
struct Vec3 {
    float x, y, z;
};
extern "C" {
void* GetActorSpritePtr__C7VIScenei(void* scene, int actor);
void __10VIMatrix33(void* matrix);
void _$_10VIMatrix33(void* matrix, int flags);
void _$_10VIMatrix44(void* matrix, int flags);
void CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44(void* sprite, int slot, int actor, void* scene, void* matrix);
void GetActorTransform__7VISceneiR10VIMatrix44(void* scene, int actor, void* matrix);
void SetActorLocation__7VISceneiRC7VIVect3(void* scene, int actor, const Vec3* position);
long long GetTime__7VIScene(void* scene);
void Remove__7VILNode(void* node);
void ReleaseActor__7VIScenei(void* scene, int actor);
void _$_17VIParticleEmitter(void* emitter, int flags);
void _$_14VIMotifBlender(void* blender, int flags);
void __builtin_delete(void* pointer);

int ProcessCreateSound__18VISpellEventDriverP18VISpellActiveEventb(unsigned char* self, unsigned char* node, int flag) {
    int actor = *reinterpret_cast<int*>(self + 24);
    unsigned char* ev = *reinterpret_cast<unsigned char**>(node + 116);
    if (actor != -1) {
        if (flag != 1) {
            return 0;
        }
        int flags = *reinterpret_cast<int*>(ev + 12);
        int src = actor;
        if ((flags & 0x10000) != 0) {
            src = *reinterpret_cast<int*>(*reinterpret_cast<unsigned char**>(self + 16) + 8);
        }
        if ((flags & 0x20000) != 0) {
            src = *reinterpret_cast<int*>(*reinterpret_cast<unsigned char**>(self + 16) + 12);
        }
        if (src != -1) {
            unsigned char* sprite = static_cast<unsigned char*>(GetActorSpritePtr__C7VIScenei(*reinterpret_cast<void**>(self + 20), src));
            if (sprite != 0) {
                alignas(16) float m[16];
                float rotation[9];
                for (int i = 0; i < 16; ++i) {
                    m[i] = 0.0f;
                }
                __10VIMatrix33(rotation);
                if (*reinterpret_cast<int*>(sprite + 52) == 5 && *reinterpret_cast<unsigned short*>(ev + 12) != 0xffff) {
                    CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44(
                        sprite, *reinterpret_cast<unsigned short*>(ev + 12), src, *reinterpret_cast<void**>(self + 20), m);
                } else {
                    GetActorTransform__7VISceneiR10VIMatrix44(*reinterpret_cast<void**>(self + 20), src, m);
                }
                rotation[0] = m[0];
                rotation[1] = m[1];
                rotation[2] = m[2];
                rotation[3] = m[4];
                rotation[4] = m[5];
                rotation[5] = m[6];
                rotation[6] = m[8];
                rotation[7] = m[9];
                rotation[8] = m[10];
                Vec3 position;
                position.x = m[12];
                position.y = m[13];
                position.z = m[14];
                SetActorLocation__7VISceneiRC7VIVect3(*reinterpret_cast<void**>(self + 20), *reinterpret_cast<int*>(node + 140), &position);
                _$_10VIMatrix33(rotation, 2);
                _$_10VIMatrix44(m, 2);
            }
        }
    }
    if (flag != 1) {
        return 0;
    }
    if (*reinterpret_cast<int*>(ev + 16) == 0) {
        return 0;
    }
    long long now = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
    if (*reinterpret_cast<int*>(ev + 16) < static_cast<int>(now - *reinterpret_cast<long long*>(node + 176))) {
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
    return 0;
}
}
