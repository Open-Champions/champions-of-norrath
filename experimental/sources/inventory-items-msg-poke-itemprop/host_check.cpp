#define NORRATH_INVENTORY_HOST 1
#include "msg_poke.cpp"
#include <cstring>
#include <vector>

extern "C" int nmPlayers = 3;
static std::vector<int> asked;
static std::vector<void*> asked_description;
static int results[8];
static int calls;
static unsigned called_function;
static void* called_self;
static int called_flag;
extern "C" int playerMaybeGetItem__FiPQ24item11description(int player, void* description) {
    asked.push_back(player);
    asked_description.push_back(description);
    return results[player & 7];
}
static unsigned char g_table[24];
extern "C" const unsigned char* host_slot(unsigned word) { return word == 0x77 ? g_table : nullptr; }
extern "C" void host_call(unsigned function, void* self, int flag) {
    ++calls;
    called_function = function;
    called_self = self;
    called_flag = flag;
}

static void reset() {
    asked.clear(); asked_description.clear();
    std::memset(results, 0, sizeof results);
    calls = 0;
}

int main() {
    alignas(8) unsigned char object[400];
    std::memset(object, 0, sizeof object);
    // The vtable word is a token; its slot at +8 is {adjustment -16, function 0xabcd}.
    unsigned token = 0x77;
    std::memcpy(object + 72, &token, 4);
    short adjust = -16;
    std::memcpy(g_table + 8, &adjust, 2);
    unsigned function = 0xabcdu;
    std::memcpy(g_table + 12, &function, 4);

    // Flag clear: only the named player is asked.
    reset();
    object[340] = 0x7f;
    results[2] = 1;
    msg_poke__8ItemPropi(object, 2);
    if (asked.size() != 1 || asked[0] != 2 || asked_description[0] != object + 328) return 1;
    if (calls != 1 || called_function != 0xabcdu || called_flag != 3 || called_self != object - 16) return 2;
    reset();
    msg_poke__8ItemPropi(object, 1);
    if (asked.size() != 1 || calls != 0) return 3;
    // Flag set: every player is asked, in order, even after one has taken it.
    reset();
    object[340] = 0x80;
    results[1] = 5;
    msg_poke__8ItemPropi(object, 0);
    if (asked.size() != 3 || asked[0] != 0 || asked[1] != 1 || asked[2] != 2 || calls != 1) return 4;
    reset();
    msg_poke__8ItemPropi(object, 0);
    if (asked.size() != 3 || calls != 0) return 5;
    reset();
    nmPlayers = 0;
    results[0] = 1;
    msg_poke__8ItemPropi(object, 0);
    if (!asked.empty() || calls != 0) return 6;
    nmPlayers = -2;
    msg_poke__8ItemPropi(object, 0);
    if (!asked.empty() || calls != 0) return 7;
    return 0;
}
