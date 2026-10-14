#include <cstdio>
#include <cstring>

extern "C" int amxPutPlayersInVehicle__FP5__amxPi(void*, int*);
extern "C" int activePlayer = 0;
extern "C" int nmPlayers = 0;
extern "C" unsigned char player[96000] = {};

static unsigned char vehicle[200];
static unsigned char table[128];
static unsigned char headers[3][16];
static int called[8];
static void* selves[8];
static int calls;
static int present = 1;
static int shrink;

extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int slot;
    *physical = &slot;
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", "cart");
    return 0;
}

extern "C" void* objectFindObject__FPc(char* name) {
    if (!present || std::strcmp(name, "cart") != 0) return nullptr;
    return vehicle;
}

static void on_slot(void* self, int player_index) {
    selves[calls] = self;
    called[calls] = player_index;
    calls += 1;
    if (shrink && player_index == 0) nmPlayers = 1;
}

static void arm() {
    void* pointer = table;
    std::memcpy(vehicle + 72, &pointer, sizeof pointer);
    short adjust = 4;
    std::memcpy(table + 112, &adjust, sizeof adjust);
    auto function = on_slot;
    std::memcpy(table + 116, &function, sizeof function);
}

static void set_header(int index, int flags) {
    std::memcpy(headers[index] + 12, &flags, 4);
    unsigned char* pointer = headers[index];
    std::memcpy(player + 21872 + index * 23984, &pointer, sizeof pointer);
}

int main() {
    int params[2] = {4, 1};
    arm();
    present = 0;
    if (amxPutPlayersInVehicle__FP5__amxPi(nullptr, params) != 0 || calls != 0) return 1;
    present = 1;

    activePlayer = -1;
    nmPlayers = 0;
    if (amxPutPlayersInVehicle__FP5__amxPi(nullptr, params) != 0 || calls != 0) return 2;

    nmPlayers = 2;
    set_header(0, 0);
    set_header(1, 8);
    if (amxPutPlayersInVehicle__FP5__amxPi(nullptr, params) != 0) return 3;
    if (calls != 1 || called[0] != 0 || selves[0] != vehicle + 4) return 4;

    calls = 0;
    shrink = 1;
    nmPlayers = 2;
    set_header(0, 0);
    set_header(1, 0);
    if (amxPutPlayersInVehicle__FP5__amxPi(nullptr, params) != 0) return 8;
    if (calls != 1 || called[0] != 0 || nmPlayers != 1) return 9;
    shrink = 0;

    calls = 0;
    activePlayer = 1;
    nmPlayers = 2;
    set_header(0, 0);
    set_header(1, 0);
    if (amxPutPlayersInVehicle__FP5__amxPi(nullptr, params) != 0) return 5;
    if (calls != 2 || called[0] != 1 || called[1] != 0) return 6;
    if (selves[0] != vehicle + 4 || selves[1] != vehicle + 4) return 7;
    std::printf("put players in vehicle passed\n");
    return 0;
}
