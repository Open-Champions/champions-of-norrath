// Observed Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide
// at 0x0113ac28, 924 bytes. Arguments after the scene: a 64-bit value (the
// original calls it a long), the dictionary, the raster, the sound device and
// the collide object. Result: -1 as soon as a checked step returns a negative
// value, otherwise zero. Offsets are bytes from the scene.
//  1. Stores the 64-bit value at 1152 and the four pointers at 19352, 19356,
//     19360 and 19364.
//  2. Checked: Init VIList<VIActor> (+972); Init VIList<VISceneOccup...>
//     (+984); Init(+996, 0) of VIPool<VISprite*>; Init(+1020, 0) of
//     VIPool<VIHSpriteAnim*>; Init(+1044, 0) of VIPool<VIRefMap*>;
//     Init(+1068, 0) of VIPool<VIStaticLighting*>.
//  3. Init__7VIWorld(+4, 1, 1, 1, 4, 2000.0f, origin, box) with the origin
//     (0, 0, 0) and the box from (-1000, -300, -1000) to (1000, 300, 1000);
//     checked.
//  4. Stores CreateSprite__7VIScene12VISpriteTypei(scene, 2, 1) at 1124;
//     returns -1 if that is negative.
//  5. Identity__10VIMatrix44 on +1168 and +1232.
//  6. Checked: Init__11VIClipStack(+1296); Init__t7VIArray1Zii(+1104, 5000);
//     Init__14VIQueryTriList(+6128, 200, zero box, (0, 1, 0), 80.0f).
//  7. Zero at 6792; -1 at the 32 words 1464..1591; zero at 1592; -1 at 1596.
//  8. Init__16VIParticleSystem(+1352, value, raster, 2000), result ignored.
//     When the raster is nonzero Init__12VIAtmosphere(+1600, value, raster,
//     600), result ignored. Init__19VISpellEffectSystem(+7024, scene), result
//     ignored. When the raster is nonzero VITrailFxInit(raster, dictionary).
//  9. InitTimeOfDay__7VIScene(scene), checked.
// 10. SetPersonalLightMode__7VIScene(scene, 0); SetPersonalLight(scene,
//     origin, 20.0f, color (1, 1, 1, 1)); Init__17VISceneSoundTrack(+6800).
// 11. Stores floats 950, 1050, 900, 550, 500 at 10844, 10840, 10848, 10852,
//     10856 and 950, 900, 550, 500 at 10824..10836; 64-bit zero at 10864; zero
//     at 10872.
// 12. Init__19VIRadialFloraSystem(+10876, scene). If that is negative the
//     result is -1. Otherwise 1 is stored at 19348, GlobalInit__14VIEffectVolume
//     (scene) is called and the result is zero.
// The meaning of the constants (boxes, sizes, distances) is not established
// here; the reference arguments are passed as the address of stack copies.
static void store_pointer(char* where, const void* value) {
    *reinterpret_cast<unsigned*>(where) = static_cast<unsigned>(reinterpret_cast<unsigned long>(value));
}

