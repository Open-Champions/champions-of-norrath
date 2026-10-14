// Observed shopStart__Fiii at 0x1b3b20, 2944 bytes. Arguments: player, a stock
// flag, and a kind number (stored at sp+80 in the image).
// First, a zero stock flag with netPlayerIsLocal(player) non-zero sets interfaceOn.
// A non-zero stock flag then builds the three shop lists (shopList, 4616-byte
// lists as in record.hpp) and returns; a zero flag opens the shop screen.
//
// Stock, with d = gameDifficulty and k = kind. lumpLoad("shop.lmp") runs first.
//   floats: f22 = (float)(d-1) * 7.0f, f21 = ((float)k + (float)k) * (float)d,
//   f20 = f22 * 8000.0f. Each random draw takes the seed word syncRandState,
//   steps it twice (seed = seed*1664525 + 1013904223), stores the second result,
//   and uses r1 = first >> 16, r2 = second >> 16 (unsigned).
//   List 0: count = 0, then item::create of "arrows", "firepotion",
//   "explodingpotion", "gaspotion" into slots 0..3 (arguments -1 and 0), then
//   random items: until 22 are accepted, createRandom(slot[count], value, 0, -1,
//   24d + 4k - 24) with value = (int)((float)(r1%150 + n*(r2%200 + 50) + 25) *
//   f21 + f20), n the number accepted so far. A zero return retries. Each existing
//   slot below the count is compared with compareToItem (see below); any non-zero
//   result is a duplicate and, while fewer than 6 duplicates have followed one
//   another, retries. Otherwise the item is rejected, with no change, when bit 40
//   of the 64-bit value at slot+8 (bit 0 of the byte at slot+13) is set, and
//   accepted (count+1, n+1, duplicate run reset) when clear.
//   List 1: same, 17 items, values r1%100 + n*(r2%300 + 214) + 50, createRandom
//   arguments 1 and -1, no bit test.
//   List 2: count = 0, then the potions for kind (1: group 0; 2, 3: group 1; 4, 5:
//   group 2; others none) - two item::create calls, the health name then the
//   rejuvenation name, at index 11 - 3d - group in the 9-entry tables - then
//   "recallpotion"; then 10 attempts (each attempt counts whether or not it
//   succeeds) of createRandom with values r1%200 + n*(r2%500 + 700) + 250,
//   arguments 2 and 1, last argument 24d + 4k - 18. A duplicate retries on the same
//   terms; an item whose byte at slot+33 is 255 is dropped.
//   The compare walks 72-byte records from shopList + 9232*L (L = 0, 1, 2 for
//   lists 0, 1, 2), not from the list being built; for L > 0 that is a different
//   region. This is the observed addressing.
//   Last, list 0 is insertion-sorted ascending by getValue.
// Open: gamePause = 1 when netNetworkMode is 0; otherwise when playerNetDead is
// non-zero the camera is popped, interfaceOn cleared, and the function returns.
// Then msgHudSetPlayerBusy(local player 0, 2), hudClearInfoMessage, the fields
// startFrame = drawFrameNm, animStatePtr = animState, shopIntro = 1,
// gameNoDraw = 1, shopRunning = 1, floorDrawEnable = 0, objectDrawEnable = 0,
// waitingForStreamSpool = 0; Camera Push when the "pauseMenu" task is active, else
// SetMode(0); engineSetBlur(128); coins and shopTick resources from "shop.lmp";
// two 116-byte records at shop are initialized and engineAddTask(shopFrame, 50,
// "shop", 1) is called.
// Unknown: record meanings; the mapping of d to a table index outside 1..3.

#include "../inventory-items-record/record.hpp"
#include "../inventory-items-object/object.hpp"

extern "C" {
extern unsigned syncRandState;
extern int gameDifficulty;
extern signed char netNetworkMode;
extern int gamePause;
extern int gameNoDraw;
extern int shopRunning;
extern signed char interfaceOn;
extern int drawFrameNm;
extern signed char floorDrawEnable;
extern signed char objectDrawEnable;
extern void* gameCam;
extern int shopDisableMouse[2];
extern int shopMouseDecay[2];
extern int waitingForStreamSpool;
extern int startFrame;
extern int shopIntro;
extern void* animStatePtr;
extern void* coins;
extern void* shopTick;
extern unsigned char animState[];
extern unsigned char shopList[];
extern unsigned char shop[];
extern unsigned char inventory[];
int netPlayerIsLocal__Fi(int player);
void lumpLoad__FPc(const char* name);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
void create__4itemPcPQ24item11descriptionii(const char* name, void* out, int first, int second);
int createRandom__4itemPQ24item11descriptioniiii(void* out, int value, int first, int second, int third);
int compareToItem__Q24item11descriptionPQ24item11description(const void* left, const void* right);
int getValue__4itemPQ24item11description(const void* description);
int playerNetDead__Fv();
void Pop__6Camera(void* camera);
void Push__6Camera(void* camera);
void SetMode__6Camerai(void* camera, int mode);
int netGetLocalPlayerNm__Fi(int index);
void msgHudSetPlayerBusy__Fcc(char player, char state);
void hudClearInfoMessage__Fv();
int engineTaskActive__FPc(const char* name);
void engineSetBlur__Fi(int amount);
void engineAddTask__FPFv_viPci(void (*task)(), int priority, const char* name, int flag);
void shopFrame__Fv();
}

