// Observed: NewCreateSound__18VISpellEventDriverP12VISpellEvent at 0x0117e5e8 is 336 bytes in the
// pinned SLUS-20565 image. Arguments: (this, record). Always returns 0.
//   1. If the int at this+24 is -1 it returns 0 and does nothing else.
//   2. a = __builtin_new(184); when non-null it calls __18VISpellActiveEventP12VISpellEventP7VIScene
//      (a, record, the word at this+20). Then stores the 64-bit GetTime__7VIScene(scene) at a+176
//      (also when a is null).
//   3. actor = CreateActor__7VISceneRC7VIVect3T1fi(scene, &zero3, &zero3, the float at record+20, the
//      int at record+8), with two stack triples of 0.0f; stores it at a+140.
//   4. sprite = GetActorSpritePtr__C7VIScenei(scene, actor). flags = the int at record+12.
//      EnablePan__13VISoundSpriteb(sprite, (flags >> 18) & 1). SetAttenuationMode__13VISoundSprite
//      Q213VISoundSprite15ATTENUATIONMODE(sprite, m) with m = 1 when flags & 0x80000, else 2 when
//      flags & 0x100000, else 0.
//   5. AddTail__7VILListP7VILNode(this+76, a); ProcessCreateSound__18VISpellEventDriverP18VISpell
//      ActiveEventb(this, a, 1); returns 0.
// Inferred: record type 11 of the event table: starts a sound as a scene actor with a sound sprite and
// links the active event into the driver's list at +76, where Process calls ProcessCreateSound for it
// each frame. The 184-byte active event and its +140 actor handle match NewDestroySound.
// Unknown: the units of the float at record+20, the meaning of the int at record+8 (passed as the
// last CreateActor argument), the attenuation mode names, and the other flag bits. Nothing here ran on
// the EE.
struct Vec3 {
    float x, y, z;
};
extern "C" {
void* __builtin_new(unsigned size);
void __18VISpellActiveEventP12VISpellEventP7VIScene(void* self, void* event, void* scene);
long long GetTime__7VIScene(void* scene);
int CreateActor__7VISceneRC7VIVect3T1fi(void* scene, const Vec3* position, const Vec3* velocity, float amount, int kind);
void* GetActorSpritePtr__C7VIScenei(void* scene, int actor);
void EnablePan__13VISoundSpriteb(void* sprite, int enable);
void SetAttenuationMode__13VISoundSpriteQ213VISoundSprite15ATTENUATIONMODE(void* sprite, int mode);
void AddTail__7VILListP7VILNode(void* list, void* node);
void ProcessCreateSound__18VISpellEventDriverP18VISpellActiveEventb(void* driver, void* node, int flag);

int NewCreateSound__18VISpellEventDriverP12VISpellEvent(unsigned char* self, unsigned char* record) {
    if (*reinterpret_cast<int*>(self + 24) == -1) {
        return 0;
    }
    unsigned char* a = static_cast<unsigned char*>(__builtin_new(184));
    if (a != 0) {
        __18VISpellActiveEventP12VISpellEventP7VIScene(a, record, *reinterpret_cast<void**>(self + 20));
    }
    *reinterpret_cast<long long*>(a + 176) = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
    Vec3 position = {0.0f, 0.0f, 0.0f};
    Vec3 velocity = {0.0f, 0.0f, 0.0f};
    int actor = CreateActor__7VISceneRC7VIVect3T1fi(*reinterpret_cast<void**>(self + 20), &position, &velocity,
                                                    *reinterpret_cast<float*>(record + 20), *reinterpret_cast<int*>(record + 8));
    *reinterpret_cast<int*>(a + 140) = actor;
    void* sprite = GetActorSpritePtr__C7VIScenei(*reinterpret_cast<void**>(self + 20), actor);
    int flags = *reinterpret_cast<int*>(record + 12);
    EnablePan__13VISoundSpriteb(sprite, (flags >> 18) & 1);
    if ((flags & 0x80000) != 0) {
        SetAttenuationMode__13VISoundSpriteQ213VISoundSprite15ATTENUATIONMODE(sprite, 1);
    } else if ((flags & 0x100000) != 0) {
        SetAttenuationMode__13VISoundSpriteQ213VISoundSprite15ATTENUATIONMODE(sprite, 2);
    } else {
        SetAttenuationMode__13VISoundSpriteQ213VISoundSprite15ATTENUATIONMODE(sprite, 0);
    }
    AddTail__7VILListP7VILNode(self + 76, a);
    ProcessCreateSound__18VISpellEventDriverP18VISpellActiveEventb(self, a, 1);
    return 0;
}
}
