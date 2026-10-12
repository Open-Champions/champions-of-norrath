// Host check for shopMsgCallback__FPUci with stubbed callees.
#define NORRATH_INVENTORY_HOST 1
#define norrath_factory_strstr host_strstr
#include "shop_msg_callback.cpp"
#undef norrath_factory_strstr
#include <cstdint>
#include <cstring>

extern "C" unsigned char player[23984 * 2] = {};
extern "C" char* host_strstr(const char* haystack, const char* needle) { return const_cast<char*>(std::strstr(haystack, needle)); }

static int logged = -99, bought_player = -99, value_calls, drops;
static void* bought_item;
static int drop_args[7];
static int value_to_return;
static void* value_item;
extern "C" {
void playerLogInventory__Fi(int p) { logged = p; }
void shopBuy__FcPQ24item11description(char p, void* item) { bought_player = p; bought_item = item; }
int getValue__4itemPQ24item11description(void* item) { ++value_calls; value_item = item; return value_to_return; }
void inventoryGridDropItem__Fiiciiii(int a, int b, char c, int d, int e, int f, int g) {
    ++drops;
    drop_args[0] = a; drop_args[1] = b; drop_args[2] = c; drop_args[3] = d; drop_args[4] = e; drop_args[5] = f; drop_args[6] = g;
}
}

using namespace inventory_items;
static const void* table[32];
static unsigned put(const void* object) {
    for (unsigned i = 0; i < 32; ++i) if (table[i] == nullptr) { table[i] = object; return i + 1; }
    return 0;
}
namespace inventory_items { const void* host_resolve(unsigned word) { return table[word - 1]; } }

static Description descs[4];
static char names[4][16] = {"dagger", "a spear", "mace", "spears"};
static void word(unsigned char* at, unsigned value) { std::memcpy(at, &value, 4); }
static int gold(int owner) { int v; std::memcpy(&v, player + owner * 23984 + 232, 4); return v; }

static void set_slot(int owner, int list, int slot, int desc, int type, int quantity) {
    descs[desc].name = put(names[desc]);
    descs[desc].type = static_cast<signed char>(type);
    Slot* s = &player_lists(owner)[list].slots[slot];
    s->description = put(&descs[desc]);
    s->quantity = static_cast<short>(quantity);
}
static unsigned char packet[32];
static void sell(int owner, int list, int slot) {
    std::memset(packet, 0, sizeof packet);
    packet[0] = 1; packet[1] = static_cast<unsigned char>(owner);
    short h2 = -4, h8 = 12;
    std::memcpy(packet + 2, &h2, 2);
    std::memcpy(packet + 4, &slot, 4);
    std::memcpy(packet + 8, &h8, 2);
    packet[10] = static_cast<unsigned char>(list); packet[11] = 0xfe; packet[12] = 5;
    logged = -99; drops = 0; value_calls = 0;
    shopMsgCallback__FPUci(packet, 1234);
}
#define CHECK(c, code) do { if (!(c)) return code; } while (0)

int main() {
    // Type 0 buys: log, then shopBuy with the player byte and the item word.
    std::memset(packet, 0, sizeof packet);
    packet[0] = 0; packet[1] = 1;
    word(packet + 4, 0x5000);
    shopMsgCallback__FPUci(packet, 8);
    CHECK(logged == 1 && bought_player == 1 && reinterpret_cast<std::uintptr_t>(bought_item) == 0x5000 && drops == 0, 1);

    // Type 1 sells. 100 / 2 * 0.19999998f truncates to 9 (50 * 0.2 would give 10).
    set_slot(0, 2, 3, 0, 1, 2);
    value_to_return = 100;
    sell(0, 2, 3);
    CHECK(logged == 0 && value_calls == 1 && value_item == &player_lists(0)[2].slots[3], 2);
    CHECK(gold(0) == 9, 3);
    CHECK(drops == 1 && drop_args[0] == 0 && drop_args[1] == 3 && drop_args[2] == 2 && drop_args[3] == -4 && drop_args[4] == 12 && drop_args[5] == -2 && drop_args[6] == 5, 4);

    // Type 8 and a name containing "spear" multiply the value by 20.
    set_slot(0, 2, 3, 2, 8, 4);          // mace-type 8: 100*20/4*0.2 = 99.99 -> 99
    sell(0, 2, 3);
    CHECK(gold(0) == 9 + 99, 5);
    set_slot(0, 2, 3, 1, 1, 5);          // "a spear": 100*20/5*0.2 -> 79
    sell(0, 2, 3);
    CHECK(gold(0) == 9 + 99 + 79, 6);
    set_slot(0, 2, 3, 3, 3, 10);         // "spears" also matches: 100*20/10*0.2 -> 39
    sell(0, 2, 3);
    CHECK(gold(0) == 9 + 99 + 79 + 39, 7);

    // A non-positive result becomes 1.
    set_slot(0, 0, 0, 0, 1, 1);
    value_to_return = 0;
    sell(0, 0, 0);
    CHECK(gold(0) == 9 + 99 + 79 + 39 + 1, 8);
    value_to_return = -50;
    sell(0, 0, 0);
    CHECK(gold(0) == 9 + 99 + 79 + 39 + 2, 9);

    // Second player record; other types do nothing.
    set_slot(1, 1, 0, 0, 1, 1);
    value_to_return = 1000;
    sell(1, 1, 0);
    CHECK(gold(1) == 199 && gold(0) == 9 + 99 + 79 + 39 + 2, 10);
    std::memset(packet, 0, sizeof packet);
    packet[0] = 2;
    logged = -99; drops = 0;
    shopMsgCallback__FPUci(packet, 8);
    packet[0] = 0xff;
    shopMsgCallback__FPUci(packet, 8);
    CHECK(logged == -99 && drops == 0, 11);
    return 0;
}
