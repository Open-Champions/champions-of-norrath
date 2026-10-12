#define NORRATH_INVENTORY_HOST 1
#include <cstring>
#define strstr host_strstr
#include "get_index.cpp"
#undef strstr

extern "C" unsigned char player[23984 * 2] = {};
extern "C" char* host_strstr(const char* haystack, const char* needle) { return const_cast<char*>(std::strstr(haystack, needle)); }

static const void* table[32];
static unsigned put(const void* object) {
    for (unsigned i = 0; i < 32; ++i) {
        if (table[i] == nullptr) { table[i] = object; return i + 1; }
    }
    return 0;
}
namespace inventory_items { const void* host_resolve(unsigned word) { return table[word - 1]; } }

struct Desc { unsigned name; };
static Desc descs[16];
static char names[16][16];
static int used;

static void word(unsigned char* at, unsigned value) { std::memcpy(at, &value, 4); }
static void add(int owner, int list, int slot, const char* name) {
    const int id = used++;
    std::strcpy(names[id], name);
    descs[id].name = put(names[id]);
    unsigned char* base = player + owner * 23984 + 328 + list * 4616;
    word(base + slot * 72, put(&descs[id]));
    int count;
    std::memcpy(&count, base + 4608, 4);
    if (slot + 1 > count) word(base + 4608, slot + 1);
}

static int run(int owner, const char* text, int& slot, int& list) {
    slot = 77;
    list = 88;
    inventoryGetItemIndex__FiPcPiT2(owner, const_cast<char*>(text), &slot, &list);
    return 0;
}

int main() {
    add(0, 0, 0, "sword");
    add(0, 0, 1, "bow");
    add(0, 1, 0, "potion");
    add(0, 2, 0, "bow");
    add(1, 1, 0, "sword");
    int slot, list;
    run(0, "a sword here", slot, list);
    if (slot != 0 || list != 0) return 1;
    run(0, "bow", slot, list);        // last match wins: list 2
    if (slot != 0 || list != 2) return 2;
    run(0, "none", slot, list);       // slot is -1, list untouched
    if (slot != -1 || list != 88) return 3;
    run(1, "sword", slot, list);
    if (slot != 0 || list != 1) return 4;
    run(0, "potionbow", slot, list);
    if (slot != 0 || list != 2) return 5;
    word(player + 328 + 4608, 1);     // list 0 cut to one slot: bow slot hidden
    run(0, "bow", slot, list);
    if (list != 2) return 6;
    word(player + 328 + 2 * 4616 + 4608, 0);
    run(0, "bow", slot, list);
    if (slot != -1 || list != 88) return 7;
    return 0;
}
