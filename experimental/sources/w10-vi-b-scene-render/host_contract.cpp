#include "scene_render.cpp"

#include <cstring>
#include <string>

static unsigned char* scene_bytes;
static unsigned char* camera_bytes;
static std::string trace;
static int flora_result;
static int render_result;
static unsigned vars_seen[14];
static unsigned char blend_seen[32];
static bool vars_was_portals;

static std::string scene_off(const void* p) { return "s" + std::to_string(static_cast<const unsigned char*>(p) - scene_bytes); }
static std::string cam_off(const void* p) { return "c" + std::to_string(static_cast<const unsigned char*>(p) - camera_bytes); }
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); }
static std::string color(const Color* c) { return std::to_string(c->r) + "," + std::to_string(c->g) + "," + std::to_string(c->b) + "," + std::to_string(c->a); }

extern "C" {
void SetTime__8VIRasterl(void* r, long long t) { trace += "settime" + std::to_string(reinterpret_cast<unsigned long>(r)) + "," + std::to_string(t) + ";"; }
void ClearShadowQueue__7VIScene(VIScene* s) { trace += "csq" + scene_off(s) + ";"; }
void SetProjection__8VIRasterRC10VIMatrix44(void* r, const void* m) { trace += "proj" + std::to_string(reinterpret_cast<unsigned long>(r)) + "," + cam_off(m) + ";"; }
void CalcOutdoorsIntensity__7VISceneR8VICamera(VIScene* s, VICamera* c) { trace += "outd" + scene_off(s) + "," + cam_off(c) + ";"; }
void SetStaticLighting__8VIRasterRC10VIColor32F(void* r, const Color* c) { trace += "light" + std::to_string(reinterpret_cast<unsigned long>(r)) + "," + color(c) + ";"; }
void Update__12VIAtmosphereR7VISceneR8VICameralff(Member* m, VIScene* s, VICamera* c, long long t, float a, float b) {
    trace += "atmo" + scene_off(m) + "," + scene_off(s) + "," + cam_off(c) + "," + std::to_string(t) + "," + std::to_string(a) + "," + std::to_string(b) + ";";
}
int Update__19VIRadialFloraSystemRC7VIVect3(Member* m, const void* p) { trace += "flora" + scene_off(m) + "," + cam_off(p) + ";"; return flora_result; }
unsigned GetEnvFlags__7VISceneRC7VIVect3(VIScene* s, const void* p) { trace += "env" + scene_off(s) + "," + cam_off(p) + ";"; return 0x1234; }
void CalcProfileBlend__7VIWorldRC7VIVect3R21VITerrainProfileBlend(Member* m, const void* p, void* blend) {
    trace += "blend" + scene_off(m) + "," + cam_off(p) + ";";
    for (int i = 0; i < 32; ++i) static_cast<unsigned char*>(blend)[i] = static_cast<unsigned char>(i + 1);
    std::memcpy(blend_seen, blend, 32);
}
static bool same_blend(const void* b) { return std::memcmp(b, blend_seen, 32) == 0; }
void SetupTimeOfDay__7VISceneR8VICameraR21VITerrainProfileBlendUi(VIScene* s, VICamera* c, void* b, unsigned f) {
    trace += "tod" + scene_off(s) + "," + cam_off(c) + "," + (same_blend(b) ? "b" : "?") + "," + std::to_string(f) + ";";
}
void RenderSky__12VIAtmosphereR7VISceneR8VICamera(Member* m, VIScene* s, VICamera* c) { trace += "sky" + scene_off(m) + "," + scene_off(s) + "," + cam_off(c) + ";"; }
void SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi(VIScene* s, void* b, unsigned f) {
    trace += "fog" + scene_off(s) + "," + (same_blend(b) ? "b" : "?") + "," + std::to_string(f) + ";";
}
void SetModelView__8VIRasterRC10VIMatrix44(void* r, const void* m) { trace += "mv" + std::to_string(reinterpret_cast<unsigned long>(r)) + "," + cam_off(m) + ";"; }
void Reset__11VIClipStack(Member* m) { trace += "reset" + scene_off(m) + ";"; }
int Init__t8VIVector1Z18VISceneDisplayElemi(Member* m, int n) { trace += "vec" + scene_off(m) + "," + std::to_string(n) + ";"; return 0; }
int RenderByFrustum__7VIWorldR15VISceneRendVars(Member* m, void* v) {
    trace += "frustum" + scene_off(m) + ";";
    std::memcpy(vars_seen, v, sizeof vars_seen);
    vars_was_portals = false;
    return render_result;
}
int RenderByPortals__7VIWorldR15VISceneRendVars(Member* m, void* v) {
    trace += "portals" + scene_off(m) + ";";
    std::memcpy(vars_seen, v, sizeof vars_seen);
    vars_was_portals = true;
    return render_result;
}
void RenderDeferredList__7VISceneR8VICameraii(VIScene* s, VICamera* c, int a, int b) {
    trace += "defer" + scene_off(s) + "," + cam_off(c) + "," + std::to_string(a) + std::to_string(b) + ";";
}
void Render__19VIRadialFloraSystemR8VICamera(Member* m, VICamera* c) { trace += "rflora" + scene_off(m) + "," + cam_off(c) + ";"; }
void UpdateSoundEmitters__7VISceneR8VICamera(VIScene* s, VICamera* c) { trace += "emit" + scene_off(s) + "," + cam_off(c) + ";"; }
void RasterDeferred__12VINameSpriteR8VIRaster(void* r) { trace += "rdefer" + std::to_string(reinterpret_cast<unsigned long>(r)) + ";"; }
void Render__16VIParticleSystemlR8VIRaster(Member* m, long long t, void* r) {
    trace += "rpart" + scene_off(m) + "," + std::to_string(t) + "," + std::to_string(reinterpret_cast<unsigned long>(r)) + ";";
}
void RenderShadows__7VIScene(VIScene* s) { trace += "shadows" + scene_off(s) + ";"; }
void Process__19VISpellEffectSystem(Member* m) { trace += "spell" + scene_off(m) + ";"; }
void EnableAllLights__8VIRasteri(void* r, int e) { trace += "lights" + std::to_string(reinterpret_cast<unsigned long>(r)) + "," + std::to_string(e) + ";"; }
void VITrailFxRender__FlR8VIRasterR8VICamera(long long t, void* r, VICamera* c) {
    trace += "trail" + std::to_string(t) + "," + std::to_string(reinterpret_cast<unsigned long>(r)) + "," + cam_off(c) + ";";
}
void RenderWeather__12VIAtmosphereR7VISceneR8VICamera(Member* m, VIScene* s, VICamera* c) { trace += "weather" + scene_off(m) + "," + scene_off(s) + "," + cam_off(c) + ";"; }
void _$_10VIMatrix44(Member* m, int f) {
    const float* w = reinterpret_cast<const float*>(m);
    bool zero = true;
    for (int i = 0; i < 16; ++i) zero = zero && w[i] == 0.0f;
    trace += std::string("mdtor") + (zero ? "z" : "?") + std::to_string(f) + ";";
}
}

