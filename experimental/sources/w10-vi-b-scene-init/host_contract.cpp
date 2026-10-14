#include "scene_init.cpp"

#include <cstring>
#include <string>

static unsigned char* scene_bytes;
static std::string trace;
static int fail_at;     // 1-based index among the checked steps, 0 for none
static int checked_seen;

static std::string at(const void* p) {
    return std::to_string(static_cast<const unsigned char*>(p) - scene_bytes);
}
static std::string f(float v) { return std::to_string(v); }
static std::string vec(const Vect3* v) { return f(v->x) + "," + f(v->y) + "," + f(v->z); }
static std::string box(const Box* b) { return vec(&b->min) + "/" + vec(&b->max); }
static int step(const std::string& what, int ok_value = 0) {
    trace += what + ";";
    return ++checked_seen == fail_at ? -1 : ok_value;
}

extern "C" {
int Init__t6VIList1Z7VIActor(Member* m) { return step("actors@" + at(m)); }
int Init__t6VIList1ZQ27VIScene12VISceneOccup(Member* m) { return step("occup@" + at(m)); }
int Init__t6VIPool1ZP8VISpritei(Member* m, int n) { return step("sprites@" + at(m) + "," + std::to_string(n)); }
int Init__t6VIPool1ZP13VIHSpriteAnimi(Member* m, int n) { return step("anims@" + at(m) + "," + std::to_string(n)); }
int Init__t6VIPool1ZP8VIRefMapi(Member* m, int n) { return step("maps@" + at(m) + "," + std::to_string(n)); }
int Init__t6VIPool1ZP16VIStaticLightingi(Member* m, int n) { return step("lights@" + at(m) + "," + std::to_string(n)); }
int Init__7VIWorldiiiifRC7VIVect3RC6VIBBox(Member* m, int a, int b, int c, int d, float size,
                                           const Vect3* origin, const Box* bx) {
    return step("world@" + at(m) + "," + std::to_string(a) + std::to_string(b) + std::to_string(c) +
                std::to_string(d) + "," + f(size) + "," + vec(origin) + "," + box(bx));
}
int CreateSprite__7VIScene12VISpriteTypei(VIScene* s, int type, int flag) {
    // The stored index is the returned value; -1 fails, 5 is kept.
    return step("create@" + at(s) + "," + std::to_string(type) + "," + std::to_string(flag), 5);
}
void Identity__10VIMatrix44(Member* m) { trace += "ident@" + at(m) + ";"; }
int Init__11VIClipStack(Member* m) { return step("clip@" + at(m)); }
int Init__t7VIArray1Zii(Member* m, int n) { return step("array@" + at(m) + "," + std::to_string(n)); }
int Init__14VIQueryTriListiRC6VIBBoxRC7VIVect3f(Member* m, int n, const Box* bx, const Vect3* v, float d) {
    return step("query@" + at(m) + "," + std::to_string(n) + "," + box(bx) + "," + vec(v) + "," + f(d));
}
int Init__16VIParticleSystemlP8VIRasteri(Member* m, long long v, void* raster, int n) {
    trace += "particles@" + at(m) + "," + std::to_string(v) + "," + std::to_string(reinterpret_cast<unsigned long>(raster)) +
             "," + std::to_string(n) + ";";
    return -7;
}
int Init__12VIAtmospherelR8VIRasteri(Member* m, long long v, void* raster, int n) {
    trace += "atmo@" + at(m) + "," + std::to_string(v) + "," + std::to_string(reinterpret_cast<unsigned long>(raster)) +
             "," + std::to_string(n) + ";";
    return -7;
}
int Init__19VISpellEffectSystemP7VIScene(Member* m, VIScene* s) {
    trace += "spell@" + at(m) + "," + at(s) + ";";
    return -7;
}
void VITrailFxInit__FR8VIRasterR12VIDictionary(void* raster, void* dictionary) {
    trace += "trail" + std::to_string(reinterpret_cast<unsigned long>(raster)) + "," +
             std::to_string(reinterpret_cast<unsigned long>(dictionary)) + ";";
}
int InitTimeOfDay__7VIScene(VIScene* s) { return step("tod@" + at(s)); }
void SetPersonalLightMode__7VIScene19VIPersonalLightMode(VIScene* s, int mode) {
    trace += "plmode@" + at(s) + "," + std::to_string(mode) + ";";
}
void SetPersonalLight__7VISceneRC7VIVect3fRC10VIColor32F(VIScene* s, const Vect3* v, float d, const Color* c) {
    trace += "plight@" + at(s) + "," + vec(v) + "," + f(d) + "," + f(c->r) + f(c->g) + f(c->b) + f(c->a) + ";";
}
void Init__17VISceneSoundTrack(Member* m) { trace += "track@" + at(m) + ";"; }
int Init__19VIRadialFloraSystemR7VIScene(Member* m, VIScene* s) {
    return step("flora@" + at(m) + "," + at(s));
}
void GlobalInit__14VIEffectVolumeP7VIScene(VIScene* s) { trace += "ginit@" + at(s) + ";"; }
}

