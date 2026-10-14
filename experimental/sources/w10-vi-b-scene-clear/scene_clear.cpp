// Observed Clear__7VIScene at 0x0113afc8, 836 bytes. Offsets are bytes from
// the scene; dictionary and raster below are the words at 19352 and 19356.
// Result: -1 as soon as one of the checked steps returns a negative value,
// otherwise zero.
//  1. Stores 2500.0f (0x451c4000) at 6268; -1 at 6276, 1124, 1132, 1128, 1136,
//     1140, 1144 and 6272; a 64-bit zero at 1152; zero at 0, 19368 and 10772;
//     a 64-bit zero at 10808 and zero at 10816; then GlobalFree__14VIEffectVolume.
//  2. Clear__19VIRadialFloraSystem(+10876), Clear__17VISceneSoundTrack(+6800),
//     VITrailFxClear(raster, dictionary) when the raster is nonzero,
//     Clear__19VISpellEffectSystem(+7024), Clear__12VIAtmosphere(+1600, scene)
//     when the raster is nonzero.
//  3. Checked steps in order, each returning -1 when negative:
//     Clear__16VIParticleSystem(+1352, dictionary); Clear VIList<VIActor>
//     (+972); Clear VIList<VISceneOccup...> (+984).
//  4. VIPool<VISprite*> at +996: each nonzero entry has its virtual function
//     at vtable slot 0 (the vtable pointer is at +60 of the sprite, slots are
//     8 bytes starting 8 bytes in, a 16-bit adjust then the function pointer)
//     called with the adjusted object and flag 3; then the pool is cleared
//     (checked).
//  5. VIPool<VIStaticLighting*> at +1068: each nonzero entry gets the
//     VIArray<int> destructor (flag 2) on its member at +8 and
//     __builtin_delete; the pool is cleared (checked).
//  6. Stores -1 at 1096 and 1092. VIPool<VIRefMap*> at +1044: each nonzero
//     entry gets the VIMap<int,int> destructor (flag 2) on +8 and
//     __builtin_delete; the pool is cleared (checked).
//  7. VIPool<VIHSpriteAnim*> at +1020: each nonzero entry gets the
//     VIHSpriteAnim destructor with flag 3; the pool is cleared (checked).
//  8. Checked: Clear VIVector<VISceneDisplayElem> (+1112); Clear__11VIClipStack
//     (+1296); Clear__14VIQueryTriList (+6128). Then zero at 6792, then
//     checked Clear VIArray<int> (+1104), then Clear__7VIWorld (+4).
//  9. Only when that last step is nonnegative: zero at 19364, 19352, 19356 and
//     19360, and the result is zero.
// A pool entry is 12 bytes in the array whose address is the word at pool+8;
// the pointer is its first word. Begin/Next/End iterate by index. What the
// other two words of an entry hold is not established here.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
extern "C" void norrath_host_call_destructor(unsigned function, char* object, int flags);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#define NORRATH_CALL_DESTRUCTOR(function, object, flags) \
    norrath_host_call_destructor((function), (object), (flags))
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#define NORRATH_CALL_DESTRUCTOR(function, object, flags)                                     \
    reinterpret_cast<void (*)(void*, int)>(static_cast<unsigned long>(function))((object), (flags))
#endif

