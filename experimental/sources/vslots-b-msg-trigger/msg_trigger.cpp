#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_trigger__8Creaturei at 0x1137b0, 116 bytes.
// A zero word at object+892 returns. The integer argument is not read.
// Otherwise findTagString is called with the bytes "dialogScript" and
// occurrence 0. A null result returns. DLG_SetMutex then runs; a nonzero
// result returns. findTagString runs again and its result is passed to
// scriptStart. norrath_actor_find_tag_string is the existing findTagString
// body. DLG_SetMutex and scriptStart are not defined here.
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence);
extern "C" unsigned DLG_SetMutex__Fv();
extern "C" void scriptStart__FPc(char* script);
extern "C" void msg_trigger__8Creaturei(void* self, int value) {
    (void)value;
    using namespace vslots_b;
    if (read_word(address(self) + 892u) == 0u) return;
    static char tag[] = {'d', 'i', 'a', 'l', 'o', 'g', 'S', 'c', 'r', 'i', 'p', 't', '\0'};
    char* script = norrath_actor_find_tag_string(self, tag, 0);
    if (script == nullptr) return;
    if (DLG_SetMutex__Fv() != 0u) return;
    scriptStart__FPc(norrath_actor_find_tag_string(self, tag, 0));
}
