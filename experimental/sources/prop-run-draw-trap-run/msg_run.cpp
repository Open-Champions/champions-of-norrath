#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__4Trap at 0x21b298, 400 bytes.
// A box is built around the point at object+16 with the single at object+28
// as the half-size in each axis. objectFindInBox fills a 128-entry list of
// object pointers (stack bytes 0..511 in the reference) from it. A 4-byte
// seen array is cleared. For each listed object whose word at +12 has
// (word & 0x48) == 0x40, the byte at index (word at +960) of the seen array
// is set to 1, and when the previous seen byte at object+76 plus that index
// is zero, gameAttackResolver is called with the listed object, this object,
// 20.0, 30.0, 10000 and zeros for the rest (null description, false). After
// the list ends, the four seen bytes are copied to object+76..+79. The index
// is not bounds checked in the reference; a word at +960 above 3 would write
// outside the local array.
// Observed: 20.0 and 30.0 are the lui constants 0x41a00000 and 0x41f00000.
// Unknown: whether the list is null terminated by objectFindInBox (the loop
// stops at the first null word) and what the called function returns.
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* low, const void* high, void** objects, int count);
extern "C" void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
    void* target, void* source, float first, float second, int a, int b, int c, int d, void* description, bool flag);

extern "C" void msg_run__4Trap(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    void* objects[128];
    float low[3];
    float high[3];
    unsigned char seen[4];
    const float extent = read_single(base + 28u);
    const float x = read_single(base + 16u);
    const float y = read_single(base + 20u);
    const float z = read_single(base + 24u);
    high[0] = x + extent;
    high[1] = y + extent;
    high[2] = z + extent;
    low[0] = x - extent;
    low[1] = y - extent;
    low[2] = z - extent;
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, objects, 128);
    for (int i = 3; i >= 0; --i) seen[i] = 0;
    for (unsigned index = 0; objects[index] != nullptr; ++index) {
        const Address other = address(objects[index]);
        if ((read_word(other + 12u) & 0x48u) != 0x40u) continue;
        const unsigned player = read_word(other + 960u);
        seen[player] = 1;
        if (static_cast<signed char>(read_u8(base + 12u + read_word(other + 960u) + 64u)) == 0) {
            gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
                objects[index], self, 20.0f, 30.0f, 10000, 0, 0, 0, nullptr, false);
        }
    }
    for (unsigned i = 0; i < 4u; ++i) write_u8(base + 76u + i, seen[i]);
}
