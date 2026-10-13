// Observed Render__7VISceneR8VICamera at 0x0113bb48, 912 bytes. Offsets are
// bytes from the scene (raster is the word at 19356) or from the camera.
// Result: -1 when the raster is zero or a checked step fails, otherwise zero.
// A local 4x4 matrix is zeroed on entry and destroyed (flag 2) on every exit.
//  1. Raster zero: result -1.
//  2. SetTime__8VIRaster(raster, long at 1152); ClearShadowQueue__7VIScene;
//     SetProjection__8VIRaster(raster, camera+112); CalcOutdoorsIntensity;
//     SetStaticLighting__8VIRaster(raster, (0, 0, 0, 1)).
//  3. Update__12VIAtmosphere(+1600, scene, camera, long at 1152, float at
//     8160, float at 6848).
//  4. When the word at 19348 is 1: Update__19VIRadialFloraSystem(+10876,
//     camera+28), a negative result giving -1.
//  5. flags = GetEnvFlags(scene, camera+28); CalcProfileBlend(world at +4,
//     camera+28, blend); SetupTimeOfDay(scene, camera, blend, flags);
//     RenderSky(+1600, scene, camera); SetupTimeOfDayFog(scene, blend, flags);
//     SetModelView(raster, camera+48); Reset__11VIClipStack(+1296);
//     Init VIVector<VISceneDisplayElem> (+1112, 1000).
//  6. A 56-byte render-variable block (bytes 64..119 of the local frame) is
//     filled: scene at +4, zero at +8, camera at +12, raster at +16, clip
//     stack (+1296) at +20, zeros at +24..+40, (scene word 0 xor 1) & 1 at
//     +44, -1 at +48, and min(camera float at 4, 500.0f) at +52; word +0 is
//     left as it was. RenderByPortals__7VIWorld(+4, block) when +44 is
//     nonzero, otherwise RenderByFrustum__7VIWorld; a negative result gives -1.
//  7. RenderDeferredList(scene, camera, 0, 1); when the word at 19348 is 1
//     Render__19VIRadialFloraSystem(+10876, camera); RenderDeferredList(scene,
//     camera, 1, 0); the word at 1348 is incremented; when the word at 19360
//     is nonzero UpdateSoundEmitters(scene, camera) is called and the word at
//     1592 is incremented.
//  8. RasterDeferred__12VINameSprite(raster); SetModelView(raster, camera+48);
//     Render__16VIParticleSystem(+1352, long at 1152, raster); SetModelView
//     again; RenderShadows(scene); Process__19VISpellEffectSystem(+7024);
//     EnableAllLights__8VIRaster(raster, 0); VITrailFxRender(long at 1152,
//     raster, camera); RenderWeather(+1600, scene, camera);
//     SetStaticLighting(raster, (0, 0, 0, 1)). Result zero.
// The layout of the render-variable block and the profile blend (32 bytes
// assumed from the frame offsets) is not established beyond the words named.
extern "C" {
struct VIScene;
struct VICamera;
struct Member;
struct Color {
    float r, g, b, a;
};

void SetTime__8VIRasterl(void* raster, long long time);
void ClearShadowQueue__7VIScene(VIScene* scene);
void SetProjection__8VIRasterRC10VIMatrix44(void* raster, const void* matrix);
void CalcOutdoorsIntensity__7VISceneR8VICamera(VIScene* scene, VICamera* camera);
void SetStaticLighting__8VIRasterRC10VIColor32F(void* raster, const Color* color);
void Update__12VIAtmosphereR7VISceneR8VICameralff(Member* atmosphere, VIScene* scene,
                                                  VICamera* camera, long long time, float a, float b);
int Update__19VIRadialFloraSystemRC7VIVect3(Member* flora, const void* position);
unsigned GetEnvFlags__7VISceneRC7VIVect3(VIScene* scene, const void* position);
void CalcProfileBlend__7VIWorldRC7VIVect3R21VITerrainProfileBlend(Member* world, const void* position,
                                                                  void* blend);
void SetupTimeOfDay__7VISceneR8VICameraR21VITerrainProfileBlendUi(VIScene* scene, VICamera* camera,
                                                                  void* blend, unsigned flags);
void RenderSky__12VIAtmosphereR7VISceneR8VICamera(Member* atmosphere, VIScene* scene, VICamera* camera);
void SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi(VIScene* scene, void* blend, unsigned flags);
void SetModelView__8VIRasterRC10VIMatrix44(void* raster, const void* matrix);
void Reset__11VIClipStack(Member* stack);
int Init__t8VIVector1Z18VISceneDisplayElemi(Member* vector, int count);
int RenderByFrustum__7VIWorldR15VISceneRendVars(Member* world, void* vars);
int RenderByPortals__7VIWorldR15VISceneRendVars(Member* world, void* vars);
void RenderDeferredList__7VISceneR8VICameraii(VIScene* scene, VICamera* camera, int a, int b);
void Render__19VIRadialFloraSystemR8VICamera(Member* flora, VICamera* camera);
void UpdateSoundEmitters__7VISceneR8VICamera(VIScene* scene, VICamera* camera);
void RasterDeferred__12VINameSpriteR8VIRaster(void* raster);
void Render__16VIParticleSystemlR8VIRaster(Member* particles, long long time, void* raster);
void RenderShadows__7VIScene(VIScene* scene);
void Process__19VISpellEffectSystem(Member* effects);
void EnableAllLights__8VIRasteri(void* raster, int enable);
void VITrailFxRender__FlR8VIRasterR8VICamera(long long time, void* raster, VICamera* camera);
void RenderWeather__12VIAtmosphereR7VISceneR8VICamera(Member* atmosphere, VIScene* scene,
                                                      VICamera* camera);
void _$_10VIMatrix44(Member* matrix, int flags);

static unsigned handle(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<unsigned long>(pointer));
}

