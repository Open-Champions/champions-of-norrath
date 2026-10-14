#define NORRATH_HOST_CONTRACT 1
#include "update_sound_emitters.cpp"

#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[1 << 17];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static unsigned off_of(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf_bits(unsigned bits) { float f; std::memcpy(&f, &bits, 4); return f; }

static const unsigned scene = 0x1000, pool = 0x3000, sprites = 0x3400, emitters = 0x4000, entries = 0x5000, camera = 0x6000, path_text = 0x7000;

struct Ev { std::string what; long a, b; float f; };
static std::vector<Ev> ev;
static int phase;                  // number of queries issued so far
static int counts[3];              // entries returned by the first and second query
static int stream_enabled;
static int query_result[2];
static int priorities[8];
static int loop_flag, mode, pan_enabled, voice_result;
static float distance_value;
static std::string bgm_path;

extern "C" void __t10VIMultiMap2ZfZ14VIEmitterEntry(void*) { ev.push_back({"mctor", 0, 0, 0}); }
extern "C" void _$_t10VIMultiMap2ZfZ14VIEmitterEntry(void*, int f) { ev.push_back({"mdtor", f, 0, 0}); }
extern "C" int IsIopStreamingEnabled__6VIFile() { return stream_enabled; }
extern "C" void Init__14VIEmitterQueryi12VISpriteType(void*, int cap, int type) { ev.push_back({"qinit", cap, type, 0}); }
extern "C" int QueryEmittersAtLocation__7VISceneRC8VICameraR14VIEmitterQuery(VIScene*, const VICamera*, void* q) {
    ++phase;
    ev.push_back({"query", phase, 0, 0});
    put(off_of(q) + 24, entries);
    return query_result[phase - 1];
}
extern "C" int Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(const void*) { return 0; }
extern "C" int End__Ct10VIMultiMap2ZfZ14VIEmitterEntry(const void*) { return counts[phase]; }
extern "C" int Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi(const void*, int i) { return i + 1; }
extern "C" int Begin__Ct6VIPool1ZP8VISprite(const void*) { return 0; }
extern "C" int End__Ct6VIPool1ZP8VISprite(const void*) { return 6; }
extern "C" int Next__Ct6VIPool1ZP8VISpritei(const void*, int i) { return i + 1; }
extern "C" int GetPriority__C19VIStreamAudioSprite(const void* s) { return priorities[(off_of(s) - sprites) / 0x100]; }
extern "C" int IsLooping__C19VIStreamAudioSprite(const void*) { return loop_flag; }
extern "C" const char* GetRelativePathname__C19VIStreamAudioSprite(const void*) { return reinterpret_cast<const char*>(arena) + path_text; }
extern "C" void PlayBgm__13VISoundDeviceiPCc(void*, int a, const char* path) { ev.push_back({"playbgm", a, 0, 0}); bgm_path = path; }
extern "C" void StopBgm__13VISoundDevicei(void*, int a) { ev.push_back({"stopbgm", a, 0, 0}); }
extern "C" int Play__13VISoundDeviceii12VISOUNDCLASS(void*, int a, int b, int c) { ev.push_back({"play", a, b * 10 + c, 0}); return voice_result; }
extern "C" void SetVolume__13VISoundDeviceif(void*, int voice, float v) { ev.push_back({"volume", voice, 0, v}); }
extern "C" void SetPan__13VISoundDeviceif(void*, int voice, float p) { ev.push_back({"pan", voice, 0, p}); }
extern "C" void Stop__13VISoundDevicei(void*, int voice) { ev.push_back({"stop", voice, 0, 0}); }
extern "C" void Normalize__7VIVect3(float*) { ev.push_back({"normalize", 0, 0, 0}); }
extern "C" float Distance__C7VIVect3RC7VIVect3(const float*, const float*) { return distance_value; }
extern "C" int AttenuationMode__C13VISoundSprite(const void*) { return mode; }
extern "C" int IsPanEnabled__C13VISoundSprite(const void*) { return pan_enabled; }
extern "C" char* strcat(char* d, const char* s) { std::strcpy(d + std::strlen(d), s); return d; }