namespace {
using inventory_items::List;
using inventory_items::Slot;

const char* const health_potion[9] = {"fullhealpotion", "gianthealpotion", "exhealingpotion",
                                      "greathealpotion", "largehealpotion", "healingpotion",
                                      "minorhealpotion", "smallhealpotion", "leshealingpotion"};
const char* const rejuv_potion[9] = {"fullrejuvpotion", "giantrejuvpotion", "exrejuvpotion",
                                     "greatrejuvpotion", "largerejuvpotion", "rejuvpotion",
                                     "minorrejuvpotion", "smallrejuvpotion", "lesrejuvpotion"};

unsigned step(unsigned seed) { return seed * 1664525u + 1013904223u; }

enum Reject { RejectNone, RejectFlagBit, RejectByte255 };

struct Stage {
    unsigned first_mod, second_mod;
    int second_add, base_add;
    int first_argument, second_argument, last_adjust;
    int total;
    bool every_attempt_counts;
    Reject reject;
};

const Stage first_stage = {150, 200, 50, 25, 0, -1, -24, 22, false, RejectFlagBit};
const Stage second_stage = {100, 300, 214, 50, 1, -1, -24, 17, false, RejectNone};
const Stage third_stage = {200, 500, 700, 250, 2, 1, -18, 10, true, RejectByte255};

// The candidate is slot[count] of `list`; `compare` is where the duplicate walk starts.
void fill(List* list, const Slot* compare, const Stage& stage, int kind, float f21, float f20) {
    int run = 0;
    for (int n = 0; n < stage.total;) {
        const unsigned first = step(syncRandState);
        const unsigned second = step(first);
        const int mix = static_cast<int>((first >> 16) % stage.first_mod) +
                        n * static_cast<int>((second >> 16) % stage.second_mod + stage.second_add) + stage.base_add;
        const int value = static_cast<int>(static_cast<float>(mix) * f21 + f20);
        syncRandState = second;
        Slot* candidate = &list->slots[list->count];
        const int made = createRandom__4itemPQ24item11descriptioniiii(
            candidate, value, stage.first_argument, stage.second_argument,
            24 * gameDifficulty + 4 * kind + stage.last_adjust);
        bool advance = stage.every_attempt_counts;
        if (made != 0) {
            bool duplicate = false;
            for (int index = 0; index < list->count; ++index) {
                if (compareToItem__Q24item11descriptionPQ24item11description(compare + index, &list->slots[list->count]) != 0) {
                    duplicate = true;
                }
            }
            if (duplicate && run < 6) {
                ++run;
            } else {
                const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&list->slots[list->count]);
                const bool rejected = (stage.reject == RejectFlagBit && (bytes[13] & 1u) != 0u) ||
                                      (stage.reject == RejectByte255 && bytes[33] == 255u);
                if (!rejected) {
                    ++list->count;
                    run = 0;
                    advance = true;
                }
            }
        }
        if (advance) ++n;
    }
}

void add_item(List* list, const char* name) {
    Slot* slot = &list->slots[list->count++];
    create__4itemPcPQ24item11descriptionii(name, slot, -1, 0);
}

void add_potions(List* list, int kind) {
    int group = -1;
    if (kind == 1) group = 0;
    else if (kind == 2 || kind == 3) group = 1;
    else if (kind == 4 || kind == 5) group = 2;
    if (group < 0) return;
    const int index = 11 - 3 * gameDifficulty - group;
    add_item(list, health_potion[index]);
    add_item(list, rejuv_potion[index]);
}

