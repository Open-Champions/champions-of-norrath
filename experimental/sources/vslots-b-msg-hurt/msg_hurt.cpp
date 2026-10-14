#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_hurt__8CreaturefP10GameObjecti at 0x113618, 240 bytes.
// A nonzero signed byte at object+570 returns without a write.
// Otherwise the word at object+744 is ORed with the integer argument, the
// current frameNm word is copied to object+812, and the single at object+560
// is reduced by the amount while the single at object+192 is increased by it.
// When the other object is non-null and the single at object+560 is greater
// than zero, iatan2 of the two point differences is subtracted from 16384 and
// stored at object+748, listDestroy runs at object+752, and a one-way link is
// added there. Bit 0x80 at other+12 with a non-null word at other+76 replaces
// the linked object with the word at that pointer+12.
// Inferred: frameNm is the existing 4-byte word at 0x4844a4. listDestroy and
// the one-way link are the already defined norrath_list_destroy and
// norrath_list_oneway_link. Unknown: R5900 NaN and denormal results of the
// compare and the two single updates.

#define read_single(address) vslots_b::read_single(address)
#define write_single(address, value) vslots_b::write_single((address), (value))

extern "C" unsigned frameNm;
extern "C" void norrath_list_destroy(void* head);
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value);
extern "C" int iatan2__Fff(float first, float second);

extern "C" void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, void* other, int kind) {
    using namespace vslots_b;
    const Address base = address(self);
    if (read_u8(base + 570u) != 0u) return;
    write_word(base + 812u, frameNm);
    write_word(base + 744u, read_word(base + 744u) | static_cast<unsigned>(kind));
    if (other != nullptr && 0.0f < read_single(base + 560u)) {
        const Address other_base = address(other);
        const float first = read_single(base + 16u) - read_single(other_base + 16u);
        const float second = read_single(base + 20u) - read_single(other_base + 20u);
        write_u16(base + 748u, 16384u - static_cast<unsigned>(iatan2__Fff(first, second)));
        void* head = static_cast<unsigned char*>(self) + 752u;
        norrath_list_destroy(head);
        void* linked = other;
        if ((read_word(other_base + 12u) & 0x80u) != 0u) {
            const unsigned inner_bits = read_word(other_base + 76u);
            if (inner_bits != 0u) linked = pointer(read_word(address(pointer(inner_bits)) + 12u));
        }
        if (linked != nullptr) norrath_list_oneway_link(head, linked, linked);
    }
    write_single(base + 560u, read_single(base + 560u) - amount);
    write_single(base + 192u, read_single(base + 192u) + amount);
}
