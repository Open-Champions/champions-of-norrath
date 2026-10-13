// Observed joinCommander__3Orc at 0x0022b880, 92 bytes. Calls findCommander__3Orc(object). When it returns
// a commander C, calls listOnewayLink__FP8ListHeadT0Pv(object+1128, C, C); the list head at +1128 then
// has a first node whose word 12 (the node's data word, per the reads below) is C, and the unsigned byte
// at C+1132 is increased by one (no clamp). With no commander nothing else happens. Returns 1 when a
// commander was joined and otherwise the value findCommander gave, zero.
// Inferred: the +1132 byte counts the members that have joined; that is a reading of the increment and
// the matching decrement in leaveCommander, not a confirmed name. Nothing here ran on the EE.
#include "../w11-actor-b-common/access.hpp"

extern "C" {
void* findCommander__3Orc(void* self);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* other, void* data);

int joinCommander__3Orc(void* self) {
    using namespace w11b;
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* commander = static_cast<unsigned char*>(findCommander__3Orc(self));
    if (!commander) return 0;
    unsigned char* head = o + 1128;
    listOnewayLink__FP8ListHeadT0Pv(head, commander, commander);
    unsigned char* node = from_word(word(head, 0));
    unsigned char* data = from_word(word(node, 12));
    data[1132] = static_cast<unsigned char>(data[1132] + 1);
    return 1;
}
}
