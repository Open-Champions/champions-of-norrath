// Observed AddNPCDriver__7VehicleP3NPC at 0x00277ad8, 40 bytes.
// vehicle+2148 becomes 1. The NPC's word at +1132 receives the vehicle
// pointer. The NPC flag word at +12 gets bit 0 set and bit 20 (0x100000)
// cleared, in that order, from a fresh read. Nothing is returned. Nothing here
// was executed on the EE.
typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void AddNPCDriver__7VehicleP3NPC(unsigned char* vehicle, unsigned char* npc) {
    *reinterpret_cast<AliasedWord*>(vehicle + 2148) = 1;
    *reinterpret_cast<unsigned char**>(npc + 1132) = vehicle;
    AliasedWord& flags = *reinterpret_cast<AliasedWord*>(npc + 12);
    flags = (flags | 1u) & 0xffefffffu;
}
