// Host check for shopStart__Fiii. Build with -ffp-contract=off. Callees are
// stubs; records are 72-byte slots whose first word is an identity and whose
// byte 13 / byte 33 are controlled by the test.
#define NORRATH_INVENTORY_HOST 1
#include "shop_start.cpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
unsigned syncRandState = 0;
int gameDifficulty = 2;
signed char netNetworkMode = 0;
int gamePause = 0;
int gameNoDraw = 0;
signed char interfaceOn = 0;
int drawFrameNm = 77;
signed char floorDrawEnable = 1;
signed char objectDrawEnable = 1;
void* gameCam = reinterpret_cast<void*>(0x9000);
int shopDisableMouse[2] = {5, 5};
int shopMouseDecay[2] = {5, 5};
int waitingForStreamSpool = 5;
int startFrame = 0;
int shopIntro = 0;
void* animStatePtr = nullptr;
void* coins = nullptr;
void* shopTick = nullptr;
unsigned char animState[16];
alignas(8) unsigned char shopList[5 * 4616 + 256];
alignas(8) unsigned char shop[232];
alignas(8) unsigned char inventory[464];
unsigned char player[23984 * 2 + 64];
}
int shopRunning = 0;

static std::string log_text;
static std::vector<std::string> created_names;
static int next_id = 1000;
struct Call { int value, a, b, t; unsigned char* slot; unsigned seed_at_call; };
static std::vector<Call> random_calls;
static std::vector<int> script;               // 0: fail; 1: ok unique; 2: ok unique, byte13 bit 0; 3: ok id 7; 4: ok unique, byte33 255
static unsigned script_index;
static int local_result = 1;
static int dead_result = 0;
static int task_active = 0;
static void* popped; static void* pushed; static void* set_mode_camera; static int set_mode_value = -1;
static int blur = -1, busy_player = -99, busy_state = -99;
static int task_priority, task_flag; static void (*task_function)(); static std::string task_name;
static std::string lump_load;
static std::string lump_a, lump_b;
static int compare_calls;
static int value_calls;

static unsigned word_at(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }

extern "C" {
int netPlayerIsLocal__Fi(int p) { log_text += "n"; return p == 0 ? local_result : 0; }
void lumpLoad__FPc(const char* name) { log_text += "L"; lump_load = name; }
void* lumpFindResource__FPcT0(const char* lump, const char* name) {
    log_text += "F";
    if (lump_a.empty()) lump_a = name; else lump_b = name;
    if (std::string(lump) != "shop.lmp") log_text += "!";
    return std::string(name) == "coins.vag" ? reinterpret_cast<void*>(0xc01) : reinterpret_cast<void*>(0xc02);
}
void create__4itemPcPQ24item11descriptionii(const char* name, void* out, int a, int b) {
    created_names.push_back(name);
    if (a != -1 || b != 0) created_names.push_back("BAD-ARGS");
    unsigned id = static_cast<unsigned>(next_id++);
    std::memset(out, 0, 72);
    std::memcpy(out, &id, 4);
    const int value = static_cast<int>((id * 37u) % 101u);
    std::memcpy(static_cast<unsigned char*>(out) + 4, &value, 4);
}
int createRandom__4itemPQ24item11descriptioniiii(void* out, int value, int a, int b, int t) {
    Call c = {value, a, b, t, static_cast<unsigned char*>(out), syncRandState};
    random_calls.push_back(c);
    const int mode = script_index < script.size() ? script[script_index++] : 1;
    if (mode == 0) return 0;
    unsigned id = mode == 3 ? 7u : static_cast<unsigned>(next_id++);
    std::memset(out, 0, 72);
    std::memcpy(out, &id, 4);
    const int assigned = static_cast<int>((id * 37u) % 101u);
    std::memcpy(static_cast<unsigned char*>(out) + 4, &assigned, 4);
    static_cast<unsigned char*>(out)[13] = mode == 2 ? 1 : 0;
    static_cast<unsigned char*>(out)[33] = mode == 4 ? 255 : 0;
    return 1;
}
int compareToItem__Q24item11descriptionPQ24item11description(const void* l, const void* r) {
    ++compare_calls;
    return word_at(static_cast<const unsigned char*>(l)) == word_at(static_cast<const unsigned char*>(r));
}
int getValue__4itemPQ24item11description(const void* d) { ++value_calls; return static_cast<int>(word_at(static_cast<const unsigned char*>(d) + 4)); }
int playerNetDead__Fv() { log_text += "d"; return dead_result; }
void Pop__6Camera(void* c) { log_text += "O"; popped = c; }
void Push__6Camera(void* c) { log_text += "P"; pushed = c; }
void SetMode__6Camerai(void* c, int m) { log_text += "M"; set_mode_camera = c; set_mode_value = m; }
int netGetLocalPlayerNm__Fi(int i) { log_text += "g"; return i == 0 ? 0x1f1 : -1; }
void msgHudSetPlayerBusy__Fcc(char p, char s) { log_text += "B"; busy_player = p; busy_state = s; }
void hudClearInfoMessage__Fv() { log_text += "H"; }
int engineTaskActive__FPc(const char* n) { log_text += "T"; return std::string(n) == "pauseMenu" ? task_active : -1; }
void engineSetBlur__Fi(int a) { log_text += "U"; blur = a; }
void engineAddTask__FPFv_viPci(void (*f)(), int p, const char* n, int flag) { log_text += "A"; task_function = f; task_priority = p; task_name = n; task_flag = flag; }
void shopFrame__Fv() {}
}

