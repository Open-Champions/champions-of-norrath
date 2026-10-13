// Observed checkIfSpellIsRunning__8CreatureRC9type_info at 0x001138d0, 120 bytes.
// Walks the singly linked list whose head word is at +888 (each node's first word is the next
// node, the word at +12 is the spell object). A node with a null object is skipped. For any
// other node the table pointer at object+20 is loaded and the function word at table byte 4 is
// called with no arguments to obtain a type-info node; __eq__C9type_infoRC9type_info compares it
// with the argument. The first object whose type matches is returned; the result is 0 when the
// list ends without a match.
// Inferred: the +888 list holds the running spells (the name of the function).
// Unknown: the layout of the table at object+20 beyond the slot at byte 4.
#include "../w11-creature-a-common/words.hpp"

extern "C" int __eq__C9type_infoRC9type_info(const void* left, const void* right);

extern "C" void* checkIfSpellIsRunning__8CreatureRC9type_info(void* self, const void* type) {
    using namespace w11_creature_a;
    for (void* node = at<void*>(self, 888); node != nullptr; node = at<void*>(node, 0)) {
        void* spell = at<void*>(node, 12);
        if (spell == nullptr) continue;
        const unsigned char* table = at<const unsigned char*>(spell, 20);
        const void* spell_type = reinterpret_cast<const void* (*)()>(*reinterpret_cast<const Ptr*>(table + 4))();
        if (__eq__C9type_infoRC9type_info(spell_type, type) != 0) return spell;
    }
    return nullptr;
}
