// Observed: NewCreateSprite__18VISpellEventDriverP12VISpellEvent at 0x0117e830 is 464 bytes in the
// pinned SLUS-20565 image. Arguments: (this, record). Result: -1 in one case, otherwise 0.
//   1. If the int at this+24 is -1 it returns 0 and does nothing else.
//   2. a = __builtin_new(184); when non-null it calls __18VISpellActiveEventP12VISpellEventP7VIScene
//      (a, record, the word at this+20 = scene). Stores the 64-bit GetTime__7VIScene(scene) at a+176
//      (also when a is null).
//   3. sp = Sprite__7VIScenei(scene, the int n at record+8). When the int at sp+52 is 4 it replaces
//      n with CopySprite__7VISceneii(scene, n, 0) and, if that is negative, returns -1 (the active
//      event allocated above is left unlinked).
//   4. s = Sprite__7VIScenei(scene, n). When the int at s+52 is 4: EraseAllPlay__9VIHSpriteR7VIScene
//      (s, scene); then with flags = the int at record+12: if flags & 0x40000,
//      AddPlay__9VIHSpritei14VIPlaybackTypeR7VIScene(s, the int at s+388, 1, scene); else if
//      flags & 0x80000 the same with 0; else AddPlay__9VIHSpriteiR7VIScene(s, the int at s+388, scene).
//   5. actor = CreateActor__7VISceneRC7VIVect3T1fi(scene, &zero3, &zero3, 1.0f, n) with two stack
//      triples of 0.0f; stores it at a+140. SetActorCollideable__7VISceneii(scene, actor, 0) and a
//      GetActorSpritePtr__C7VIScenei(scene, the int at a+140) whose result is not used. Then
//      AddTail__7VILListP7VILNode(this+76, a), ProcessCreateSprite__18VISpellEventDriverP18VISpell
//      ActiveEventb(this, a, 1) and returns 0.
// Inferred: record type 9 of the event table (the matching NewDestroySprite releases a +140 actor).
// A sprite whose kind word is 4 is a sprite with playback (animation) lists: it is copied first, its
// plays are cleared and one is added in the mode the flags select. The copy index is used for the actor.
// Unknown: the meaning of kind 4, the play types 0 and 1, the unused GetActorSpritePtr result, and
// the int at s+388. Nothing here ran on the EE.
struct Vec3 {
    float x, y, z;
};
extern "C" {
void* __builtin_new(unsigned size);
void __18VISpellActiveEventP12VISpellEventP7VIScene(void* self, void* event, void* scene);
long long GetTime__7VIScene(void* scene);
unsigned char* Sprite__7VIScenei(void* scene, int index);
int CopySprite__7VISceneii(void* scene, int index, int flag);
void EraseAllPlay__9VIHSpriteR7VIScene(void* sprite, void* scene);
void AddPlay__9VIHSpritei14VIPlaybackTypeR7VIScene(void* sprite, int anim, int type, void* scene);
void AddPlay__9VIHSpriteiR7VIScene(void* sprite, int anim, void* scene);
int CreateActor__7VISceneRC7VIVect3T1fi(void* scene, const Vec3* position, const Vec3* velocity, float amount, int kind);
void SetActorCollideable__7VISceneii(void* scene, int actor, int flag);
void* GetActorSpritePtr__C7VIScenei(void* scene, int actor);
void AddTail__7VILListP7VILNode(void* list, void* node);
void ProcessCreateSprite__18VISpellEventDriverP18VISpellActiveEventb(void* driver, void* node, int flag);

int NewCreateSprite__18VISpellEventDriverP12VISpellEvent(unsigned char* self, unsigned char* record) {
    if (*reinterpret_cast<int*>(self + 24) == -1) {
        return 0;
    }
    unsigned char* a = static_cast<unsigned char*>(__builtin_new(184));
    if (a != 0) {
        __18VISpellActiveEventP12VISpellEventP7VIScene(a, record, *reinterpret_cast<void**>(self + 20));
    }
    *reinterpret_cast<long long*>(a + 176) = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
    int n;
    if (*reinterpret_cast<int*>(Sprite__7VIScenei(*reinterpret_cast<void**>(self + 20), *reinterpret_cast<int*>(record + 8)) + 52) == 4) {
        n = CopySprite__7VISceneii(*reinterpret_cast<void**>(self + 20), *reinterpret_cast<int*>(record + 8), 0);
        if (n < 0) {
            return -1;
        }
    } else {
        n = *reinterpret_cast<int*>(record + 8);
    }
    unsigned char* sprite = Sprite__7VIScenei(*reinterpret_cast<void**>(self + 20), n);
    if (*reinterpret_cast<int*>(sprite + 52) == 4) {
        EraseAllPlay__9VIHSpriteR7VIScene(sprite, *reinterpret_cast<void**>(self + 20));
        int flags = *reinterpret_cast<int*>(record + 12);
        if ((flags & 0x40000) != 0) {
            AddPlay__9VIHSpritei14VIPlaybackTypeR7VIScene(sprite, *reinterpret_cast<int*>(sprite + 388), 1, *reinterpret_cast<void**>(self + 20));
        } else if ((flags & 0x80000) != 0) {
            AddPlay__9VIHSpritei14VIPlaybackTypeR7VIScene(sprite, *reinterpret_cast<int*>(sprite + 388), 0, *reinterpret_cast<void**>(self + 20));
        } else {
            AddPlay__9VIHSpriteiR7VIScene(sprite, *reinterpret_cast<int*>(sprite + 388), *reinterpret_cast<void**>(self + 20));
        }
    }
    Vec3 position = {0.0f, 0.0f, 0.0f};
    Vec3 velocity = {0.0f, 0.0f, 0.0f};
    int actor = CreateActor__7VISceneRC7VIVect3T1fi(*reinterpret_cast<void**>(self + 20), &position, &velocity, 1.0f, n);
    *reinterpret_cast<int*>(a + 140) = actor;
    SetActorCollideable__7VISceneii(*reinterpret_cast<void**>(self + 20), actor, 0);
    GetActorSpritePtr__C7VIScenei(*reinterpret_cast<void**>(self + 20), *reinterpret_cast<int*>(a + 140));
    AddTail__7VILListP7VILNode(self + 76, a);
    ProcessCreateSprite__18VISpellEventDriverP18VISpellActiveEventb(self, a, 1);
    return 0;
}
}
