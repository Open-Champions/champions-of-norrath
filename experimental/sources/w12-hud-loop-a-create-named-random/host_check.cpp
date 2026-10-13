// Host check for createNamedRandom__4itemPQ24item11descriptioniii. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -fno-builtin host_check.cpp
// The expectations are derived by a small reference model written from the observed rules (see the source
// header), not by calling the function: eligibility, the draw sequence, the "k decreases per eligible row,
// stop at zero" choice, the keep rule and the final roll rewrite. libc strcpy, strstr and powf are used as is.
#define NORRATH_HOST_CONTRACT
#include "create_named_random.cpp"

#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>

static const unsigned kBase = 0x31fbe0;
extern "C" {
unsigned char _4item$namedItemTable[0x6000];
int nmPlayers;
unsigned syncRandState;
alignas(8) unsigned char player[23984 * 4];
unsigned char* norrath_cnr_pointer(unsigned word) { return _4item$namedItemTable + (word - kBase); }
}

static std::map<std::string, int> valueOf;
static std::vector<std::string> created;
static std::string currentName;
static int getValueCalls;
static unsigned rollRowAddress;

// Attribute ids present in the template after create, per row name (-1 = free slot).
static const int* slotIds(const std::string& name) {
    static const int plain[4] = {7, -1, -1, -1};
    static const int has14[4] = {7, 14, 6, -1};
    static const int full3[4] = {7, 6, 8, -1};
    static const int full4[4] = {7, 6, 8, 9};
    if (name == "axe") return has14;
    if (name == "cloak") return full3;
    if (name == "gem") return full4;
    return plain;
}

// Index of the slot the function should fill, or -1.
static int expectedSlot(const std::string& name) {
    const int* ids = slotIds(name);
    for (int i = 0; i < 4; ++i)
        if (ids[i] == -1 || ids[i] == 14) return i;
    return -1;
}

extern "C" {
void create__4itemPcPQ24item11descriptionii(char* name, void* description, int a, int b) {
    created.push_back(name);
    currentName = name;
    unsigned char* item = static_cast<unsigned char*>(description);
    bool bad = a != -1 || b != 1;
    for (int i = 0; i < 4; ++i) bad = bad || item[37 + 8 * i] != 0xff || item[36 + 8 * i] != 0xff;
    bad = bad || item[32] != 0xff || item[33] != 0xff;
    for (int i = 0; i < 4; ++i) bad = bad || item[16 + 2 * i] || item[17 + 2 * i] || item[24 + 2 * i] || item[25 + 2 * i];
    if (bad) created.push_back("BAD-TEMPLATE");
    std::memset(item, 0x5a, 72);
    std::memcpy(item, &rollRowAddress, 4);
    const unsigned long long second = 0xfedcba9876543210ull;
    std::memcpy(item + 8, &second, 8);
    for (int i = 0; i < 4; ++i) {
        const int id = slotIds(currentName)[i];
        item[36 + 8 * i] = static_cast<unsigned char>(id);
        item[37 + 8 * i] = static_cast<unsigned char>(id >> 8);
    }
    item[32] = 0xff; item[33] = 0xff;
}
int getValue__4itemPQ24item11description(const void*) {
    ++getValueCalls;
    return valueOf[currentName];
}
}

static void put_u32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void put_u64(unsigned char* p, unsigned long long v) { std::memcpy(p, &v, 8); }
static unsigned text_cursor;
static unsigned put_text(const std::string& text) {
    unsigned address = kBase + text_cursor;
    std::memcpy(_4item$namedItemTable + text_cursor, text.c_str(), text.size() + 1);
    text_cursor += static_cast<unsigned>(text.size()) + 1;
    return address;
}

struct Row {
    std::string name;
    int minimum;
    int category;
    unsigned long long flags;
    int value;
};

static std::vector<Row> rows;

