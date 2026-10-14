// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST
#include <cstdio>
#include <cstring>
#include <deque>
#include <string>
#include <vector>

extern "C" {
int frameNm;
unsigned world;
int levelFlags;
int gameDifficulty;
char levelName[400];
unsigned char norrath_auto_chest_msg_run_fuseParticle_bytes[80];
unsigned char norrath_auto_chest_msg_run_smokeParticle_bytes[80];
unsigned char global_item_view[512] asm("_4item$globalItem");
}
#include "msg_run.cpp"

static std::string log_text;
static unsigned char* current;
static std::deque<unsigned> rolls;
static std::deque<int> check_results, alloc_results, tag_results, proxy_results;
static int perturb_result, strcmp_result, tagstring_result;
static std::vector<std::vector<float>> numbers;  // per interesting call: tag then values
static std::string lump_name, sound_name;
static int sound_handle;
static std::vector<std::string> sounds;
static std::vector<int> effects;
static unsigned char block_storage[1024];
static int alloc_sizes[32], alloc_count;
static int stopped_handle, shake_set;
static float shake_amount;
static std::vector<int> prop_ids; static std::vector<long> prop_layers; static std::vector<std::string> prop_names;
static int fires, deletes, delete_flag; static void* delete_target;
static int gold_amount; static int create_count, potion_count, treasure_count;
static float treasure_args[4][3];
static unsigned char proxy_tables[4];

