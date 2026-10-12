#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_poke__13PokeReflectori at 0x21c870, 172 bytes.
// The tag "target" is looked up; when present its value goes to
// objectFindObject with the list address and the count 16. That callee fills a
// list of object words on the stack. The image tests the first word and, when
// nonzero, walks forward calling table slot byte 112 on each object with the
// poke argument until a zero word. The list is not cleared first, and no bound
// beyond the callee's 16 is applied. Slot byte 112 is msg_poke in the
// published Vehicle table (inferred from that view only).
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence);
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** list, int count);
extern "C" void msg_poke__13PokeReflectori(void* self, int value) {
    using namespace vslots_b;
    static char target[] = {'t', 'a', 'r', 'g', 'e', 't', '\0'};
    if (norrath_actor_find_tag_string(self, target, 0) == nullptr) return;
    void* list[16];
    objectFindObject__FPcPP10GameObjecti(norrath_actor_find_tag_string(self, target, 0), list, 16);
    if (list[0] == nullptr) return;
    void** cursor = list;
    do {
        void* current = *cursor;
        ++cursor;
        void* adjusted;
        using SlotCall = void (*)(void*, int);
        SlotCall call = slot_function<SlotCall>(current, 112u, &adjusted);
        call(adjusted, value);
    } while (*cursor != nullptr);
}