static void build() {
    std::memset(_4item$namedItemTable, 0, sizeof _4item$namedItemTable);
    text_cursor = 0x5000;
    valueOf.clear();
    created.clear();
    getValueCalls = 0;
    for (size_t i = 0; i < rows.size(); ++i) {
        unsigned char* row = _4item$namedItemTable + 64 * i;
        put_u32(row, put_text(rows[i].name));
        row[4] = static_cast<unsigned char>(rows[i].minimum);
        row[5] = static_cast<unsigned char>(rows[i].minimum >> 8);
        row[29] = static_cast<unsigned char>(rows[i].category);
        put_u64(row + 40, rows[i].flags);
        valueOf[rows[i].name] = rows[i].value;
    }
    rollRowAddress = kBase;      // row 0 is the row named by the produced description
}

static unsigned draw_ref(unsigned& state) {
    state = state * 1664525u + 1013904223u;
    return state >> 16;
}

static const unsigned long long kFlag35 = 1ull << 35;

// Reference model: returns the expected result and fills the expected created-name list and out bytes.
struct Expected {
    int result;
    std::vector<std::string> names;
    int chosenIndexForOut;
    unsigned finalState;
    bool potionless;
};

static bool ref_eligible(const Row& row, int category, int level, bool flagA) {
    if (row.flags & 0x10000ull) return false;
    if (category != -1 && row.category != category) return false;
    if (level < row.minimum) return false;
    if (row.flags & kFlag35) return false;
    if (flagA && row.category == 1 && (row.flags & 0x80000ull)) return false;
    return true;
}

static Expected reference(unsigned state, int wizards, int players, int limit, int category, int level) {
    Expected e = {0, {}, -1, 0, false};
    bool flagA = false;
    if (category == 1) {
        unsigned r = draw_ref(state);
        float fraction = static_cast<float>(wizards) / static_cast<float>(players);
        flagA = static_cast<int>(fraction * 65536.0f) < static_cast<int>(r % 65535u);
    }
    std::vector<int> ok;
    for (size_t i = 0; i < rows.size(); ++i)
        if (ref_eligible(rows[i], category, level, flagA)) ok.push_back(static_cast<int>(i));
    if (ok.empty()) { e.finalState = state; return e; }
    int best = -1;
    for (int round = 0; round < 10; ++round) {
        unsigned k = draw_ref(state) % static_cast<unsigned>(ok.size());
        size_t chosen = k <= 1 ? 0 : k - 1;
        const Row& row = rows[ok[chosen]];
        e.names.push_back(row.name);
        if (row.value >= best && row.value <= limit) { best = row.value; e.chosenIndexForOut = ok[chosen]; }
    }
    if (best < 0) { e.finalState = state; return e; }
    e.result = 1;
    int rolls = static_cast<int>((rows[0].flags >> 11) & 0x1f);
    if (rolls > 0) (void)draw_ref(state);
    e.finalState = state;
    return e;
}