static bool same(const char* a, const char* b) { for (unsigned i = 0;; ++i) { if (a[i] != b[i]) return false; if (a[i] == 0) return true; } }
static unsigned next_roll() { if (rolls.empty()) return 0x4000; unsigned v = rolls.front(); rolls.pop_front(); return v; }
extern "C" {
void genericMove__10GameObjectG6Point3P10GameObject(void*, const float* move, void* other) { log_text += other == nullptr ? "M" : "M?"; numbers.push_back({1, move[0], move[1], move[2]}); }
int worldPerturbWater__FP12_worldHeaderG6Point3ff(void* w, const float* position, float a, float b) { log_text += w == nullptr ? "P" : "P?"; numbers.push_back({2, a, b, position[0]}); return perturb_result; }
int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void*, const float*, float range, float* out) {
    log_text += out == nullptr ? "W" : "W?"; numbers.push_back({3, range});
    if (check_results.empty()) return 0;
    const int v = check_results.front(); check_results.pop_front(); return v;
}
int sprintf(char* out, const char* format, ...) {
    // The only formats used are plain names, or the splash name with a %d of 1.
    std::string f = format;
    const size_t at = f.find("%d");
    if (at != std::string::npos) f = f.substr(0, at) + "1" + f.substr(at + 2);
    std::strcpy(out, f.c_str());
    log_text += "F"; return static_cast<int>(f.size());
}
int strcmp(const char* a, const char* b) { log_text += std::string("C(") + (a == levelName + 81 ? "level" : "?") + "," + b + ")"; return strcmp_result; }
void* lumpFindResource__FPcT0(char* lump, char* name) { lump_name = lump; sound_name = name; log_text += "L"; return &sound_handle; }
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* s, void* object, float volume) {
    log_text += s == &sound_handle && object == current ? "Q" : "Q?"; sounds.push_back(sound_name); numbers.push_back({4, volume}); return 4242;
}
void soundStopSound__Fi(int handle) { log_text += "X"; stopped_handle = handle; }
void objectRemoveFromRunList__FP10GameObject(void*) { log_text += "u"; }
void gameSetCameraShake__Ffb(float amount, int flag) { log_text += flag == 0 ? "K" : "K?"; shake_amount = amount; }
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void*, const float*, short angle, void* state, int index, float* out, float* other, float scale) {
    log_text += state == nullptr && index == 0 && other == nullptr && scale == 1.0f ? "T" : "T?"; numbers.push_back({5, static_cast<float>(angle)});
    out[0] = 100.0f; out[1] = 200.0f; out[2] = 300.0f;
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* def, float* position, float* velocity, float age) {
    log_text += def == norrath_auto_chest_msg_run_fuseParticle_bytes ? "f" : def == norrath_auto_chest_msg_run_smokeParticle_bytes ? "s" : "?";
    numbers.push_back({6, position[0], position[1], position[2], velocity[0], velocity[1], velocity[2], age});
}
void makeExplosion__FG6Point3f(const float* position, float scale) { log_text += "E"; numbers.push_back({7, position[2], scale}); }
void objectRadialDamage__FP10GameObjectG6Point3iffiii(void*, const float*, int damage, float a, float b, int c, int d, int e) {
    log_text += c == 0x40800 && d == 0 && e == 0 ? "R" : "R?"; numbers.push_back({8, static_cast<float>(damage), a, b});
}
void floorAdd__FG6Point3sfii(const float* position, short angle, float extent, int kind, int extra) {
    log_text += "D"; numbers.push_back({9, position[2], static_cast<float>(angle), extent, static_cast<float>(kind), static_cast<float>(extra)});
}
void* blockAlloc__Fi(int bytes) {
    log_text += "b"; alloc_sizes[alloc_count++ & 31] = bytes;
    int v = 1;
    if (!alloc_results.empty()) { v = alloc_results.front(); alloc_results.pop_front(); }
    return v ? block_storage : nullptr;
}
void __11LightEffectG6Point3ffffffffffffP10GameObjecti(void*, const float*, float, float, float, float, float, float, float, float, float p11, float p12, float p13, float p14, void* object, int flag) {
    log_text += object == nullptr && flag == 0 ? "I" : "I?"; numbers.push_back({10, p11, p12, p13, p14});
}
char* findTagString__10GameObjectPci(void*, char* tag, int) { log_text += same(tag, "reduceCarnage") ? "t" : "t?"; return tagstring_result ? tag : nullptr; }
int findTagInt__10GameObjectPci(void*, char*, int) { log_text += "i"; if (tag_results.empty()) return 0; const int v = tag_results.front(); tag_results.pop_front(); return v; }
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void*, const float*, char* a, char* b, char* c, long layer, int id, void* description, float scale) {
    log_text += same(a, "equipmnt.lmp") ? "H" : "h";
    prop_ids.push_back(id); prop_layers.push_back(layer); prop_names.push_back(a);
    (void)b; (void)c; (void)description; (void)scale;
}
void setOnFire__6PhPropi(void*, int value) { log_text += "Z"; ++fires; (void)value; }
void initStuff__6PhPropi(void* prop, int id) { log_text += "J"; numbers.push_back({11, static_cast<float>(id), *reinterpret_cast<float*>(static_cast<unsigned char*>(prop) + 252), *reinterpret_cast<float*>(static_cast<unsigned char*>(prop) + 256)}); }
void addEffect__FiG6Point3(int effect, const float* position) { log_text += "A"; effects.push_back(effect); numbers.push_back({12, position[0], position[1], position[2]}); }
unsigned eRand__10GameObject(void*) { log_text += "r"; return next_roll(); }
float gameTreasureRamp__Fv() { log_text += "G"; return 2.0f; }
void __4GoldG6Point3i(void*, const float*, int amount) { log_text += "g"; gold_amount = amount; }
void create__4itemPcPQ24item11descriptionii(char* name, void*, int a, int b) { log_text += same(name, "arrows") && a == -1 && b == 0 ? "a" : "a?"; ++create_count; }
void makeRandomPotion__4itemPQ24item11description(void*) { log_text += "p"; ++potion_count; }
int getProxyMeshId__4itemPQ24item11description(void*) { log_text += "y"; if (proxy_results.empty()) return 1; const int v = proxy_results.front(); proxy_results.pop_front(); return v; }
void dropTreasure__4itemG6Point3iffffffPci(const float* position, int angle, float a, float b, float c, float d, float e, float f, char* name, int extra) {
    log_text += name == nullptr && extra == 0 && position[2] == 3.25f && a == 36.0f && b == 2.0f && c == 0.0f && d == 0.0f ? "d" : "d?";
    treasure_args[treasure_count & 3][0] = e; treasure_args[treasure_count & 3][1] = f; treasure_args[treasure_count & 3][2] = static_cast<float>(angle); ++treasure_count;
}
float icos__Fi(int angle) { log_text += "c"; numbers.push_back({13, static_cast<float>(angle)}); return 0.5f; }
float isin__Fi(int angle) { log_text += "n"; numbers.push_back({14, static_cast<float>(angle)}); return 0.25f; }
}
static void slot8(void* self, int flag) { log_text += "V"; delete_target = self; delete_flag = flag; ++deletes; }

