// Observed leaveCommander__3Orc at 0x0022b8e0, 80 bytes. When the word at object+1128 (a list head) is
// non-zero: let C be word 12 of the node it points to; if the signed byte at C+1132 is above zero, it is
// decreased by one; then listDestroy__FP8ListHead(object+1128) is called and the word at +1128 is set to
// zero. With a zero word nothing happens. No value is returned (the caller in findCommander ignores one).
// Inferred: counterpart of joinCommander's increment. Nothing here ran on the EE.
#include "../w11-actor-b-common/access.hpp"

extern "C" {
void listDestroy__FP8ListHead(void* head);

void leaveCommander__3Orc(void* self) {
    using namespace w11b;
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* head = o + 1128;
    if (!word(o, 1128)) return;
    unsigned char* data = from_word(word(from_word(word(o, 1128)), 12));
    if (static_cast<signed char>(data[1132]) > 0) data[1132] = static_cast<unsigned char>(data[1132] - 1);
    listDestroy__FP8ListHead(head);
    word(o, 1128) = 0;
}
}