static void reset() {
    std::memset(arena, 0, sizeof arena);
    ev.clear(); phase = 0; bgm_path.clear();
    put(scene + 996 + 8, pool); put(scene + 19360, 0xd0d0); put(scene + 1592, 50); put(scene + 1596, 0xffffffffu);
    for (int k = 0; k < 8; ++k) {
        put(pool + 12 * k, sprites + 0x100 * k);
        put(sprites + 0x100 * k + 72, 0); put(sprites + 0x100 * k + 68, 100 + k); put(sprites + 0x100 * k + 64, 200 + k);
        putf(sprites + 0x100 * k + 44, 5.0f); putf(sprites + 0x100 * k + 84, 2.0f);
        priorities[k] = 0;
    }
    putf(emitters + 68, 3.0f); putf(emitters + 76, 4.0f); putf(emitters + 92, 2.0f);
    putf(camera + 8, 0.5f); putf(camera + 16, 8.0f); putf(camera + 28, 7.0f); putf(camera + 36, 10.0f);
    std::strcpy(reinterpret_cast<char*>(arena) + path_text, "Song.str");
    for (int i = 0; i < 4; ++i) { put(entries + 112 * i + 32, emitters); put(entries + 112 * i + 36, 0); }
    stream_enabled = 0; query_result[0] = query_result[1] = 0; counts[0] = counts[1] = counts[2] = 0;
    loop_flag = 0; mode = 0; pan_enabled = 1; voice_result = 5; distance_value = 5.0f;
    for (int i = 0; i < 32; ++i) put(scene + 1464 + 4 * i, 0xffffffffu);
}
static int count_of(const char* what) { int n = 0; for (auto& e : ev) if (e.what == what) ++n; return n; }
static Ev* nth(const char* what, int n) { for (auto& e : ev) if (e.what == what && n-- == 0) return &e; return nullptr; }

