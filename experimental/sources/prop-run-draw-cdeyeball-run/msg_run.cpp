#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__9CdEyeball at 0x21d1e0, 192 bytes.
// objectActivationCheck gets the object and 600. Virtual slot byte 168 is then
// called with the adjusted this and no other argument set. When the word at
// object+576 is non-null, virtual slot byte 176 is called with the adjusted
// this, integer 0 and a copy of the 12-byte point at (word at that
// pointer+12)+16. Finally the signed difference d = halfword object+962 minus
// halfword object+56, narrowed to 16 bits, limits the facing: above 6000
// stores object+962 - 6000 at object+56, below -6000 stores object+962 + 6000.
// Unknown: the names of the two slots and what object+576 and +962 mean.
extern "C" void objectActivationCheck__8Creaturei(void* self, int distance);
extern "C" void msg_run__9CdEyeball(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    objectActivationCheck__8Creaturei(self, 600);
    {
        void* adjusted = nullptr;
        using SlotCall = void (*)(void*);
        SlotCall call = slot_function<SlotCall>(self, 168u, &adjusted);
        call(adjusted);
    }
    const unsigned target = read_word(base + 576u);
    if (target != 0u) {
        const Address source = address(pointer(read_word(address(pointer(target)) + 12u))) + 16u;
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = static_cast<unsigned char>(read_u8(source + i));
        void* adjusted = nullptr;
        using SlotCall = void (*)(void*, int, const void*);
        SlotCall call = slot_function<SlotCall>(self, 176u, &adjusted);
        call(adjusted, 0, point);
    }
    const unsigned facing = read_u16(base + 962u);
    const short difference = static_cast<short>(facing - read_u16(base + 56u));
    if (difference > 6000) write_u16(base + 56u, facing - 6000u);
    else if (difference < -6000) write_u16(base + 56u, facing + 6000u);
}