static const int kScene = 19500;
alignas(8) static unsigned char scene[kScene];
alignas(8) static unsigned char camera[256];
static void put32(int off, unsigned v) { std::memcpy(scene + off, &v, 4); }
static unsigned get32(int off) { unsigned v; std::memcpy(&v, scene + off, 4); return v; }

static void build(unsigned raster, unsigned flora_flag, unsigned word0, unsigned emitters, float cam_limit) {
    std::memset(scene, 0x5a, sizeof scene);
    std::memset(camera, 0x6b, sizeof camera);
    scene_bytes = scene;
    camera_bytes = camera;
    put32(19356, raster);
    put32(19348, flora_flag);
    put32(0, word0);
    put32(19360, emitters);
    put32(1348, 10);
    put32(1592, 20);
    long long t = 777;
    std::memcpy(scene + 1152, &t, 8);
    float a = 12.5f, b = 0.25f;
    std::memcpy(scene + 8160, &a, 4);
    std::memcpy(scene + 6848, &b, 4);
    std::memcpy(camera + 4, &cam_limit, 4);
    trace.clear();
}

int main() {
    VIScene* s = reinterpret_cast<VIScene*>(scene);
    VICamera* c = reinterpret_cast<VICamera*>(camera);
    flora_result = 0;
    render_result = 0;

    // No raster: -1, only the matrix destructor runs.
    build(0, 1, 0, 1, 10.0f);
    if (Render__7VISceneR8VICamera(s, c) != -1) return 1;
    if (trace != "mdtorz2;") return 2;

    // Flora update fails.
    build(7424, 1, 0, 1, 10.0f);
    flora_result = -1;
    if (Render__7VISceneR8VICamera(s, c) != -1) return 3;
    if (trace.substr(trace.size() - 8) != "mdtorz2;" || trace.find("blend") != std::string::npos) return 4;
    flora_result = 0;

    // Full path, flora on, word0 even -> portals, emitters on.
    build(7424, 1, 0, 1, 10.0f);
    if (Render__7VISceneR8VICamera(s, c) != 0) return 5;
    std::string want = "settime7424,777;csqs0;proj7424,c112;outds0,c0;light7424,0.000000,0.000000,0.000000,1.000000;"
                       "atmos1600,s0,c0,777,12.500000,0.250000;floras10876,c28;envs0,c28;blends4,c28;tods0,c0,b,4660;"
                       "skys1600,s0,c0;fogs0,b,4660;mv7424,c48;resets1296;vecs1112,1000;portalss4;"
                       "defers0,c0,01;rfloras10876,c0;defers0,c0,10;emits0,c0;rdefer7424;mv7424,c48;"
                       "rparts1352,777,7424;mv7424,c48;shadowss0;spells7024;lights7424,0;trail777,7424,c0;"
                       "weathers1600,s0,c0;light7424,0.000000,0.000000,0.000000,1.000000;mdtorz2;";
    if (trace != want) return 6;
    if (!vars_was_portals) return 7;
    if (vars_seen[1] != low(scene) || vars_seen[2] != 0 || vars_seen[3] != low(camera) || vars_seen[4] != 7424 ||
        vars_seen[5] != low(scene + 1296) || vars_seen[11] != 1 || vars_seen[12] != 0xffffffffu)
        return 8;
    for (int i = 6; i <= 10; ++i)
        if (vars_seen[i] != 0) return 9;
    float lim;
    std::memcpy(&lim, &vars_seen[13], 4);
    if (lim != 10.0f) return 10;
    if (get32(1348) != 11 || get32(1592) != 21) return 11;

    // Odd word0 -> frustum; limit above 500 clamps; flora off; emitters off.
    build(7424, 0, 1, 0, 900.0f);
    if (Render__7VISceneR8VICamera(s, c) != 0) return 12;
    if (vars_was_portals || vars_seen[11] != 0) return 13;
    std::memcpy(&lim, &vars_seen[13], 4);
    if (lim != 500.0f) return 14;
    if (trace.find("flora") != std::string::npos || trace.find("emit") != std::string::npos) return 15;
    if (trace.find("frustums4;") == std::string::npos) return 16;
    if (get32(1348) != 11 || get32(1592) != 20) return 17;
    // word0 == 3: bit 0 of the xor is clear -> frustum.
    build(7424, 0, 3, 0, 500.0f);
    Render__7VISceneR8VICamera(s, c);
    std::memcpy(&lim, &vars_seen[13], 4);
    if (vars_was_portals || lim != 500.0f) return 18;
    // word0 == 2: xor 1 = 3, bit 0 set -> portals; limit exactly 500 stays.
    build(7424, 0, 2, 0, -3.0f);
    Render__7VISceneR8VICamera(s, c);
    std::memcpy(&lim, &vars_seen[13], 4);
    if (!vars_was_portals || lim != -3.0f) return 19;

    // World render fails: -1 right after, nothing deferred.
    build(7424, 0, 0, 1, 10.0f);
    render_result = -2;
    if (Render__7VISceneR8VICamera(s, c) != -1) return 20;
    if (trace.find("defer") != std::string::npos || trace.substr(trace.size() - 8) != "mdtorz2;") return 21;
    return 0;
}