extern "C" {
struct VIScene;
struct Member;

void GlobalFree__14VIEffectVolume();
int Clear__19VIRadialFloraSystem(Member* member);
int Clear__17VISceneSoundTrack(Member* member);
void VITrailFxClear__FR8VIRasterR12VIDictionary(void* raster, void* dictionary);
int Clear__19VISpellEffectSystem(Member* member);
int Clear__12VIAtmosphereR7VIScene(Member* member, VIScene* scene);
int Clear__16VIParticleSystemR12VIDictionary(Member* member, void* dictionary);
int Clear__t6VIList1Z7VIActor(Member* member);
int Clear__t6VIList1ZQ27VIScene12VISceneOccup(Member* member);
int Begin__Ct6VIPool1ZP8VISprite(const Member* pool);
int Next__Ct6VIPool1ZP8VISpritei(const Member* pool, int index);
int End__Ct6VIPool1ZP8VISprite(const Member* pool);
int Clear__t6VIPool1ZP8VISprite(Member* pool);
int Begin__Ct6VIPool1ZP16VIStaticLighting(const Member* pool);
int Next__Ct6VIPool1ZP16VIStaticLightingi(const Member* pool, int index);
int End__Ct6VIPool1ZP16VIStaticLighting(const Member* pool);
int Clear__t6VIPool1ZP16VIStaticLighting(Member* pool);
void _$_t7VIArray1Zi(Member* member, int flags);
void __builtin_delete(void* pointer);
int Begin__Ct6VIPool1ZP8VIRefMap(const Member* pool);
int Next__Ct6VIPool1ZP8VIRefMapi(const Member* pool, int index);
int End__Ct6VIPool1ZP8VIRefMap(const Member* pool);
int Clear__t6VIPool1ZP8VIRefMap(Member* pool);
void _$_t5VIMap2ZiZi(Member* member, int flags);
int Begin__Ct6VIPool1ZP13VIHSpriteAnim(const Member* pool);
int Next__Ct6VIPool1ZP13VIHSpriteAnimi(const Member* pool, int index);
int End__Ct6VIPool1ZP13VIHSpriteAnim(const Member* pool);
int Clear__t6VIPool1ZP13VIHSpriteAnim(Member* pool);
void _$_13VIHSpriteAnim(void* anim, int flags);
int Clear__t8VIVector1Z18VISceneDisplayElem(Member* member);
int Clear__11VIClipStack(Member* member);
int Clear__14VIQueryTriList(Member* member);
int Clear__t7VIArray1Zi(Member* member);
int Clear__7VIWorld(Member* member);

static unsigned word_at(const char* address) { return *reinterpret_cast<const unsigned*>(address); }

// The pointer stored in the first word of entry `index` of a pool.
static char* pool_entry(const char* pool, int index) {
    const char* array = NORRATH_TRANSLATE(word_at(pool + 8));
    return NORRATH_TRANSLATE(word_at(array + 12 * index));
}

int Clear__7VIScene(VIScene* scene) {
    char* base = reinterpret_cast<char*>(scene);
    Member* sprites = reinterpret_cast<Member*>(base + 996);
    Member* lights = reinterpret_cast<Member*>(base + 1068);
    Member* maps = reinterpret_cast<Member*>(base + 1044);
    Member* anims = reinterpret_cast<Member*>(base + 1020);

    *reinterpret_cast<unsigned*>(base + 6268) = 0x451c4000u;
    *reinterpret_cast<int*>(base + 6276) = -1;
    *reinterpret_cast<int*>(base + 1124) = -1;
    *reinterpret_cast<int*>(base + 1132) = -1;
    *reinterpret_cast<int*>(base + 1128) = -1;
    *reinterpret_cast<int*>(base + 1136) = -1;
    *reinterpret_cast<int*>(base + 1140) = -1;
    *reinterpret_cast<int*>(base + 1144) = -1;
    *reinterpret_cast<int*>(base + 6272) = -1;
    *reinterpret_cast<unsigned long long*>(base + 1152) = 0;
    *reinterpret_cast<unsigned*>(base) = 0;
    *reinterpret_cast<unsigned*>(base + 19368) = 0;
    *reinterpret_cast<unsigned*>(base + 10772) = 0;
    *reinterpret_cast<unsigned long long*>(base + 10808) = 0;
    *reinterpret_cast<unsigned*>(base + 10816) = 0;
    GlobalFree__14VIEffectVolume();
    Clear__19VIRadialFloraSystem(reinterpret_cast<Member*>(base + 10876));
    Clear__17VISceneSoundTrack(reinterpret_cast<Member*>(base + 6800));
    void* raster = reinterpret_cast<void*>(static_cast<unsigned long>(word_at(base + 19356)));
    void* dictionary = reinterpret_cast<void*>(static_cast<unsigned long>(word_at(base + 19352)));
    if (raster) VITrailFxClear__FR8VIRasterR12VIDictionary(raster, dictionary);
    Clear__19VISpellEffectSystem(reinterpret_cast<Member*>(base + 7024));
    if (word_at(base + 19356) != 0) Clear__12VIAtmosphereR7VIScene(reinterpret_cast<Member*>(base + 1600), scene);
    if (Clear__16VIParticleSystemR12VIDictionary(reinterpret_cast<Member*>(base + 1352), dictionary) < 0) return -1;
    if (Clear__t6VIList1Z7VIActor(reinterpret_cast<Member*>(base + 972)) < 0) return -1;
    if (Clear__t6VIList1ZQ27VIScene12VISceneOccup(reinterpret_cast<Member*>(base + 984)) < 0) return -1;

    for (int i = Begin__Ct6VIPool1ZP8VISprite(sprites); i != End__Ct6VIPool1ZP8VISprite(sprites);
         i = Next__Ct6VIPool1ZP8VISpritei(sprites, i)) {
        char* sprite = pool_entry(base + 996, i);
        if (sprite) {
            const char* slot = NORRATH_TRANSLATE(word_at(sprite + 60)) + 8;
            short adjust = *reinterpret_cast<const short*>(slot);
            NORRATH_CALL_DESTRUCTOR(word_at(slot + 4), sprite + adjust, 3);
        }
    }
    if (Clear__t6VIPool1ZP8VISprite(sprites) < 0) return -1;

    for (int i = Begin__Ct6VIPool1ZP16VIStaticLighting(lights);
         i != End__Ct6VIPool1ZP16VIStaticLighting(lights);
         i = Next__Ct6VIPool1ZP16VIStaticLightingi(lights, i)) {
        char* light = pool_entry(base + 1068, i);
        if (light) {
            _$_t7VIArray1Zi(reinterpret_cast<Member*>(light + 8), 2);
            __builtin_delete(light);
        }
    }
    if (Clear__t6VIPool1ZP16VIStaticLighting(lights) < 0) return -1;

    *reinterpret_cast<int*>(base + 1096) = -1;
    *reinterpret_cast<int*>(base + 1092) = -1;
    for (int i = Begin__Ct6VIPool1ZP8VIRefMap(maps); i != End__Ct6VIPool1ZP8VIRefMap(maps);
         i = Next__Ct6VIPool1ZP8VIRefMapi(maps, i)) {
        char* map = pool_entry(base + 1044, i);
        if (map) {
            _$_t5VIMap2ZiZi(reinterpret_cast<Member*>(map + 8), 2);
            __builtin_delete(map);
        }
    }
    if (Clear__t6VIPool1ZP8VIRefMap(maps) < 0) return -1;

    for (int i = Begin__Ct6VIPool1ZP13VIHSpriteAnim(anims); i != End__Ct6VIPool1ZP13VIHSpriteAnim(anims);
         i = Next__Ct6VIPool1ZP13VIHSpriteAnimi(anims, i)) {
        char* anim = pool_entry(base + 1020, i);
        if (anim) _$_13VIHSpriteAnim(anim, 3);
    }
    if (Clear__t6VIPool1ZP13VIHSpriteAnim(anims) < 0) return -1;

    if (Clear__t8VIVector1Z18VISceneDisplayElem(reinterpret_cast<Member*>(base + 1112)) < 0) return -1;
    if (Clear__11VIClipStack(reinterpret_cast<Member*>(base + 1296)) < 0) return -1;
    if (Clear__14VIQueryTriList(reinterpret_cast<Member*>(base + 6128)) < 0) return -1;
    *reinterpret_cast<unsigned*>(base + 6792) = 0;
    if (Clear__t7VIArray1Zi(reinterpret_cast<Member*>(base + 1104)) < 0) return -1;
    if (Clear__7VIWorld(reinterpret_cast<Member*>(base + 4)) < 0) return -1;
    *reinterpret_cast<unsigned*>(base + 19364) = 0;
    *reinterpret_cast<unsigned*>(base + 19352) = 0;
    *reinterpret_cast<unsigned*>(base + 19356) = 0;
    *reinterpret_cast<unsigned*>(base + 19360) = 0;
    return 0;
}
}
