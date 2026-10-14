// Observed msg_load__8AutoPropPc at 0x00224d18, 84 bytes.
// Reads a word from the start of the stream, stores it at object+376, then calls the
// table entry at byte offset 144 of the table whose address is the word at object+72,
// with the adjusted object address and that word (in the AutoProp family tables that
// entry holds setState__8AutoPropi, slot 18). Returns 4, the bytes consumed.
// Unknown: what the stored word means beyond being the state passed to the slot.

#include "../w8-actor-virtuals-b-common/virtual_call.hpp"

extern "C" int msg_load__8AutoPropPc(void* self, const char* stream) {
    unsigned char* object = static_cast<unsigned char*>(self);
    int word = *reinterpret_cast<const int*>(stream);
    *reinterpret_cast<int*>(object + 376) = word;
    w8avb::call_slot(self, 144, word);
    return 4;
}
