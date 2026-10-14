// Provenance: this unit carries the same function body as w11-vi-render-c-scene-update-sound-emitters
// (the source text below is unchanged). It is published again here because that unit was not
// selected: its host contract wrote through a stack address truncated to an arena offset and crashed
// on hosts whose stack lies more than 4 GiB from the arena. The host check in this directory writes
// through the pointer instead, and a fresh paired run against the original (122 scenarios) matched.
// Observed UpdateSoundEmitters__7VISceneR8VICamera at 0x0113d4a8, 1500 bytes; arguments scene and
// camera, result zero. Offsets are bytes. Scene words used: the sprite pool at +996 (data pointer at
// pool + 8, 12-byte entries whose first word is a sprite pointer), a frame counter at +1592, the current
// background-music sprite index at +1596 (-1 for none), thirty-two index slots at +1464, and the sound
// device pointer at +19360. A local query object (16 bytes) is followed at +16 by a multimap of emitter
// entries (VIMultiMap<float, VIEmitterEntry>), constructed first and destroyed last with flags 2; its data
// pointer is the word at map + 8 and entries there are 112 bytes apart: the word at +32 is an emitter
// pointer and the word at +36 a sprite index.
// 1. Only if IsIopStreamingEnabled__6VIFile() is nonzero: Init__14VIEmitterQueryi12VISpriteType(query,
//    32, 12) and QueryEmittersAtLocation(scene, camera, query); if that is not negative, walk the map for
//    the stream-audio sprite with the largest priority (GetPriority__C19VIStreamAudioSprite, a later one wins
//    ties; the initial best is index -1 and priority 0). If none, the scene's +1596 word becomes -1 and
//    StopBgm__13VISoundDevicei(device, 1) is called. Otherwise, unless that sprite is already the current one
//    and not IsLooping, the +1596 word becomes its index and PlayBgm__13VISoundDeviceiPCc(device, 1, path)
//    is called with path = "Music\" + GetRelativePathname__C19VIStreamAudioSprite(sprite) (strcat).
// 2. Init__14VIEmitterQueryi12VISpriteType(query, 32, 9) and a second query. If it is not negative: for each
//    map entry, with the emitter E and the sound sprite S found through the sprite index: when the word at
//    S + 72 is not (frame counter - 1) the sound is started, voice = Play__13VISoundDeviceii12VISOUNDCLASS(
//    device, *(S + 64), 0, 0), stored at S + 68 (entries with a non-positive voice are skipped). Then the
//    camera position (camera + 28) minus E + 68 gives a horizontal vector (x, 0, z) and camera + 16, 0,
//    -(camera + 8) a second one; both are normalised (Normalize__7VIVect3) and their dot product is the pan.
//    With distance = Distance__C7VIVect3RC7VIVect3(camera + 28, E + 68) the ratio is
//    distance / (E[92] * S[44]); AttenuationMode__C13VISoundSprite(S) picks the falloff: mode 1 gives
//    1 - r^4, mode 2 gives (1 - r)^4, any other mode 1 - r. SetVolume(device, S[68], falloff * S[84]); the pan
//    is S's if IsPanEnabled__C13VISoundSprite(S) else 0.0, via SetPan__13VISoundDeviceif; finally S + 72 is set
//    to the frame counter.
// 3. For each of the thirty-two slots: a slot that is not -1 is looked up by walking the sprite pool's index
//    range (Begin, End, Next); when it is found and the sprite's +72 word is not the frame counter,
//    Stop__13VISoundDevicei(device, S[68]) is called. The slot is then set to -1 (found or not).
// 4. If the second query was not negative, the slots at +1464, +1468, ... are refilled from the sprite
//    indices of the map entries in order (no bound check).
// Inferred: "emitters" and "voices" follow the callee names; the +72 word is a last-updated frame mark.
// Unknown: the meaning of the strings' directory prefix, the query/map internals, every other sprite field.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
struct VIScene;
struct VICamera;