static unsigned char object[1100];
static unsigned char vtable[64];
static unsigned char list[64];
static int failures;
static void expect(bool ok, const char* what, int a = 0, int b = 0) {
    if (!ok) { std::fprintf(stderr, "fail: %s (%d, %d) log=%s\n", what, a, b, log_text.c_str()); ++failures; }
}
static void put32(unsigned off, unsigned v) { std::memcpy(object + off, &v, 4); }
static void put16(unsigned off, unsigned v) { unsigned short s = static_cast<unsigned short>(v); std::memcpy(object + off, &s, 2); }
static void putf(unsigned off, float v) { std::memcpy(object + off, &v, 4); }
static unsigned word(unsigned off) { unsigned v; std::memcpy(&v, object + off, 4); return v; }
static float single(unsigned off) { float v; std::memcpy(&v, object + off, 4); return v; }
static void reset(unsigned state, unsigned flags) {
    std::memset(object, 0x3c, sizeof object);
    std::memset(vtable, 0, sizeof vtable); std::memset(list, 0, sizeof list);
    rtti_c::host_note(vtable); rtti_c::host_note(list); rtti_c::host_note(reinterpret_cast<void*>(slot8));
    put32(72, rtti_c::address_bits(vtable));
    { const short adj = 0; std::memcpy(vtable + 8, &adj, 2); const unsigned f = rtti_c::address_bits(reinterpret_cast<void*>(slot8)); std::memcpy(vtable + 12, &f, 4); }
    put32(316, state); put32(292, flags); put32(304, 0); put32(308, 0x1234); object[312] = 0; object[288] = 0; object[40] = 20;
    putf(296, 0.0f); putf(300, 0.0f); putf(28, 10.0f); putf(32, 20.0f);
    putf(16, 1.5f); putf(20, -2.5f); putf(24, 3.25f); put16(56, 0x4000);
    put32(12, 0); put32(216, 0); put32(144, 0);
    std::memcpy(object + 109, "abc", 4); std::memcpy(object + 224, "def", 4); std::memcpy(object + 256, "ghi", 4);
    world = 0; frameNm = 1; levelFlags = 0; gameDifficulty = 0; std::strcpy(levelName + 81, "level9");
    log_text.clear(); rolls.clear(); check_results.clear(); alloc_results.clear(); tag_results.clear(); proxy_results.clear();
    perturb_result = 0; strcmp_result = 1; tagstring_result = 0; numbers.clear(); sounds.clear(); effects.clear();
    alloc_count = 0; stopped_handle = -1; prop_ids.clear(); prop_layers.clear(); prop_names.clear(); fires = deletes = 0; delete_target = nullptr; delete_flag = -1;
    gold_amount = -1; create_count = potion_count = treasure_count = 0; current = object;
    std::memset(block_storage, 0, sizeof block_storage);
}
static void with_resources(int count) {
    put32(216, rtti_c::address_bits(list));
    list[18] = static_cast<unsigned char>(count);
}
static const std::vector<float>* find(float tag, int nth = 0) {
    for (const auto& v : numbers) if (v[0] == tag && nth-- == 0) return &v;
    return nullptr;
}