static int run_case(int code, unsigned seed, int wizards, int players, int limit, int category, int level) {
    build();
    std::memset(player, 0, sizeof player);
    for (int i = 0; i < players; ++i) put_u32(player + 23984 * i + 80, i < wizards ? 4 : 1);
    nmPlayers = players;
    syncRandState = seed;
    Expected want = reference(seed, wizards, players, limit, category, level);
    unsigned char out[72];
    std::memset(out, 0xee, sizeof out);
    int got = createNamedRandom__4itemPQ24item11descriptioniii(out, limit, category, level);
    if (got != want.result) { std::printf("case %d result %d want %d\n", code, got, want.result); return code; }
    if (syncRandState != want.finalState) { std::printf("case %d state\n", code); return code + 1000; }
    if (created != want.names) {
        std::printf("case %d names:", code);
        for (auto& n : created) std::printf(" %s", n.c_str());
        std::printf(" | want");
        for (auto& n : want.names) std::printf(" %s", n.c_str());
        std::printf("\n");
        return code + 2000;
    }
    if (want.result == 1) {
        // the output carries the attribute slot written by the last kept round
        const Row& kept = rows[want.chosenIndexForOut];
        bool potion = kept.name.find("potion") != std::string::npos;
        const int slot = expectedSlot(kept.name);
        if (!potion && slot >= 0) {
            unsigned char* id = out + 36 + 8 * slot;
            if (!(id[0] == 14 && id[1] == 0)) return code + 3000;
            int base = kept.value - 1200;
            if (base < 0) base = 0;
            base /= 2000;
            int amount = static_cast<int>(powf(static_cast<float>(base), 0.66f)) * 2 + 1;
            if (amount > 50) amount = 50;
            if (id[2] != amount || id[3] != 0 || id[4] != 0 || id[5] != 0 || id[6] != 0 || id[7] != 0) return code + 4000;
        } else {
            const int* ids = slotIds(kept.name);
            for (int i = 0; i < 4; ++i) {
                int got = static_cast<short>(out[36 + 8 * i] | (out[37 + 8 * i] << 8));
                if (got != ids[i]) return code + 5000;
            }
        }
        unsigned long long second;
        std::memcpy(&second, out + 8, 8);
        if ((second & ~(0x1full << 11)) != (0xfedcba9876543210ull & ~(0x1full << 11))) return code + 6000;
    }
    return 0;
}

int main() {
    // row 0 doubles as the roll row (flags carry 3 in bits 11..15) and is a plain eligible row.
    rows = {
        {"sword", 0, 0, 3ull << 11, 300},
        {"ruby", 0, 0, 0, 700000},
        {"opal", 0, 0, 0, 270000},
        {"potion1", 0, 2, 0, 5000},
        {"ring", 5, 1, 0x80000, 3300},
        {"hat", 10, 1, 0, 100},
        {"axe", 30, 0, 0, 20000},
        {"barred", 0, 0, 0x10000, 100},
        {"hidden", 0, 0, kFlag35, 100},
        {"cloak", 0, 1, 0, 1300},
        {"elixir potion", 0, 2, 0, 5000},
        {"gem", 0, 0, 0, 600000},
    };
    int rc;
    for (unsigned seed : {1u, 2u, 3u, 4u, 5u, 99u, 0x12345678u, 0xfffffff0u, 777u})
        for (int category : {-1, 0, 1, 2})
            for (int level : {-1, 0, 5, 6, 10, 12, 30, 40})
                for (int limit : {50, 300, 1300, 3300, 5000, 20000, 1 << 30})
                    if ((rc = run_case(1, seed, 1, 2, limit, category, level))) return rc;
    // the flag draw compares against r % 65535: r = 65535 gives 0 there but would give 65535 modulo 65536
    {
        unsigned inverse = 1;
        for (int i = 0; i < 6; ++i) inverse *= 2u - 1664525u * inverse;
        unsigned seed = (0xffffabcdu - 1013904223u) * inverse;
        unsigned probe = seed;
        if (draw_ref(probe) != 65535u) return 7;
        for (int level : {6, 40})
            if ((rc = run_case(7, seed, 1, 2, 1 << 30, 1, level))) return rc;
    }
    if ((rc = run_case(2, 31, 0, 3, 100000, 1, 40))) return rc;
    if ((rc = run_case(3, 32, 3, 3, 100000, 1, 40))) return rc;
    // limit below every value: no description is kept
    if ((rc = run_case(4, 33, 1, 2, 50, -1, 40))) return rc;
    // an impossible request
    if ((rc = run_case(5, 34, 1, 2, 4000, 7, 40))) return rc;
    // the roll row with no roll bits does not draw again
    rows[0].flags = 0;
    for (unsigned seed : {41u, 42u, 43u})
        if ((rc = run_case(6, seed, 1, 2, 4000, -1, 40))) return rc;
    std::puts("create named random passed");
    return 0;
}