void __t10VIMultiMap2ZfZ14VIEmitterEntry(void* map);
void _$_t10VIMultiMap2ZfZ14VIEmitterEntry(void* map, int flags);
int IsIopStreamingEnabled__6VIFile();
void Init__14VIEmitterQueryi12VISpriteType(void* query, int capacity, int type);
int QueryEmittersAtLocation__7VISceneRC8VICameraR14VIEmitterQuery(VIScene* scene, const VICamera* camera, void* query);
int Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(const void* map);
int End__Ct10VIMultiMap2ZfZ14VIEmitterEntry(const void* map);
int Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi(const void* map, int index);
int Begin__Ct6VIPool1ZP8VISprite(const void* pool);
int End__Ct6VIPool1ZP8VISprite(const void* pool);
int Next__Ct6VIPool1ZP8VISpritei(const void* pool, int index);
int GetPriority__C19VIStreamAudioSprite(const void* sprite);
int IsLooping__C19VIStreamAudioSprite(const void* sprite);
const char* GetRelativePathname__C19VIStreamAudioSprite(const void* sprite);
char* strcat(char* destination, const char* source);
void PlayBgm__13VISoundDeviceiPCc(void* device, int a, const char* path);
void StopBgm__13VISoundDevicei(void* device, int a);
int Play__13VISoundDeviceii12VISOUNDCLASS(void* device, int a, int b, int soundClass);
void SetVolume__13VISoundDeviceif(void* device, int voice, float volume);
void SetPan__13VISoundDeviceif(void* device, int voice, float pan);
void Stop__13VISoundDevicei(void* device, int voice);
void Normalize__7VIVect3(float* vector);
float Distance__C7VIVect3RC7VIVect3(const float* a, const float* b);
int AttenuationMode__C13VISoundSprite(const void* sprite);
int IsPanEnabled__C13VISoundSprite(const void* sprite);

static unsigned word_at(const char* address) { return *reinterpret_cast<const unsigned*>(address); }
static float float_at(const char* address) { return *reinterpret_cast<const float*>(address); }

// A pool entry's sprite pointer (the first word of the 12-byte entry `index`).
static char* sprite_of(const char* scene, int index) {
    const char* data = NORRATH_TRANSLATE(word_at(scene + 996 + 8));
    return NORRATH_TRANSLATE(word_at(data + 12 * index));
}

struct Locals {
    char query[16];
    char map[32];
};