int main() {
    // State 4 returns at once; flags decide whether the object is moved.
    reset(4, 0); msg_run__13AutoContainer(object); expect(log_text.empty(), "state 4");
    reset(2, 0); putf(296, 3.0f); putf(300, -4.0f); frameNm = 1; msg_run__13AutoContainer(object);
    expect(log_text == "M" && find(1)->at(1) == 3.0f && find(1)->at(2) == -4.0f && find(1)->at(3) == 0.0f, "genericMove with the velocity");
    reset(2, 8); frameNm = 1; msg_run__13AutoContainer(object); expect(log_text.empty(), "bit 8 skips the move");
    // Water.
    reset(2, 8); object[312] = 1; putf(296, 1.0f); perturb_result = 1; msg_run__13AutoContainer(object);
    expect(log_text == "P" && find(2)->at(1) == 300.0f && find(2)->at(2) == 12.0f && object[312] == 1, "in water, fast: perturb keeps the flag");
    reset(2, 8); object[312] = 1; putf(296, 1.0f); perturb_result = 0; msg_run__13AutoContainer(object); expect(log_text == "P" && object[312] == 0, "perturb zero clears the flag");
    reset(2, 8); object[312] = 0xff; check_results = {1}; msg_run__13AutoContainer(object); expect(log_text == "W" && find(3)->at(1) == 12.0f && object[312] == 0xff, "in water, slow: check keeps the flag");
    reset(2, 8); object[312] = 1; check_results = {0}; msg_run__13AutoContainer(object); expect(log_text == "W" && object[312] == 0, "check zero clears the flag");
    reset(2, 8); frameNm = 8; check_results = {1}; msg_run__13AutoContainer(object);
    expect(log_text == "WrFLQP" && object[312] == 1 && sounds[0] == "foly_wtrsplash_sml1.anm" && find(4)->at(1) == rtti_c::single_from_bits(0x3e999999u), "splash");
    expect(find(2)->at(1) == 1000.0f && find(2)->at(2) == 24.0f && lump_name == "fx.lmp", "splash perturb arguments");
    reset(2, 8); frameNm = 9; msg_run__13AutoContainer(object); expect(log_text.empty(), "no water test off the 8th frame");
    reset(2, 8); frameNm = 16; check_results = {0}; msg_run__13AutoContainer(object); expect(log_text == "W" && object[312] == 0, "no water found");
    // State 0: damping and leaving the run list.
    reset(0, 8); putf(296, 0.05f); putf(300, 0.05f); msg_run__13AutoContainer(object);
    expect(log_text == "u" && single(296) == 0.05f * rtti_c::single_from_bits(0x3f5c28f5u), "state 0 slow object leaves");
    reset(0, 8); putf(296, 1.0f); msg_run__13AutoContainer(object); expect(log_text.empty(), "state 0 fast object stays");
    reset(2, 8); putf(296, 1.0f); msg_run__13AutoContainer(object); expect(log_text.empty() && single(296) == rtti_c::single_from_bits(0x3f5c28f5u), "damping in other states");
    // Fuse.
    reset(2, 4 | 8); put32(304, 180); put32(144, 0x1a90000); with_resources(0); rolls = {0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000};
    rtti_c::host_note(reinterpret_cast<void*>(0x1a90000));
    msg_run__13AutoContainer(object);
    expect(log_text == "LQTrrrfrrrs" && sounds[0] == "expl_fuse.anm" && word(308) == 4242u && word(304) == 179u && word(316) == 2u, "fuse start");
    expect(find(5)->at(1) == static_cast<float>(static_cast<short>(0x4000 - 16384)), "attachment angle is the facing minus 16384");
    expect(find(6, 0)->at(4) == -2.5f && find(6, 0)->at(5) == -2.5f && find(6, 0)->at(6) == 0.5f * 25.0f, "fuse particle velocity");
    reset(2, 4 | 8); put32(304, 179); msg_run__13AutoContainer(object); expect(log_text.find('L') == std::string::npos && word(304) == 178u, "no sound off 180");
    reset(2, 4 | 8); put32(304, 1); msg_run__13AutoContainer(object); expect(word(304) == 0u && word(316) == 1u && log_text.substr(0, 2) == "K" "T" && shake_amount == 10.0f, "countdown reaches zero");
    reset(1, 4 | 8); put32(304, 0); alloc_results = {1, 1}; msg_run__13AutoContainer(object);
    reset(3, 4 | 8); put32(304, 0); msg_run__13AutoContainer(object); expect(log_text.empty(), "state other than 1 does nothing at zero");
    // Explosion in detail.
    for (int kind : {0, 1, 2, 3}) {
        reset(1, 4 | 8); put32(304, 0); object[288] = static_cast<unsigned char>(kind); object[40] = 36; with_resources(2); tagstring_result = 1; tag_results = {1}; rolls = {0x4000, 0x2000, 0x1};
        gameDifficulty = 2; levelFlags = 0x10000; strcmp_result = 1;
        msg_run__13AutoContainer(object);
        const char* names[] = {"crsh_barrel.anm", "crsh_claypot.anm", "crsh_basket.anm"};
        expect(sounds.size() == (kind < 3 ? 2u : 1u) && sounds[0] == "expl_sweeten.anm" && (kind >= 3 || sounds[1] == names[kind]), "explosion sounds", kind);
        expect(find(7)->at(2) == 2.0f && find(7)->at(1) == 3.25f + rtti_c::single_from_bits(0x416b3333u), "explosion scale below 37", kind);
        expect(find(8)->at(1) == 180.0f && find(8)->at(2) == 40.0f && find(8)->at(3) == 108.0f, "damage 60 * 3, radius 3 * 36", kind);
        expect(find(9)->at(3) == 72.0f && find(9)->at(4) == 9.0f && find(9)->at(5) == 0.0f && find(9)->at(2) == static_cast<float>(static_cast<short>(0x4000)), "decal", kind);
        expect(deletes == 1 && delete_flag == 3 && stopped_handle == 0x1234, "delete through slot 8", kind);
    }
    reset(1, 4 | 8); put32(304, 0); object[40] = 37; levelFlags = 0x18000; strcmp_result = 0; gameDifficulty = 3; with_resources(0); msg_run__13AutoContainer(object);
    expect(find(7)->at(2) == 4.0f && find(8)->at(1) == 24.0f && find(8)->at(3) == 111.0f, "explosion scale 4, tutor damage 4 * 6", 0);
    reset(1, 4 | 8); put32(304, 0); object[40] = 200; levelFlags = 0x8000; gameDifficulty = 0; msg_run__13AutoContainer(object);
    expect(find(8)->at(1) == 220.0f && find(9)->at(3) == 96.0f && find(10)->at(1) == 300.0f && find(10)->at(2) == 200.0f && find(10)->at(3) == 1.0f && find(10)->at(4) == rtti_c::single_from_bits(0x3eb33333u), "damage 220, extent and light capped");
    reset(1, 4 | 8); put32(304, 0); object[40] = 10; gameDifficulty = 4; msg_run__13AutoContainer(object);
    expect(find(8)->at(1) == 15.0f && find(9)->at(3) == 20.0f && find(10)->at(1) == 40.0f, "damage 15, extent 2 * 10, light 4 * 10");
    reset(1, 4 | 8); put32(304, 0); alloc_results = {0}; msg_run__13AutoContainer(object); expect(find(10) == nullptr && log_text.find('I') == std::string::npos, "no light when the allocation fails");
    // Props on destruction: carnage skips and fire.
    reset(1, 4 | 8); put32(304, 0); with_resources(3); tagstring_result = 1; tag_results = {1}; rolls = {0x4000, 0x0, 0x1, 0x8, 0x0, 0x0, 0x1};
    alloc_results = {1, 1, 1, 1, 1, 1, 1};
    msg_run__13AutoContainer(object);
    expect(prop_ids.size() <= 3 && !prop_ids.empty() && prop_ids[0] == 534, "destruction props use id 534");
    // Opened container.
    reset(1, 1 | 8); with_resources(2); rolls = {0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 30000, 30000, 30000};
    msg_run__13AutoContainer(object);
    expect(effects.size() == 6 && effects[0] == 2 && word(316) == 4u && word(12) == 16387u && prop_ids.size() == 1 && prop_ids[0] == 526 && prop_layers[0] == 1, "opened: debris, props, state 4, flag word");
    reset(1, 1 | 8 | 0x400); msg_run__13AutoContainer(object); expect(effects.size() == 6 && effects[0] == 8, "opened: effect 8 with flag 0x400");
    reset(1, 1 | 8 | 0x200); with_resources(3); msg_run__13AutoContainer(object); expect(prop_ids.size() == 3 && prop_layers[1] == 2, "opened: step 1 with flag 0x200");
    reset(1, 1 | 8); with_resources(3); msg_run__13AutoContainer(object); expect(prop_ids.size() == 2 && prop_layers[1] == 4, "opened: step 2 without it");
    reset(0, 1 | 8); msg_run__13AutoContainer(object); expect(log_text.empty() || log_text == "u", "opened needs state 1");
    reset(1, 8); msg_run__13AutoContainer(object); expect(log_text.empty(), "neither bit does nothing");
    // Gold and loot.
    reset(1, 1 | 8); rolls = {0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 23999, 3, 30000};
    msg_run__13AutoContainer(object);
    expect(gold_amount == 1 + static_cast<int>(3.0f * 2.0f) && log_text.find("rGg") != std::string::npos, "gold below 24000");
    reset(1, 1 | 8); rolls = {0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 24000, 30000}; msg_run__13AutoContainer(object);
    expect(gold_amount == -1, "no gold at 24000");
    reset(1, 1 | 8); rolls = {0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 30000, 8999, 0xf617, 8000};
    msg_run__13AutoContainer(object);
    expect(create_count == 1 && potion_count == 0 && treasure_count == 0, "one arrows item (roll 0xf617 is not above 0xf617)");
    reset(1, 1 | 8); rolls = {0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 30000, 8999, 0xf618, 15000};
    msg_run__13AutoContainer(object);
    expect(create_count == 0 && potion_count == 2, "two potions");
    reset(1, 1 | 8); rolls = {0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 30000, 8999, 0xffdc, 26000};
    msg_run__13AutoContainer(object);
    expect(create_count == 0 && potion_count == 0 && treasure_count == 12, "three equipment drops of four");
    expect(treasure_args[0][0] == 1.5f && treasure_args[0][1] == 1.0f && treasure_args[1][0] == 2.5f && treasure_args[1][1] == rtti_c::single_from_bits(0x3c23d70au) && treasure_args[3][0] == 4.5f && treasure_args[3][1] == rtti_c::single_from_bits(0x38d1b717u), "drop strengths");
    reset(1, 1 | 8); rolls = {0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 30000, 9000}; msg_run__13AutoContainer(object);
    expect(create_count == 0 && potion_count == 0 && treasure_count == 0, "no loot at 9000");
    reset(1, 1 | 8); rolls = {0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 30000, 8999, 1, 10000}; msg_run__13AutoContainer(object);
    expect(potion_count == 1, "choice 10000 is a potion");
    reset(1, 1 | 8); rolls = {0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 0x4000,0x4000,0x4000, 30000, 8999, 1, 25000}; msg_run__13AutoContainer(object);
    expect(treasure_count == 4, "choice 25000 is an equipment drop");
    return failures ? 1 : 0;
}
