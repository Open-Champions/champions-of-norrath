// Observed runStandardGlow__8Creature at 0x00109710, 244 bytes. The signed byte at
// object + 156 selects a glow colour by a jump table at 0x00460760: values 1 to 8
// pick the targets (halfword +140, +142, +144, +146):
//   1 (30, 30, 30, 50)   2 (30, 15, 15, 50)   3 (15, 15, 30, 50)   4 (10, 20, 10, 50)
//   5 (20, 20, 10, 50)   6 (20,  0, 30, 50)   7 (20, 20, 15, 50)   8 (20,  0, 30, 50)
// any other value picks (0, 0, 0, 0). Each of the four signed halfwords at +140
// through +146 is then raised to its target when it is lower; nothing else is
// written.
// Inferred: the halfwords are the glow colour and strength that Creature draw and
// the per-frame update decay; the byte is a glow kind.
// Unknown: what the kinds mean.
template <class T>
static inline T& field(void* object, unsigned offset) {
    return *reinterpret_cast<T*>(static_cast<char*>(object) + offset);
}

extern "C" void runStandardGlow__8Creature(void* self) {
    static const short targets[8][4] = {
        {30, 30, 30, 50}, {30, 15, 15, 50}, {15, 15, 30, 50}, {10, 20, 10, 50},
        {20, 20, 10, 50}, {20, 0, 30, 50},  {20, 20, 15, 50}, {20, 0, 30, 50}};
    short wanted[4] = {0, 0, 0, 0};
    const unsigned kind = static_cast<unsigned>(field<signed char>(self, 156) - 1);
    if (kind < 8u) {
        for (int part = 0; part < 4; ++part) wanted[part] = targets[kind][part];
    }
    for (int part = 0; part < 4; ++part) {
        short& current = field<short>(self, 140 + 2 * part);
        if (current < wanted[part]) current = wanted[part];
    }
}