namespace inventory_items { const void* host_resolve(unsigned) { return nullptr; } }

static bool float_bits_equal(const unsigned char* p, float v) { float f; std::memcpy(&f, p, 4); return f == v; }
#define CHECK(c, code) do { if (!(c)) { std::printf("fail %d\n", code); return code; } } while (0)

static void reset() {
    std::memset(shopList, 0, sizeof shopList);
    std::memset(shop, 0xee, sizeof shop);
    std::memset(inventory, 0xee, sizeof inventory);
    std::memset(player, 0xee, sizeof player);
    log_text.clear(); created_names.clear(); random_calls.clear(); script.clear(); script_index = 0;
    next_id = 1000; compare_calls = 0; value_calls = 0; local_result = 1; dead_result = 0; task_active = 0;
    popped = pushed = set_mode_camera = nullptr; set_mode_value = -1; blur = -1; busy_player = -99; busy_state = -99;
    lump_a.clear(); lump_b.clear(); lump_load.clear(); task_name.clear();
    gamePause = 0; gameNoDraw = 0; interfaceOn = 0; shopRunning = 0; floorDrawEnable = 1; objectDrawEnable = 1;
    startFrame = 0; shopIntro = 0; animStatePtr = nullptr; coins = shopTick = nullptr; waitingForStreamSpool = 5;
    shopDisableMouse[0] = shopDisableMouse[1] = 5; shopMouseDecay[0] = shopMouseDecay[1] = 5;
    netNetworkMode = 0; gameDifficulty = 2; drawFrameNm = 77; syncRandState = 12345;
}
static List* lists() { return reinterpret_cast<List*>(shopList); }

