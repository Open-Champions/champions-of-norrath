#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_draw__3Cat at 0x1eed90, 96 bytes.
// The model passed to draw is the word at (object+36)+12. The texture is the
// word at 20 bytes past object+36 plus eight times the word at object+960.
// Before the call, halfword object+56 is reduced by halfword object+976.
// After the call, halfword object+976 is added back to whatever object+56
// holds then. The fourth draw argument is null.
extern "C" void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra);
extern "C" void msg_draw__3Cat(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    const Address group = address(pointer(read_word(base + 36u)));
    const unsigned index = read_word(base + 960u);
    const Address indexed = group + (static_cast<Address>(index) << 3);
    const unsigned removed = read_u16(base + 976u);
    write_u16(base + 56u, read_u16(base + 56u) - removed);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, pointer(read_word(group + 12u)), pointer(read_word(indexed + 20u)), nullptr);
    write_u16(base + 56u, read_u16(base + 56u) + removed);
}
