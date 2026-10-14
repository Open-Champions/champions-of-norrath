#include "../vslots-b-bytes/bytes.hpp"
// Observed GetInstType__C13VIGroupSpriteR7VIScene at 0x101db98, 168 bytes.
// The word at object+64 is a member count and the member records start at
// object+68, 32 bytes apart, with the first word of each record being an index
// for VIScene::Sprite(int). For each member, in order, the scene lookup
// Sprite__7VIScenei is called with the scene and that index; the returned
// sprite's virtual slot (vtable word at sprite+60, entry at table+16: a signed
// halfword adjustment and a function pointer four bytes on) is then called with
// the adjusted sprite and the scene. If the answer is exactly 1 the function
// returns 1 at once; after all members it returns 0. The count is read again
// after every member, so a member that changes it is honoured.
// Unknown: what the answer 1 means by name, and what the other 28 bytes of a
// member record hold.
extern "C" void* Sprite__7VIScenei(void* scene, int index);

extern "C" int GetInstType__C13VIGroupSpriteR7VIScene(const void* self, void* scene) {
    using namespace vslots_b;
    const Address base = address(self);
    typedef int (*SlotCall)(void* sprite, void* scene);
    int i = 0;
    if (signed_word(read_word(base + 64u)) > 0) {
        do {
            void* const sprite = Sprite__7VIScenei(scene, signed_word(read_word(base + 68u + 32u * static_cast<unsigned>(i))));
            const Address entry = address(pointer(read_word(address(sprite) + 60u))) + 16u;
            SlotCall call = reinterpret_cast<SlotCall>(pointer(read_word(entry + 4u)));
            ++i;
            if (call(static_cast<unsigned char*>(sprite) + read_i16(entry), scene) == 1) return 1;
        } while (i < signed_word(read_word(base + 64u)));
    }
    return 0;
}