static const int kSize = 19400;
static unsigned char scene[kSize];
static unsigned get32(int off) { unsigned v; std::memcpy(&v, scene + off, 4); return v; }

static std::string full_trace(bool raster) {
    std::string t =
        "actors@972;occup@984;sprites@996,0;anims@1020,0;maps@1044,0;lights@1068,0;"
        "world@4,1114,2000.000000,0.000000,0.000000,0.000000,-1000.000000,-300.000000,-1000.000000/"
        "1000.000000,300.000000,1000.000000;create@0,2,1;ident@1168;ident@1232;clip@1296;array@1104,5000;"
        "query@6128,200,0.000000,0.000000,0.000000/0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,80.000000;"
        "particles@1352," + std::to_string(0x1234567890abcdefll) + ",";
    t += raster ? "7424" : "0";
    t += ",2000;";
    if (raster) t += "atmo@1600," + std::to_string(0x1234567890abcdefll) + ",7424,600;";
    t += "spell@7024,0;";
    if (raster) t += "trail7424,2184;";
    t += "tod@0;plmode@0,0;plight@0,0.000000,0.000000,0.000000,20.000000,1.0000001.0000001.0000001.000000;track@6800;"
         "flora@10876,0;ginit@0;";
    return t;
}

int main() {
    scene_bytes = scene;
    VIScene* s = reinterpret_cast<VIScene*>(scene);
    void* dictionary = reinterpret_cast<void*>(2184);
    void* sound = reinterpret_cast<void*>(0x1111);
    void* collide = reinterpret_cast<void*>(0x2222);

    for (int with_raster = 0; with_raster < 2; ++with_raster) {
        std::memset(scene, 0xa5, sizeof scene);
        trace.clear();
        checked_seen = 0;
        fail_at = 0;
        void* raster = with_raster ? reinterpret_cast<void*>(7424) : nullptr;
        if (Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(
                s, 0x1234567890abcdefll, dictionary, raster, sound, collide) != 0)
            return 1;
        if (trace != full_trace(with_raster)) return 2;
        long long time;
        std::memcpy(&time, scene + 1152, 8);
        if (time != 0x1234567890abcdefll) return 3;
        if (get32(19352) != 2184 || get32(19356) != (with_raster ? 7424u : 0u) || get32(19360) != 0x1111 ||
            get32(19364) != 0x2222)
            return 4;
        if (get32(1124) != 5 || get32(19348) != 1 || get32(6792) != 0 || get32(1592) != 0) return 5;
        if (get32(1596) != 0xffffffffu) return 6;
        for (int off = 1464; off < 1592; off += 4)
            if (get32(off) != 0xffffffffu) return 7;
        if (get32(1460) != 0xa5a5a5a5u) return 8;
        float limits[9];
        const int offs[9] = {10844, 10840, 10848, 10852, 10856, 10824, 10828, 10832, 10836};
        const float want[9] = {950.0f, 1050.0f, 900.0f, 550.0f, 500.0f, 950.0f, 900.0f, 550.0f, 500.0f};
        for (int i = 0; i < 9; ++i) {
            std::memcpy(&limits[i], scene + offs[i], 4);
            if (limits[i] != want[i]) return 9;
        }
        if (get32(10872) != 0 || get32(10864) != 0 || get32(10868) != 0) return 10;
    }

    // Each checked step failing: result -1, trace ends at that step, stores
    // after it are absent.
    const char* order[] = {"actors@", "occup@", "sprites@", "anims@", "maps@", "lights@", "world@",
                           "create@", "clip@", "array@", "query@", "tod@", "flora@"};
    std::string full = full_trace(true);
    for (int k = 1; k <= 13; ++k) {
        std::memset(scene, 0xa5, sizeof scene);
        trace.clear();
        checked_seen = 0;
        fail_at = k;
        int r = Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(
            s, 0x1234567890abcdefll, dictionary, reinterpret_cast<void*>(7424), sound, collide);
        fail_at = 0;
        if (r != -1) return 100 + k;
        size_t pos = full.find(order[k - 1]);
        size_t end = full.find(';', pos);
        if (trace != full.substr(0, end + 1)) return 200 + k;
        if (k == 8 && get32(1124) != 0xffffffffu) return 300 + k;   // failing index is stored
        if (k < 8 && get32(1124) != 0xa5a5a5a5u) return 400 + k;
        if (k >= 1 && get32(19348) != 0xa5a5a5a5u) return 500 + k;
    }
    return 0;
}
