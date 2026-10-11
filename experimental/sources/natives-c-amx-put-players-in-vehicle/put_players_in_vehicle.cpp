// Observed amxPutPlayersInVehicle__FP5__amxPi at 0x0013abc0, 236 bytes.
// Cell 1 is an AMX string. A null object returns 0. When activePlayer is
// not -1, the virtual slot at vtable byte 112 is called with that player.
// nmPlayers is then the walk count. A non-positive count returns 0. Each
// record starts at player byte 21872 and advances 23984 bytes. The active
// index is skipped. A header whose word at byte 12 has bit 3 set is
// skipped. Every other index calls the same slot, then reloads nmPlayers.
// The slot's signed halfword is the this-adjust and the function is four
// bytes later. The native returns 0. amx_GetAddr's stack slot is zeroed
// first; the original slot was left uninitialized when that call failed.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void* objectFindObject__FPc(char* name);
extern "C" int activePlayer;
extern "C" int nmPlayers;
extern "C" unsigned char player[];

namespace {
void* load_pointer(const unsigned char* bytes) {
    void* value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}

int load_int(const unsigned char* bytes) {
    int value;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = bytes[0];
    out[1] = bytes[1];
    out[2] = bytes[2];
    out[3] = bytes[3];
    return value;
}

short load_half(const unsigned char* bytes) {
    const unsigned value = bytes[0] | (static_cast<unsigned>(bytes[1]) << 8);
    return static_cast<short>(value);
}

using SlotCall = void (*)(void* self, int player);

SlotCall load_slot(const unsigned char* bytes) {
    SlotCall value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}

void call_slot(unsigned char* vehicle, int player_index) {
    auto* vtable = static_cast<unsigned char*>(load_pointer(vehicle + 72));
    const short adjust = load_half(vtable + 112);
    load_slot(vtable + 116)(vehicle + adjust, player_index);
}
}

extern "C" int amxPutPlayersInVehicle__FP5__amxPi(void* amx, int* params) {
    char name[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    auto* vehicle = static_cast<unsigned char*>(objectFindObject__FPc(name));
    if (vehicle == nullptr) return 0;
    int count;
    if (activePlayer == -1) count = nmPlayers;
    else {
        call_slot(vehicle, activePlayer);
        count = nmPlayers;
    }
    if (count <= 0) return 0;
    int index = 0;
    unsigned char* record = player + 21872;
    while (index < count) {
        if (index == activePlayer) index += 1;
        else {
            auto* header = static_cast<unsigned char*>(load_pointer(record));
            if (load_int(header + 12) & 8) index += 1;
            else {
                call_slot(vehicle, index);
                count = nmPlayers;
                index += 1;
            }
        }
        record += 23984;
    }
    return 0;
}
