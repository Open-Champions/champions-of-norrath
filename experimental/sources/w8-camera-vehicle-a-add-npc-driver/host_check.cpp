// Host contract for AddNPCDriver__7VehicleP3NPC. Not an EE plan input.
#include "add_npc_driver.cpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

static int failures = 0;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static std::uint32_t word(const unsigned char* base, int offset) {
    std::uint32_t value;
    std::memcpy(&value, base + offset, 4);
    return value;
}

int main() {
    static unsigned char vehicle[2304];
    static unsigned char npc[1200];
    static unsigned char vehicle_before[2304];
    static unsigned char npc_before[1200];
    const std::uint32_t inputs[] = {0u, 1u, 0x00100000u, 0xffffffffu, 0x00100001u, 0x12345678u};
    for (std::uint32_t flags : inputs) {
        std::memset(vehicle, 0x21, sizeof vehicle);
        std::memset(npc, 0x43, sizeof npc);
        std::memcpy(npc + 12, &flags, 4);
        std::memcpy(vehicle_before, vehicle, sizeof vehicle);
        std::memcpy(npc_before, npc, sizeof npc);
        AddNPCDriver__7VehicleP3NPC(vehicle, npc);
        expect(word(vehicle, 2148) == 1, "driver marker set");
        unsigned char* stored = nullptr;
        std::memcpy(&stored, npc + 1132, sizeof stored);
        expect(stored == vehicle, "vehicle pointer stored in the npc");
        expect(word(npc, 12) == ((flags | 1u) & 0xffefffffu), "flags: bit 0 set, bit 20 cleared");
        std::memcpy(vehicle_before + 2148, vehicle + 2148, 4);
        expect(std::memcmp(vehicle, vehicle_before, sizeof vehicle) == 0, "no other vehicle byte changes");
        std::memcpy(npc_before + 12, npc + 12, 4);
        std::memcpy(npc_before + 1132, npc + 1132, sizeof stored);
        expect(std::memcmp(npc, npc_before, sizeof npc) == 0, "no other npc byte changes");
    }
    return failures ? 1 : 0;
}