int main() {
    VIScene* s = reinterpret_cast<VIScene*>(arena + scene);
    VICamera* c = reinterpret_cast<VICamera*>(arena + camera);
    // no streaming, nothing queried: only the second query pair and the destructor
    reset();
    if (UpdateSoundEmitters__7VISceneR8VICamera(s, c) != 0) return 1;
    if (count_of("qinit") != 1 || nth("qinit", 0)->b != 9 || count_of("playbgm") || count_of("stopbgm") || ev.back().what != "mdtor" || ev.back().a != 2) return 2;
    // streaming: pick the highest-priority stream sprite (a later tie wins); not current -> play "Music\" + path
    reset(); stream_enabled = 1; counts[1] = 3; priorities[1] = 3; priorities[2] = 7; priorities[3] = 7;
    put(entries + 0 + 36, 1); put(entries + 112 + 36, 2); put(entries + 224 + 36, 3);
    UpdateSoundEmitters__7VISceneR8VICamera(s, c);
    if (nth("qinit", 0)->b != 12 || nth("qinit", 0)->a != 32) return 3;
    if (get(scene + 1596) != 3 || bgm_path != "Music\\Song.str" || count_of("playbgm") != 1) return 4;
    // same sprite already current and not looping: no play; looping: play again
    reset(); stream_enabled = 1; counts[1] = 1; priorities[2] = 4; put(entries + 36, 2); put(scene + 1596, 2);
    UpdateSoundEmitters__7VISceneR8VICamera(s, c);
    if (count_of("playbgm") != 0 || count_of("stopbgm") != 0) return 5;
    reset(); stream_enabled = 1; counts[1] = 1; priorities[2] = 4; put(entries + 36, 2); put(scene + 1596, 2); loop_flag = 1;
    UpdateSoundEmitters__7VISceneR8VICamera(s, c);
    if (count_of("playbgm") != 1) return 6;
    // streaming with every priority negative: stop
    reset(); stream_enabled = 1; counts[1] = 1; priorities[1] = -5; put(entries + 36, 1);
    UpdateSoundEmitters__7VISceneR8VICamera(s, c);
    if (count_of("stopbgm") != 1 || get(scene + 1596) != 0xffffffffu) return 7;
    // failed first query: nothing played or stopped
    reset(); stream_enabled = 1; query_result[0] = -1; counts[1] = 1;
    UpdateSoundEmitters__7VISceneR8VICamera(s, c);
    if (count_of("playbgm") + count_of("stopbgm") != 0) return 8;
    // emitters: sprite 3 not yet playing (starts, voice 5); sprite 4 already marked frame - 1
    reset(); counts[1] = 2; put(entries + 36, 3); put(entries + 112 + 36, 4);
    put(sprites + 4 * 0x100 + 72, 49); put(sprites + 3 * 0x100 + 72, 7);
    put(scene + 1464, 3); put(scene + 1468, 5); put(scene + 1472, 9); put(scene + 1476, 4);
    put(sprites + 5 * 0x100 + 72, 9);
    mode = 0; pan_enabled = 1; distance_value = 5.0f;
    UpdateSoundEmitters__7VISceneR8VICamera(s, c);
    if (count_of("play") != 1 || nth("play", 0)->a != 203 || nth("play", 0)->b != 0) return 9;
    if (get(sprites + 3 * 0x100 + 68) != 5) return 10;
    // ratio = 5 / (2 * 5) = 0.5; volume = 0.5 * 2 = 1.0; pan = toward . side, toward = (4, 0, 6), side = (8, 0, -0.5) (no normalise)
    if (count_of("volume") != 2 || nth("volume", 0)->a != 5 || nth("volume", 0)->f != 1.0f) return 11;
    if (nth("volume", 1)->a != 104) return 12;
    if (nth("pan", 0)->f != 4.0f * 8.0f + 6.0f * -0.5f) return 13;
    if (count_of("normalize") != 4) return 14;
    if (get(sprites + 3 * 0x100 + 72) != 50 || get(sprites + 4 * 0x100 + 72) != 50) return 15;
    // slots: 3 and 4 were refreshed (kept playing), 5 is found but stale (stopped), 9 is not in the pool
    if (count_of("stop") != 1 || nth("stop", 0)->a != 105) return 16;
    if (get(scene + 1464) != 3 || get(scene + 1468) != 4 || get(scene + 1472) != 0xffffffffu || get(scene + 1476) != 0xffffffffu) return 17;
    if (get(scene + 1464 + 4 * 31) != 0xffffffffu) return 18;
    // attenuation modes
    mode = 1; reset(); counts[1] = 1; put(entries + 36, 3); put(sprites + 3 * 0x100 + 72, 49); mode = 1;
    UpdateSoundEmitters__7VISceneR8VICamera(s, c);
    if (nth("volume", 0)->f != (1.0f - 0.0625f) * 2.0f) return 19;
    reset(); counts[1] = 1; put(entries + 36, 3); put(sprites + 3 * 0x100 + 72, 49); mode = 2;
    UpdateSoundEmitters__7VISceneR8VICamera(s, c);
    if (nth("volume", 0)->f != 0.0625f * 2.0f) return 20;
    // pan disabled and a non-positive voice
    reset(); counts[1] = 1; put(entries + 36, 3); put(sprites + 3 * 0x100 + 72, 49); pan_enabled = 0;
    UpdateSoundEmitters__7VISceneR8VICamera(s, c);
    if (nth("pan", 0)->f != 0.0f) return 21;
    reset(); counts[1] = 1; put(entries + 36, 3); voice_result = 0;
    UpdateSoundEmitters__7VISceneR8VICamera(s, c);
    if (count_of("volume") != 0 || get(sprites + 3 * 0x100 + 72) != 0) return 22;
    // a failed second query leaves the slots alone (they are still cleared)
    reset(); query_result[0] = -1; counts[1] = 1; put(scene + 1464, 2); put(scene + 1468, 0xffffffffu);
    UpdateSoundEmitters__7VISceneR8VICamera(s, c);
    if (get(scene + 1464) != 0xffffffffu || count_of("play") != 0) return 23;
    (void)getf_bits;
    return 0;
}