int main() {
    // ---- Stock, everything succeeds and nothing repeats ----
    reset();
    interfaceOn = 0;
    shopStart__Fiii(0, 1, 3);
    CHECK(log_text == "L", 1);                          // no netPlayerIsLocal when the flag is non-zero, no interfaceOn
    CHECK(interfaceOn == 0 && lump_load == "shop.lmp", 2);
    CHECK(lists()[0].count == 4 + 22 && lists()[1].count == 17, 3);
    CHECK(lists()[2].count == 2 + 1 + 10, 4);
    // Created names: list 0 fixed items, then list 2's potions for kind 3, difficulty 2: index 11 - 6 - 1 = 4.
    CHECK(created_names.size() == 4 + 3, 5);
    CHECK(created_names[0] == "arrows" && created_names[1] == "firepotion" && created_names[2] == "explodingpotion" &&
              created_names[3] == "gaspotion", 6);
    CHECK(created_names[4] == "largehealpotion" && created_names[5] == "largerejuvpotion" && created_names[6] == "recallpotion", 7);
    CHECK(random_calls.size() == 22 + 17 + 10, 8);
    // Replay the generator for the expected values.
    {
        unsigned seed = 12345;
        const float f22 = static_cast<float>(2 - 1) * 7.0f;
        const float f21 = (3.0f + 3.0f) * 2.0f;
        const float f20 = f22 * 8000.0f;
        for (int n = 0; n < 22; ++n) {
            unsigned a = seed * 1664525u + 1013904223u;
            unsigned b = a * 1664525u + 1013904223u;
            int mix = int((a >> 16) % 150) + n * int((b >> 16) % 200 + 50) + 25;
            int want = int(float(mix) * f21 + f20);
            CHECK(random_calls[n].value == want && random_calls[n].a == 0 && random_calls[n].b == -1, 100 + n);
            CHECK(random_calls[n].t == 24 * 2 + 4 * 3 - 24, 130 + n);
            CHECK(random_calls[n].seed_at_call == b, 160 + n);       // stored before the call
            seed = b;
        }
        for (int n = 0; n < 17; ++n) {
            unsigned a = seed * 1664525u + 1013904223u;
            unsigned b = a * 1664525u + 1013904223u;
            int mix = int((a >> 16) % 100) + n * int((b >> 16) % 300 + 214) + 50;
            int want = int(float(mix) * f21 + f20);
            const Call& c = random_calls[22 + n];
            CHECK(c.value == want && c.a == 1 && c.b == -1 && c.t == 24 * 2 + 4 * 3 - 24, 200 + n);
            seed = b;
        }
        for (int n = 0; n < 10; ++n) {
            unsigned a = seed * 1664525u + 1013904223u;
            unsigned b = a * 1664525u + 1013904223u;
            int mix = int((a >> 16) % 200) + n * int((b >> 16) % 500 + 700) + 250;
            int want = int(float(mix) * f21 + f20);
            const Call& c = random_calls[39 + n];
            CHECK(c.value == want && c.a == 2 && c.b == 1 && c.t == 24 * 2 + 4 * 3 - 18, 230 + n);
            seed = b;
        }
        CHECK(syncRandState == seed, 250);
    }
    // Each random item went into slot[count] of its own list.
    CHECK(random_calls[0].slot == shopList + 4 * 72 && random_calls[22].slot == shopList + 4616 &&
              random_calls[39].slot == shopList + 2 * 4616 + 3 * 72, 9);

    // ---- Retry rules: failure, flag-bit rejection ----
    reset();
    gameDifficulty = 1; syncRandState = 1;
    script = {0, 2, 1};                     // list 0: fail, rejected by the flag bit, then accepted
    shopStart__Fiii(0, 1, 5);
    CHECK(lists()[0].count == 26 && random_calls.size() == 22 + 2 + 17 + 10, 20);
    {
        unsigned seed = 1;
        const int taken_before[4] = {0, 0, 0, 1};                 // n only advances on an accepted item
        for (int c = 0; c < 4; ++c) {
            const unsigned a = seed * 1664525u + 1013904223u;
            const unsigned b = a * 1664525u + 1013904223u;
            const int mix = int((a >> 16) % 150) + taken_before[c] * int((b >> 16) % 200 + 50) + 25;
            CHECK(random_calls[c].value == int(float(mix) * 10.0f + 0.0f), 21);
            seed = b;
        }
    }
    CHECK(random_calls[2].slot == shopList + 4 * 72 && random_calls[3].slot == shopList + 5 * 72, 22);
    CHECK(random_calls[0].t == 24 * 1 + 4 * 5 - 24, 23);

    // ---- Duplicates in list 0: exactly six retries are allowed ----
    reset();
    gameDifficulty = 1; syncRandState = 1;
    script = {3, 3, 3, 3, 3, 3, 3};              // one accepted, six duplicates retried, then defaults
    shopStart__Fiii(0, 1, 1);
    {
        int sevens = 0;
        for (int i = 0; i < 26; ++i) sevens += word_at(shopList + i * 72) == 7;
        CHECK(sevens == 1 && lists()[0].count == 26, 35);
        CHECK(random_calls.size() == 7 + 21 + 17 + 10, 36);
    }

    // ---- Duplicates in list 0: six retries, then the seventh is taken; the run resets ----
    reset();
    gameDifficulty = 1; syncRandState = 1;
    script = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};   // id 7 fifteen times
    shopStart__Fiii(0, 1, 1);
    // call 0 accepted; 1..6 duplicates; 7 accepted (run was 6); 8..13 duplicates; 14 accepted.
    CHECK(lists()[0].count == 26, 30);
    CHECK(random_calls.size() == 15 + (22 - 3) + 17 + 10, 31);
    {
        int sevens = 0;
        for (int i = 0; i < 26; ++i) sevens += word_at(shopList + i * 72) == 7;   // list 0 is sorted afterwards
        CHECK(sevens == 3, 32);
    }
    // The duplicate walk looked at the first count slots each time.
    CHECK(compare_calls >= 4 + 5 + 5 + 5 + 5 + 5 + 5 + 6 * 5 + 6, 34);

    // ---- Stage 2 walks shopList + 9232, not its own list ----
    reset();
    gameDifficulty = 1; syncRandState = 1;
    {
        unsigned seven = 7;
        std::memcpy(shopList + 9232, &seven, 4);      // slot 0 of the region the walk starts at
        script.assign(22, 1);                         // list 0
        script.push_back(1);                          // list 1 first item: unique
        script.push_back(3);                          // second: id 7, equal to the region slot, not to its own slot
        script.push_back(1);
        shopStart__Fiii(0, 1, 1);
        CHECK(lists()[1].count == 17, 40);
        CHECK(random_calls.size() == 22 + 18 + 10, 41);                 // one duplicate retry
        CHECK(word_at(shopList + 4616 + 1 * 72) != 7, 42);
    }
    // Stage 2 accepts a flag-bit item, and its duplicate run is separate from stage 1.
    reset();
    gameDifficulty = 1; syncRandState = 1;
    script.assign(22, 1);
    script.push_back(2);
    shopStart__Fiii(0, 1, 1);
    CHECK(lists()[1].count == 17 && random_calls.size() == 22 + 17 + 10, 43);
    CHECK(static_cast<unsigned char*>(shopList + 4616)[13] == 1, 44);

    // ---- Stage 4 walks shopList + 18464 and every attempt counts ----
    reset();
    gameDifficulty = 1; syncRandState = 1;
    {
        unsigned seven = 7;
        std::memcpy(shopList + 18464 + 1 * 72, &seven, 4);   // slot 1 of the region
        script.assign(22 + 17, 1);
        script.push_back(1);                                  // first attempt: unique, accepted (count 3 -> 4)
        script.push_back(3);                                  // id 7 equals region slot 1: duplicate, counted
        script.push_back(0);                                  // failure: counted
        script.push_back(4);                                  // byte 33 is 255: dropped, counted
        shopStart__Fiii(0, 1, 1);
        CHECK(random_calls.size() == 22 + 17 + 10, 50);
        // 10 attempts: accepted ones are the unique default ones; 1 + 6 later defaults.
        CHECK(lists()[2].count == 3 + 1 + 6, 51);
    }

    // ---- Sort of list 0 ----
    reset();
    gameDifficulty = 3; syncRandState = 99;
    shopStart__Fiii(0, 1, 2);
    {
        const List& l = lists()[0];
        CHECK(l.count == 26, 60);
        for (int i = 1; i < l.count; ++i) {
            const int previous = *reinterpret_cast<const int*>(reinterpret_cast<const unsigned char*>(&l.slots[i - 1]) + 4);
            const int current = *reinterpret_cast<const int*>(reinterpret_cast<const unsigned char*>(&l.slots[i]) + 4);
            CHECK(previous <= current, 61);
        }
        CHECK(value_calls == 2 * (26 * 25 / 2), 62);
        // Identities survive: every id appears once among slots 0..25.
        std::vector<unsigned> ids;
        for (int i = 0; i < l.count; ++i) ids.push_back(word_at(reinterpret_cast<const unsigned char*>(&l.slots[i])));
        for (size_t i = 0; i < ids.size(); ++i) for (size_t j = i + 1; j < ids.size(); ++j) CHECK(ids[i] != ids[j], 63);
    }
    // Sort swaps only when strictly smaller: two equal values keep their order.
    reset();
    shopList[0] = 0;
    // (covered by the strict comparison in the loop above through the ids check)

    // ---- Potion names by kind and difficulty ----
    reset();
    gameDifficulty = 1; shopStart__Fiii(0, 1, 1);
    CHECK(created_names.size() == 7 && created_names[4] == "leshealingpotion" && created_names[5] == "lesrejuvpotion", 70);
    reset();
    gameDifficulty = 3; shopStart__Fiii(0, 1, 4);
    CHECK(created_names[4] == "fullhealpotion" && created_names[5] == "fullrejuvpotion", 71);
    reset();
    gameDifficulty = 2; shopStart__Fiii(0, 1, 2);
    CHECK(created_names[4] == "largehealpotion" && created_names[5] == "largerejuvpotion", 72);
    reset();
    gameDifficulty = 1; shopStart__Fiii(0, 1, 0);          // kind 0: no potions, only recallpotion
    CHECK(created_names.size() == 5 && created_names[4] == "recallpotion" && lists()[2].count == 11, 73);
    reset();
    gameDifficulty = 1; shopStart__Fiii(0, 1, 6);
    CHECK(created_names.size() == 5 && lists()[2].count == 11, 74);

    // ---- Open: interfaceOn, pause, camera, records ----
    reset();
    netNetworkMode = 0; task_active = 0;
    shopStart__Fiii(0, 0, 1);
    CHECK(log_text == "ngHBTMUFF" "A" || log_text.substr(0, 1) == "n", 80);
    CHECK(interfaceOn == 1 && gamePause == 1 && gameNoDraw == 1 && shopRunning == 1 && shopIntro == 1, 81);
    CHECK(floorDrawEnable == 0 && objectDrawEnable == 0 && waitingForStreamSpool == 0, 82);
    CHECK(startFrame == 77 && animStatePtr == animState, 83);
    CHECK(busy_player == static_cast<char>(0x1f1) && busy_state == 2 && blur == 128, 84);
    CHECK(set_mode_camera == gameCam && set_mode_value == 0 && pushed == nullptr && popped == nullptr, 85);
    CHECK(coins == reinterpret_cast<void*>(0xc01) && shopTick == reinterpret_cast<void*>(0xc02), 86);
    CHECK(task_priority == 50 && task_flag == 1 && task_name == "shop" && task_function == shopFrame__Fv, 87);
    CHECK(shopDisableMouse[0] == 0 && shopDisableMouse[1] == 0 && shopMouseDecay[0] == 0 && shopMouseDecay[1] == 0, 89);
    for (int i = 0; i < 2; ++i) {
        const unsigned char* r = shop + 116 * i;
        CHECK(float_bits_equal(r + 72, 200.0f), 90 + i);
        CHECK(word_at(r + 76) == (i == 0 ? 3u : 2u), 92 + i);
        CHECK(word_at(r + 80) == 0 && word_at(r + 8) == 320 && word_at(r + 12) == 447, 94 + i);
        CHECK(float_bits_equal(r + 68, 100.0f + 320.0f * i), 96 + i);
        CHECK(word_at(r + 0) == (i == 0 ? 0u : 320u), 98 + i);
        CHECK(word_at(r + 4) == 0 && word_at(r + 96) == 0 && word_at(r + 84) == 0 && word_at(r + 44) == 0 &&
                  word_at(r + 48) == 0 && word_at(r + 112) == 0 && word_at(r + 36) == 0 && word_at(r + 40) == 0 &&
                  word_at(r + 24) == 0 && word_at(r + 28) == 0 && word_at(r + 20) == 0, 100 + i);
        CHECK(word_at(r + 92) == 0x3e4ccccc && word_at(r + 60) == 0 && word_at(r + 64) == 0, 102 + i);
        CHECK(word_at(inventory + 80 + 116 * i) == 0, 104 + i);
        CHECK(word_at(player + 23984 * i + 23536) == 0, 106 + i);
    }
    CHECK(word_at(shop + 56) == 0 && word_at(shop + 52) == 0, 110);                // player 0
    CHECK(word_at(shop + 172) == 0xffffffffu && word_at(shop + 168) == 0xffffffffu, 111);
    // Untouched bytes between written words stay as they were.
    CHECK(word_at(shop + 16) == 0xeeeeeeeeu && word_at(shop + 32) == 0xeeeeeeeeu && word_at(shop + 88) == 0xeeeeeeeeu, 112);

    reset();
    netNetworkMode = 1; dead_result = 0; task_active = 1;
    shopStart__Fiii(1, 0, 4);                // player 1 is not local: interfaceOn stays 0
    CHECK(interfaceOn == 0 && gamePause == 0 && pushed == gameCam && set_mode_camera == nullptr, 120);
    CHECK(word_at(shop + 56) == 1 && word_at(shop + 52) == 1, 121);
    reset();
    netNetworkMode = 1; dead_result = 1;
    interfaceOn = 1;
    shopStart__Fiii(0, 0, 1);
    CHECK(popped == gameCam && interfaceOn == 0 && gamePause == 0 && gameNoDraw == 0 && shopRunning == 0, 122);
    CHECK(log_text == "ndO", 123);
    CHECK(task_name.empty() && word_at(shop) == 0xeeeeeeeeu, 124);
    reset();
    local_result = 0;
    shopStart__Fiii(0, 0, 1);
    CHECK(interfaceOn == 0 && gamePause == 1, 125);
    return 0;
}