int Render__7VISceneR8VICamera(VIScene* scene, VICamera* camera) {
    char* base = reinterpret_cast<char*>(scene);
    char* cam = reinterpret_cast<char*>(camera);
    float matrix[16];
    for (int i = 0; i < 16; ++i) matrix[i] = 0.0f;
    Member* local_matrix = reinterpret_cast<Member*>(matrix);
    void* raster = reinterpret_cast<void*>(static_cast<unsigned long>(*reinterpret_cast<unsigned*>(base + 19356)));
    if (raster == 0) {
        _$_10VIMatrix44(local_matrix, 2);
        return -1;
    }
    Member* atmosphere = reinterpret_cast<Member*>(base + 1600);
    long long time = *reinterpret_cast<long long*>(base + 1152);
    SetTime__8VIRasterl(raster, time);
    ClearShadowQueue__7VIScene(scene);
    SetProjection__8VIRasterRC10VIMatrix44(raster, cam + 112);
    CalcOutdoorsIntensity__7VISceneR8VICamera(scene, camera);
    Color black = {0.0f, 0.0f, 0.0f, 1.0f};
    SetStaticLighting__8VIRasterRC10VIColor32F(raster, &black);
    Update__12VIAtmosphereR7VISceneR8VICameralff(atmosphere, scene, camera, time,
                                                 *reinterpret_cast<float*>(base + 8160),
                                                 *reinterpret_cast<float*>(base + 6848));
    if (*reinterpret_cast<int*>(base + 19348) == 1) {
        if (Update__19VIRadialFloraSystemRC7VIVect3(reinterpret_cast<Member*>(base + 10876), cam + 28) < 0) {
            _$_10VIMatrix44(local_matrix, 2);
            return -1;
        }
    }
    unsigned flags = GetEnvFlags__7VISceneRC7VIVect3(scene, cam + 28);
    unsigned char blend[32];
    CalcProfileBlend__7VIWorldRC7VIVect3R21VITerrainProfileBlend(reinterpret_cast<Member*>(base + 4),
                                                                 cam + 28, blend);
    SetupTimeOfDay__7VISceneR8VICameraR21VITerrainProfileBlendUi(scene, camera, blend, flags);
    RenderSky__12VIAtmosphereR7VISceneR8VICamera(atmosphere, scene, camera);
    SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi(scene, blend, flags);
    SetModelView__8VIRasterRC10VIMatrix44(raster, cam + 48);
    Member* clip_stack = reinterpret_cast<Member*>(base + 1296);
    Reset__11VIClipStack(clip_stack);
    Init__t8VIVector1Z18VISceneDisplayElemi(reinterpret_cast<Member*>(base + 1112), 1000);

    unsigned vars[14];
    float limit = *reinterpret_cast<float*>(cam + 4);
    if (!(limit <= 500.0f)) limit = 500.0f;
    vars[1] = handle(scene);
    vars[2] = 0;
    vars[3] = handle(camera);
    vars[4] = handle(raster);
    vars[5] = handle(clip_stack);
    vars[6] = 0;
    vars[7] = 0;
    vars[8] = 0;
    vars[9] = 0;
    vars[10] = 0;
    vars[11] = (*reinterpret_cast<unsigned*>(base) ^ 1u) & 1u;
    vars[12] = static_cast<unsigned>(-1);
    *reinterpret_cast<float*>(&vars[13]) = limit;
    int rendered;
    Member* world = reinterpret_cast<Member*>(base + 4);
    if (vars[11] != 0)
        rendered = RenderByPortals__7VIWorldR15VISceneRendVars(world, vars);
    else
        rendered = RenderByFrustum__7VIWorldR15VISceneRendVars(world, vars);
    if (rendered < 0) {
        _$_10VIMatrix44(local_matrix, 2);
        return -1;
    }
    RenderDeferredList__7VISceneR8VICameraii(scene, camera, 0, 1);
    if (*reinterpret_cast<int*>(base + 19348) == 1)
        Render__19VIRadialFloraSystemR8VICamera(reinterpret_cast<Member*>(base + 10876), camera);
    RenderDeferredList__7VISceneR8VICameraii(scene, camera, 1, 0);
    *reinterpret_cast<int*>(base + 1348) += 1;
    if (*reinterpret_cast<unsigned*>(base + 19360) != 0) {
        UpdateSoundEmitters__7VISceneR8VICamera(scene, camera);
        *reinterpret_cast<int*>(base + 1592) += 1;
    }
    RasterDeferred__12VINameSpriteR8VIRaster(raster);
    SetModelView__8VIRasterRC10VIMatrix44(raster, cam + 48);
    Render__16VIParticleSystemlR8VIRaster(reinterpret_cast<Member*>(base + 1352), time, raster);
    SetModelView__8VIRasterRC10VIMatrix44(raster, cam + 48);
    RenderShadows__7VIScene(scene);
    Process__19VISpellEffectSystem(reinterpret_cast<Member*>(base + 7024));
    EnableAllLights__8VIRasteri(raster, 0);
    VITrailFxRender__FlR8VIRasterR8VICamera(time, raster, camera);
    RenderWeather__12VIAtmosphereR7VISceneR8VICamera(atmosphere, scene, camera);
    Color black_again = {0.0f, 0.0f, 0.0f, 1.0f};
    SetStaticLighting__8VIRasterRC10VIColor32F(raster, &black_again);
    _$_10VIMatrix44(local_matrix, 2);
    return 0;
}
}
