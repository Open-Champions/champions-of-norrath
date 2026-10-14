// Host check for queryInventoryItem__FPci. Description and name words are
// 32-bit tokens that host_resolve maps back to host objects.
#define NORRATH_INVENTORY_HOST 1
#include <cstring>
#define strcmp host_strcmp
#include "query.cpp"
#undef strcmp

extern "C" unsigned char player[23984 * 2] = {};
extern "C" int host_strcmp(const char* left, const char* right) { return std::strcmp(left, right); }

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

static void add(int owner, int list, int slot, const char* name, int quantity) {
    const int id = used++;
    std::strcpy(names[id], name);
    descs[id].name = put(names[id]);
    unsigned char* base = player + owner * 23984 + 328 + list * 4616;
    unsigned char* cell = base + slot * 72;
    word(cell, put(&descs[id]));
    short q = static_cast<short>(quantity);
    std::memcpy(cell + 4, &q, 2);
    int count;
    std::memcpy(&count, base + 4608, 4);
    if (slot + 1 > count) word(base + 4608, slot + 1);
}

static int query(const char* name, int owner) { return queryInventoryItem__FPci(const_cast<char*>(name), owner); }

int main() {
    add(0, 0, 0, "sword", 2);
    add(0, 0, 1, "bow", 1);
    add(0, 1, 0, "sword", 4);
    add(0, 2, 0, "sword", -1);
    add(1, 0, 0, "sword", 9);
    if (query("sword", 0) != 5) return 1;
    if (query("bow", 0) != 1) return 2;
    if (query("missing", 0) != 0) return 3;
    if (query("swor", 0) != 0) return 4;
    if (query("sword", 1) != 9) return 5;
    word(player + 328 + 2 * 4616 + 4608, 0);
    if (query("sword", 0) != 6) return 6;
    word(player + 328 + 4608, static_cast<unsigned>(-1));
    if (query("sword", 0) != 4) return 7;
    return 0;
}
