#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
// Entity and definition words are handles into this table on the host.
static unsigned char* registry[64];
static unsigned registry_count = 1;
static unsigned handle_for(void* p) { registry[registry_count] = static_cast<unsigned char*>(p); return registry_count++; }
#define NORRATH_ADDRESS_FROM_WORD(w) (registry[(w)])
#include "debug_recall.cpp"

extern "C" {
unsigned char player[4 * 23984];
char levelName[322];
int nmPlayersAlive, gamePause, gameControlPaused;
int playersAlive[4];
unsigned char recallOn, particlesPaused;
}
static std::vector<std::string> events;
static std::string last_key; static int last_player, last_duration; static int msg_count;
static int no_save_zone; static unsigned short text_token[2];
static int removed_count, removed_player, removed_entry, removed_a, removed_b;
static int alloc_calls, alloc_fail_at = -1; static unsigned char block[4][96];
static int ctor_calls; static void* ctor_self[4]; static norrath_point3 ctor_pos[4];
static std::string lump_a, lump_b; static float seq_gain; static void* seq_obj; static void* seq_header;
static int status; static int fades;
extern "C" {
void debugClearStartMenu__Fv() { events.push_back("clear"); }
int gameCheckNoSaveZone__Fv() { return no_save_zone; }
unsigned short* textGetString__FPc(const char* k) { last_key = k; return text_token; }
void hudSetInfoMessage__FPUsiic(unsigned short* t, int d, int p, char e) {
    if (t != text_token || e != 0) events.push_back("badmsg");
    ++msg_count; last_duration = d; last_player = p; events.push_back("msg:" + last_key);
}
void removeItemFromInventory__Fiiii(int p, int e, int a, int b) { ++removed_count; removed_player = p; removed_entry = e; removed_a = a; removed_b = b; }
void* blockAlloc__Fi(int n) { if (n != 96) events.push_back("badsize"); int i = alloc_calls++; return i == alloc_fail_at ? nullptr : block[i]; }
void __12RecallEffectG6Point3(void* s, norrath_point3* p) { ctor_self[ctor_calls] = s; ctor_pos[ctor_calls++] = *p; }
void* lumpFindResource__FPcT0(const char* l, const char* n) { lump_a = l; lump_b = n; return &lump_a; }
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float g) { seq_header = h; seq_obj = o; seq_gain = g; events.push_back("play"); return 0; }
int soundGetStreamPlayStatus__Fi(int s) { if (s != 0) events.push_back("badstream"); return status; }
void soundFadeStream__Fi(int s) { if (s == 0) ++fades; }
}
static void store_word(unsigned char* p, unsigned v) { for (int i = 0; i < 4; ++i) p[i] = static_cast<unsigned char>(v >> (8 * i)); }
static void store_float(unsigned char* p, float f) { unsigned w; std::memcpy(&w, &f, 4); store_word(p, w); }
static unsigned char entity_bytes[4][64];
static const char* potion_name = "recallpotion";
static const char* other_name = "sword";
static const char* names[8];
static unsigned char defs[8][8];
static void reset() {
    events.clear(); msg_count = 0; no_save_zone = 0; removed_count = 0; alloc_calls = 0; alloc_fail_at = -1;
    ctor_calls = 0; fades = 0; status = 0; seq_obj = (void*)1; lump_a.clear(); lump_b.clear(); last_key.clear();
    std::memset(player, 0, sizeof player); std::memset(levelName, 0, sizeof levelName);
    std::strcpy(levelName + 81, "dungeon"); std::strcpy(reinterpret_cast<char*>(player + 36), "somewhere");
    registry_count = 1; recallOn = 0; gamePause = 9; gameControlPaused = 9; particlesPaused = 9;
    nmPlayersAlive = 0; std::memset(entity_bytes, 0, sizeof entity_bytes);
    for (int i = 0; i < 4; ++i) { store_float(player + i * 23984 + 172, 10.0f); }
}
static void make_entity(int idx) {
    store_word(player + idx * 23984 + 21872, handle_for(entity_bytes[idx]));
    store_float(entity_bytes[idx] + 16, 1.5f + idx); store_float(entity_bytes[idx] + 20, 2.5f + idx);
    store_float(entity_bytes[idx] + 24, 3.5f + idx);
}
static void put_item(int idx, int slot, const char* name, unsigned short qty) {
    unsigned char* e = player + idx * 23984 + 9552 + slot * 72;
    names[slot] = name;
    // the name pointer word of a definition is itself a handle to the characters
    store_word(defs[slot], handle_for(const_cast<char*>(name)));
    store_word(e + 8, handle_for(defs[slot]));
    e[12] = qty & 0xff; e[13] = qty >> 8;
}
static bool only_message(const char* key, int player_arg) {
    return events.size() == 2 && events[0] == "clear" && events[1] == std::string("msg:") + key &&
           last_player == player_arg && last_duration == 180 && msg_count == 1 && !recallOn && ctor_calls == 0;
}
int main() {
    int fail = 0;
    // 1 no-save zone
    reset(); no_save_zone = 1; debugRecall__Fi(1);
    if (!only_message("ErrorCannotRecall", -1)) return 1;
    // 2 dead: zero, negative; NaN is not dead (comparison is false)
    reset(); store_float(player + 23984 + 172, 0.0f); debugRecall__Fi(1);
    if (!only_message("ErrorCannotRecallDead", -1)) return 2;
    reset(); store_float(player + 2 * 23984 + 172, -4.0f); debugRecall__Fi(2);
    if (!only_message("ErrorCannotRecallDead", -1)) return 3;
    // 3 hubs
    const char* hubs[] = {"kelethin", "gnomehub", "beachhub", "elfhub", "elfhubn", "airhub"};
    for (const char* h : hubs) {
        reset(); std::strcpy(levelName + 81, h); std::strcpy(reinterpret_cast<char*>(player + 36), "None");
        debugRecall__Fi(0);
        if (!only_message("ErrorNoRecallTarget", -1)) { std::printf("hub %s\n", h); return 4; }
        reset(); std::strcpy(levelName + 81, h); debugRecall__Fi(0);       // target set: recall proceeds, no potion needed
        if (recallOn != 60 || removed_count || msg_count || lump_a != "fx.lmp" || lump_b != "spel_gatescroll.anm") { std::printf("hub2 %s\n", h); return 5; }
    }
    reset(); std::strcpy(levelName + 81, "kelethinx"); make_entity(0); debugRecall__Fi(0);   // near miss is not a hub
    if (!only_message("ErrorNoRecallCharges", 0)) return 6;
    // 4 potions
    reset(); make_entity(1); store_word(player + 23984 + 14168, 3);
    put_item(1, 0, other_name, 4); put_item(1, 1, potion_name, 2); put_item(1, 2, potion_name, 5);
    debugRecall__Fi(1);
    {
        unsigned char* e = player + 23984 + 9552;
        if (e[72 + 12] != 1 || e[144 + 12] != 5 || e[12] != 4 || removed_count) return 7;
        if (recallOn != 60) return 8;
    }
    reset(); make_entity(2); store_word(player + 2 * 23984 + 14168, 2);
    put_item(2, 0, other_name, 4); put_item(2, 1, potion_name, 1);
    debugRecall__Fi(2);
    if (removed_count != 1 || removed_player != 2 || removed_entry != 1 || removed_a != 2 || removed_b != 0) return 9;
    if (player[2 * 23984 + 9552 + 72 + 12] != 0 || recallOn != 60) return 10;
    reset(); make_entity(0); store_word(player + 14168, 2);                       // no potion
    put_item(0, 0, other_name, 4); put_item(0, 1, other_name, 1);
    debugRecall__Fi(0);
    if (!only_message("ErrorNoRecallCharges", 0)) return 11;
    reset(); make_entity(3); store_word(player + 3 * 23984 + 14168, 1); put_item(3, 0, potion_name, 1);
    store_float(player + 3 * 23984 + 172, 0.5f);                                  // alive, with potion at last index
    debugRecall__Fi(3);
    if (removed_count != 1 || removed_entry != 0 || recallOn != 60) return 12;
    reset(); store_word(player + 14168, 1); put_item(0, 0, potion_name, 3);        // no entity word: nothing searched
    debugRecall__Fi(0);
    if (!only_message("ErrorNoRecallCharges", 0) || player[9552 + 12] != 3) return 13;
    reset(); make_entity(0); put_item(0, 0, potion_name, 3);                       // count zero: nothing searched
    debugRecall__Fi(0);
    if (!only_message("ErrorNoRecallCharges", 0) || player[9552 + 12] != 3) return 14;
    // 5 the recall itself
    reset(); std::strcpy(levelName + 81, "airhub");
    for (int i = 0; i < 4; ++i) make_entity(i);
    nmPlayersAlive = 3; playersAlive[0] = 2; playersAlive[1] = 0; playersAlive[2] = 3; alloc_fail_at = 1; status = 0x40;
    debugRecall__Fi(1);
    if (alloc_calls != 3 || ctor_calls != 2) return 15;
    if (ctor_self[0] != block[0] || ctor_self[1] != block[2]) return 16;
    if (ctor_pos[0].x != 3.5f || ctor_pos[0].y != 4.5f || ctor_pos[0].z != 5.5f) return 17;
    if (ctor_pos[1].x != 4.5f || ctor_pos[1].y != 5.5f || ctor_pos[1].z != 6.5f) return 18;
    if (seq_gain != 1.0f || seq_obj != nullptr || seq_header != &lump_a || lump_a != "fx.lmp" || lump_b != "spel_gatescroll.anm") return 19;
    if (fades != 1 || gamePause != 0 || gameControlPaused != 60 || particlesPaused != 0 || recallOn != 60) return 20;
    if (events.size() != 2 || events[0] != "clear" || events[1] != "play") return 21;
    reset(); std::strcpy(levelName + 81, "airhub"); status = 0x3f; nmPlayersAlive = 0; debugRecall__Fi(0);
    if (fades != 0 || ctor_calls != 0 || alloc_calls != 0 || recallOn != 60 || gameControlPaused != 60) return 22;
    (void)fail;
    std::puts("debugRecall__Fi check passed");
    return 0;
}