int UpdateSoundEmitters__7VISceneR8VICamera(VIScene* scene_, VICamera* camera_) {
    char* scene = reinterpret_cast<char*>(scene_);
    const char* camera = reinterpret_cast<const char*>(camera_);
    Locals locals;
    char* query = locals.query;
    char* map = locals.map;
    __t10VIMultiMap2ZfZ14VIEmitterEntry(map);
    void* device = 0;
    if (IsIopStreamingEnabled__6VIFile()) {
        Init__14VIEmitterQueryi12VISpriteType(query, 32, 12);
        if (QueryEmittersAtLocation__7VISceneRC8VICameraR14VIEmitterQuery(scene_, camera_, query) >= 0) {
            int best = -1;
            int best_priority = 0;
            for (int i = Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map); i != End__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map);
                 i = Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi(map, i)) {
                const char* entry = NORRATH_TRANSLATE(word_at(map + 8)) + 112 * i;
                int index = static_cast<int>(word_at(entry + 36));
                if (!(GetPriority__C19VIStreamAudioSprite(sprite_of(scene, index)) < best_priority)) {
                    best = index;
                    best_priority = GetPriority__C19VIStreamAudioSprite(sprite_of(scene, index));
                }
            }
            device = NORRATH_TRANSLATE(word_at(scene + 19360));
            if (best < 0) {
                *reinterpret_cast<int*>(scene + 1596) = -1;
                StopBgm__13VISoundDevicei(NORRATH_TRANSLATE(word_at(scene + 19360)), 1);
            } else if (*reinterpret_cast<int*>(scene + 1596) != best || IsLooping__C19VIStreamAudioSprite(sprite_of(scene, best))) {
                char path[256];
                path[0] = 'M'; path[1] = 'u'; path[2] = 's'; path[3] = 'i'; path[4] = 'c'; path[5] = '\\'; path[6] = 0;
                *reinterpret_cast<int*>(scene + 1596) = best;
                strcat(path, GetRelativePathname__C19VIStreamAudioSprite(sprite_of(scene, best)));
                PlayBgm__13VISoundDeviceiPCc(NORRATH_TRANSLATE(word_at(scene + 19360)), 1, path);
            }
        }
    }
    (void)device;
    Init__14VIEmitterQueryi12VISpriteType(query, 32, 9);
    bool queried = QueryEmittersAtLocation__7VISceneRC8VICameraR14VIEmitterQuery(scene_, camera_, query) >= 0;
    if (queried) {
        for (int i = Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map); i != End__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map);
             i = Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi(map, i)) {
            const char* entry = NORRATH_TRANSLATE(word_at(map + 8)) + 112 * i;
            int index = static_cast<int>(word_at(entry + 36));
            char* sound = sprite_of(scene, index);
            if (*reinterpret_cast<int*>(sound + 72) != *reinterpret_cast<int*>(scene + 1592) - 1) {
                int voice = Play__13VISoundDeviceii12VISOUNDCLASS(NORRATH_TRANSLATE(word_at(scene + 19360)),
                                                                  *reinterpret_cast<int*>(sound + 64), 0, 0);
                *reinterpret_cast<int*>(sprite_of(scene, index) + 68) = voice;
                if (voice <= 0) continue;
            }
            const char* emitter = NORRATH_TRANSLATE(word_at(NORRATH_TRANSLATE(word_at(map + 8)) + 112 * i + 32));
            float toward[3] = {0.0f, 0.0f, 0.0f};
            float side[3] = {0.0f, 0.0f, 0.0f};
            toward[0] = float_at(camera + 28) - float_at(emitter + 68);
            toward[2] = float_at(camera + 36) - float_at(emitter + 76);
            side[0] = float_at(camera + 16);
            side[2] = -float_at(camera + 8);
            Normalize__7VIVect3(toward);
            Normalize__7VIVect3(side);
            float pan = toward[0] * side[0] + toward[2] * side[2];
            sound = sprite_of(scene, index);
            float ratio = Distance__C7VIVect3RC7VIVect3(reinterpret_cast<const float*>(camera + 28),
                                                        reinterpret_cast<const float*>(emitter + 68)) /
                          (float_at(emitter + 92) * float_at(sound + 44));
            int mode = AttenuationMode__C13VISoundSprite(sound);
            float falloff;
            if (mode == 1) {
                float square = ratio * ratio;
                ratio = square * square;
                falloff = 1.0f - ratio;
            } else if (mode == 2) {
                ratio = 1.0f - ratio;
                float square = ratio * ratio;
                ratio = square * square;
                ratio = 1.0f - ratio;
                falloff = 1.0f - ratio;
            } else {
                falloff = 1.0f - ratio;
            }
            void* device_now = NORRATH_TRANSLATE(word_at(scene + 19360));
            SetVolume__13VISoundDeviceif(device_now, *reinterpret_cast<int*>(sound + 68), falloff * float_at(sound + 84));
            float used_pan = IsPanEnabled__C13VISoundSprite(sound) ? pan : 0.0f;
            SetPan__13VISoundDeviceif(NORRATH_TRANSLATE(word_at(scene + 19360)), *reinterpret_cast<int*>(sound + 68), used_pan);
            *reinterpret_cast<int*>(sprite_of(scene, index) + 72) = *reinterpret_cast<int*>(scene + 1592);
        }
    }
    for (int slot = 0; slot < 32; ++slot) {
        int* slot_word = reinterpret_cast<int*>(scene + 1464 + 4 * slot);
        if (*slot_word == -1) continue;
        bool found = false;
        const char* pool = scene + 996;
        int p = Begin__Ct6VIPool1ZP8VISprite(pool);
        while (p != End__Ct6VIPool1ZP8VISprite(pool)) {
            if (p == *slot_word) {
                found = true;
                break;
            }
            p = Next__Ct6VIPool1ZP8VISpritei(pool, p);
        }
        if (found) {
            char* sound = sprite_of(scene, *slot_word);
            if (*reinterpret_cast<int*>(sound + 72) != *reinterpret_cast<int*>(scene + 1592))
                Stop__13VISoundDevicei(NORRATH_TRANSLATE(word_at(scene + 19360)), *reinterpret_cast<int*>(sound + 68));
        }
        *slot_word = -1;
    }
    if (queried) {
        int* out = reinterpret_cast<int*>(scene + 1460);
        for (int i = Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map); i != End__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map);
             i = Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi(map, i)) {
            ++out;
            *out = static_cast<int>(word_at(NORRATH_TRANSLATE(word_at(map + 8)) + 112 * i + 36));
        }
    }
    _$_t10VIMultiMap2ZfZ14VIEmitterEntry(map, 2);
    return 0;
}
}