extern "C" {
struct VIScene;
struct Member;

struct Vect3 {
    float x, y, z;
};
struct Box {
    Vect3 min, max;
};
struct Color {
    float r, g, b, a;
};

int Init__t6VIList1Z7VIActor(Member* member);
int Init__t6VIList1ZQ27VIScene12VISceneOccup(Member* member);
int Init__t6VIPool1ZP8VISpritei(Member* pool, int count);
int Init__t6VIPool1ZP13VIHSpriteAnimi(Member* pool, int count);
int Init__t6VIPool1ZP8VIRefMapi(Member* pool, int count);
int Init__t6VIPool1ZP16VIStaticLightingi(Member* pool, int count);
int Init__7VIWorldiiiifRC7VIVect3RC6VIBBox(Member* world, int a, int b, int c, int d, float size,
                                           const Vect3* origin, const Box* box);
int CreateSprite__7VIScene12VISpriteTypei(VIScene* scene, int type, int flag);
void Identity__10VIMatrix44(Member* matrix);
int Init__11VIClipStack(Member* member);
int Init__t7VIArray1Zii(Member* member, int count);
int Init__14VIQueryTriListiRC6VIBBoxRC7VIVect3f(Member* member, int count, const Box* box,
                                                const Vect3* vector, float distance);
int Init__16VIParticleSystemlP8VIRasteri(Member* member, long long value, void* raster, int count);
int Init__12VIAtmospherelR8VIRasteri(Member* member, long long value, void* raster, int count);
int Init__19VISpellEffectSystemP7VIScene(Member* member, VIScene* scene);
void VITrailFxInit__FR8VIRasterR12VIDictionary(void* raster, void* dictionary);
int InitTimeOfDay__7VIScene(VIScene* scene);
void SetPersonalLightMode__7VIScene19VIPersonalLightMode(VIScene* scene, int mode);
void SetPersonalLight__7VISceneRC7VIVect3fRC10VIColor32F(VIScene* scene, const Vect3* origin,
                                                         float distance, const Color* color);
void Init__17VISceneSoundTrack(Member* member);
int Init__19VIRadialFloraSystemR7VIScene(Member* member, VIScene* scene);
void GlobalInit__14VIEffectVolumeP7VIScene(VIScene* scene);

int Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(
    VIScene* scene, long long value, void* dictionary, void* raster, void* sound, void* collide) {
    char* base = reinterpret_cast<char*>(scene);
    *reinterpret_cast<long long*>(base + 1152) = value;
    store_pointer(base + 19352, dictionary);
    store_pointer(base + 19356, raster);
    store_pointer(base + 19360, sound);
    store_pointer(base + 19364, collide);
    if (Init__t6VIList1Z7VIActor(reinterpret_cast<Member*>(base + 972)) < 0) return -1;
    if (Init__t6VIList1ZQ27VIScene12VISceneOccup(reinterpret_cast<Member*>(base + 984)) < 0) return -1;
    if (Init__t6VIPool1ZP8VISpritei(reinterpret_cast<Member*>(base + 996), 0) < 0) return -1;
    if (Init__t6VIPool1ZP13VIHSpriteAnimi(reinterpret_cast<Member*>(base + 1020), 0) < 0) return -1;
    if (Init__t6VIPool1ZP8VIRefMapi(reinterpret_cast<Member*>(base + 1044), 0) < 0) return -1;
    if (Init__t6VIPool1ZP16VIStaticLightingi(reinterpret_cast<Member*>(base + 1068), 0) < 0) return -1;

    Vect3 origin = {0.0f, 0.0f, 0.0f};
    Box world_box;
    world_box.min = {-1000.0f, -300.0f, -1000.0f};
    world_box.max = {1000.0f, 300.0f, 1000.0f};
    if (Init__7VIWorldiiiifRC7VIVect3RC6VIBBox(reinterpret_cast<Member*>(base + 4), 1, 1, 1, 4,
                                               2000.0f, &origin, &world_box) < 0)
        return -1;
    int sprite = CreateSprite__7VIScene12VISpriteTypei(scene, 2, 1);
    *reinterpret_cast<int*>(base + 1124) = sprite;
    if (sprite < 0) return -1;
    Identity__10VIMatrix44(reinterpret_cast<Member*>(base + 1168));
    Identity__10VIMatrix44(reinterpret_cast<Member*>(base + 1232));
    if (Init__11VIClipStack(reinterpret_cast<Member*>(base + 1296)) < 0) return -1;
    if (Init__t7VIArray1Zii(reinterpret_cast<Member*>(base + 1104), 5000) < 0) return -1;
    Box zero_box;
    zero_box.min = {0.0f, 0.0f, 0.0f};
    zero_box.max = {0.0f, 0.0f, 0.0f};
    Vect3 up = {0.0f, 1.0f, 0.0f};
    if (Init__14VIQueryTriListiRC6VIBBoxRC7VIVect3f(reinterpret_cast<Member*>(base + 6128), 200,
                                                    &zero_box, &up, 80.0f) < 0)
        return -1;
    *reinterpret_cast<int*>(base + 6792) = 0;
    int* slots = reinterpret_cast<int*>(base + 1464);
    for (int i = 31; i >= 0; --i) slots[i] = -1;
    *reinterpret_cast<int*>(base + 1592) = 0;
    *reinterpret_cast<int*>(base + 1596) = -1;
    long long time = *reinterpret_cast<long long*>(base + 1152);
    void* raster_now = reinterpret_cast<void*>(static_cast<unsigned long>(*reinterpret_cast<unsigned*>(base + 19356)));
    Init__16VIParticleSystemlP8VIRasteri(reinterpret_cast<Member*>(base + 1352), time, raster_now, 2000);
    if (raster_now != 0)
        Init__12VIAtmospherelR8VIRasteri(reinterpret_cast<Member*>(base + 1600), time, raster_now, 600);
    Init__19VISpellEffectSystemP7VIScene(reinterpret_cast<Member*>(base + 7024), scene);
    if (raster_now != 0) {
        void* dictionary_now =
            reinterpret_cast<void*>(static_cast<unsigned long>(*reinterpret_cast<unsigned*>(base + 19352)));
        VITrailFxInit__FR8VIRasterR12VIDictionary(raster_now, dictionary_now);
    }
    if (InitTimeOfDay__7VIScene(scene) < 0) return -1;
    SetPersonalLightMode__7VIScene19VIPersonalLightMode(scene, 0);
    Vect3 light_origin = {0.0f, 0.0f, 0.0f};
    Color white = {1.0f, 1.0f, 1.0f, 1.0f};
    SetPersonalLight__7VISceneRC7VIVect3fRC10VIColor32F(scene, &light_origin, 20.0f, &white);
    Init__17VISceneSoundTrack(reinterpret_cast<Member*>(base + 6800));
    float* limits = reinterpret_cast<float*>(base);
    limits[10844 / 4] = 950.0f;
    limits[10840 / 4] = 1050.0f;
    limits[10848 / 4] = 900.0f;
    limits[10852 / 4] = 550.0f;
    limits[10856 / 4] = 500.0f;
    limits[10824 / 4] = 950.0f;
    limits[10828 / 4] = 900.0f;
    limits[10832 / 4] = 550.0f;
    limits[10836 / 4] = 500.0f;
    *reinterpret_cast<unsigned long long*>(base + 10864) = 0;
    *reinterpret_cast<unsigned*>(base + 10872) = 0;
    int flora = Init__19VIRadialFloraSystemR7VIScene(reinterpret_cast<Member*>(base + 10876), scene);
    if (flora < 0) return -1;
    *reinterpret_cast<int*>(base + 19348) = 1;
    GlobalInit__14VIEffectVolumeP7VIScene(scene);
    return 0;
}
}
