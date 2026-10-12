#define NORRATH_W8C_HOST 1
#include "trigger.cpp"
#include "../w8-actor-virtuals-c-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
#include <strings.h>
static int failures = 0;
static int slot_calls = 0, lump_calls = 0, sound_calls = 0;
static void* slot_self = nullptr;
static char seen_lump[64], seen_resource[64];
static void* sound_object = nullptr;
static float sound_gain = 0.0f;
static void* sound_header = nullptr;
static char marker;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void* norrath_lump_find_resource(char* lump_name, char* resource_name) {
    ++lump_calls;
    std::strcpy(seen_lump, lump_name);
    std::strcpy(seen_resource, resource_name);
    return &marker;
}
extern "C" int norrath_sound_play_sequence(void* header, void* object, float gain) {
    ++sound_calls; sound_header = header; sound_object = object; sound_gain = gain; return 0;
}
static void slot(void* self) { ++slot_calls; slot_self = self; }

static void run(unsigned gate, unsigned count, unsigned state, const char* name, int sounds,
                const char* lump, const char* resource) {
    unsigned char object[400];
    unsigned char table[256];
    std::memset(object, 0, sizeof object);
    std::memset(table, 0, sizeof table);
    w8c::host_install_slot(object, table, 136u, 8, reinterpret_cast<void*>(slot));
    const w8c::Address a = w8c::address(object);
    w8c::write_word(a + 284u, gate);
    w8c::write_word(a + 288u, count);
    w8c::write_word(a + 376u, state);
    std::strcpy(reinterpret_cast<char*>(object) + 76, name);
    slot_calls = lump_calls = sound_calls = 0;
    msg_trigger__8AutoPropi(object, 5);
    const bool gated = (gate & 0x40u) != 0u && count != 0u;
    expect(slot_calls == (gated ? 1 : 0), "slot call follows the gate");
    if (gated) expect(slot_self == object + 8, "slot gets adjusted object");
    expect(sound_calls == (gated ? sounds : 0), "sound count");
    if (gated && sounds) {
        expect(std::strcmp(seen_lump, lump) == 0 && std::strcmp(seen_resource, resource) == 0, "lump and resource");
        expect(sound_object == object && sound_gain == 1.0f && sound_header == &marker, "sound arguments");
    }
}
int main() {
    run(0x40u, 1u, 1u, "GothTrap", 1, "gothtrap.lmp", "cs_wallcrumble.anm");
    run(0x40u, 1u, 0u, "GothTrap", 0, "", "");
    run(0x40u, 1u, 1u, "other", 0, "", "");
    run(0x40u, 1u, 1u, "FAYACTUATOR", 1, "fayswitc.lmp", "fayacthack.anm");
    run(0x40u, 1u, 0u, "fayActuator", 1, "fayswitc.lmp", "fayacthack.anm");
    run(0x40u, 1u, 3u, "fayactuator", 1, "fayswitc.lmp", "fayacthack.anm");
    run(0x3fu, 1u, 1u, "gothtrap", 1, "", "");
    run(0x40u, 0u, 1u, "gothtrap", 1, "", "");
    return failures ? 1 : 0;
}