void build_stock(int kind) {
    lumpLoad__FPc("shop.lmp");
    List* lists = reinterpret_cast<List*>(shopList);
    const float f22 = static_cast<float>(gameDifficulty - 1) * 7.0f;
    const float f21 = (static_cast<float>(kind) + static_cast<float>(kind)) * static_cast<float>(gameDifficulty);
    lists[0].count = 0;
    add_item(&lists[0], "arrows");
    add_item(&lists[0], "firepotion");
    add_item(&lists[0], "explodingpotion");
    add_item(&lists[0], "gaspotion");
    const float f20 = f22 * 8000.0f;
    fill(&lists[0], reinterpret_cast<const Slot*>(shopList), first_stage, kind, f21, f20);
    lists[1].count = 0;
    fill(&lists[1], reinterpret_cast<const Slot*>(shopList + 9232), second_stage, kind, f21, f20);
    lists[2].count = 0;
    add_potions(&lists[2], kind);
    add_item(&lists[2], "recallpotion");
    fill(&lists[2], reinterpret_cast<const Slot*>(shopList + 18464), third_stage, kind, f21, f20);

    for (int sorted = 1; sorted < lists[0].count; ++sorted) {
        for (int index = sorted; index > 0; --index) {
            Slot* later = &lists[0].slots[index];
            Slot* earlier = &lists[0].slots[index - 1];
            const int later_value = getValue__4itemPQ24item11description(later);
            const int earlier_value = getValue__4itemPQ24item11description(earlier);
            if (later_value < earlier_value) {
                Slot held = *earlier;
                *earlier = *later;
                *later = held;
            }
        }
    }
}

void put_word(unsigned char* base, unsigned offset, unsigned value) {
    inventory_object::write_word(base + offset, value);
}
void put_float(unsigned char* base, unsigned offset, float value) {
    put_word(base, offset, __builtin_bit_cast(unsigned, value));
}

void open_shop(int player) {
    if (netNetworkMode == 0) {
        gamePause = 1;
    } else if (playerNetDead__Fv() != 0) {
        Pop__6Camera(gameCam);
        interfaceOn = 0;
        return;
    }
    msgHudSetPlayerBusy__Fcc(static_cast<char>(netGetLocalPlayerNm__Fi(0)), 2);
    hudClearInfoMessage__Fv();
    startFrame = drawFrameNm;
    animStatePtr = animState;
    gameNoDraw = 1;
    shopRunning = 1;
    shopIntro = 1;
    floorDrawEnable = 0;
    objectDrawEnable = 0;
    if (engineTaskActive__FPc("pauseMenu") != 0) Push__6Camera(gameCam);
    else SetMode__6Camerai(gameCam, 0);
    engineSetBlur__Fi(128);
    waitingForStreamSpool = 0;
    coins = lumpFindResource__FPcT0("shop.lmp", "coins.vag");
    shopTick = lumpFindResource__FPcT0("shop.lmp", "shoptick.vag");
    for (int index = 0; index < 2; ++index) {
        unsigned char* record = shop + 116 * index;
        put_word(inventory, 80 + 116 * index, 0);
        put_float(record, 72, 200.0f);
        put_word(record, 76, 3);
        put_word(record, 80, 0);
        put_word(record, 8, 320);
        put_word(record, 12, 447);
        put_float(record, 68, static_cast<float>(320 * index) + 100.0f);
        if (index == 0) put_word(shop, 0, 0);
        else put_word(shop + 116, 0, 320);
        put_word(record, 4, 0);
        put_word(record, 96, 0);
        put_word(inventory_items::player_record(index), 23536, 0);
        put_word(record, 84, 0);
        put_word(record, 44, 0);
        put_word(record, 48, 0);
        put_word(record, 112, 0);
        put_float(record, 92, __builtin_bit_cast(float, 0x3e4ccccc));
        put_word(record, 36, 0);
        put_word(record, 40, 0);
        put_word(record, 24, 0);
        put_word(record, 28, 0);
        put_float(record, 60, 0.0f);
        put_float(record, 64, 0.0f);
        put_word(record, 20, 0);
    }
    put_word(shop, 56, static_cast<unsigned>(player));
    put_word(shop, 192, 2);
    put_word(shop, 172, static_cast<unsigned>(-1));
    shopDisableMouse[1] = 0;
    shopMouseDecay[1] = 0;
    put_word(shop, 52, static_cast<unsigned>(player));
    put_word(shop, 168, static_cast<unsigned>(-1));
    shopDisableMouse[0] = 0;
    shopMouseDecay[0] = 0;
    engineAddTask__FPFv_viPci(shopFrame__Fv, 50, "shop", 1);
}
}

extern "C" void shopStart__Fiii(int player, int stock_flag, int kind) {
    if (stock_flag == 0 && netPlayerIsLocal__Fi(player) != 0) interfaceOn = 1;
    if (stock_flag != 0) build_stock(kind);
    else open_shop(player);
}
