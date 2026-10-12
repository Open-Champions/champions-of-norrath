#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_getPokeName__13UserParamPropPUsi at 0x154bf8, 128 bytes.
// The word frameNm is stored at object+360 on every call, before any lookup.
// A missing "message" tag returns zero. Otherwise the tag value goes through
// textGetString and the resulting text is copied into the caller's buffer
// with ustrcpy, and the function returns 1. The integer argument is unused.
// Unknown: what the word at object+360 is read for later.
extern "C" unsigned frameNm;
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence);
extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void ustrcpy__FPUsT0(unsigned short* destination, unsigned short* source);
extern "C" int msg_getPokeName__13UserParamPropPUsi(void* self, unsigned short* buffer, int value) {
    using namespace vslots_b;
    (void)value;
    write_word(address(self) + 360u, frameNm);
    static char message[] = {'m', 'e', 's', 's', 'a', 'g', 'e', '\0'};
    if (norrath_actor_find_tag_string(self, message, 0) == nullptr) return 0;
    ustrcpy__FPUsT0(buffer, textGetString__FPc(norrath_actor_find_tag_string(self, message, 0)));
    return 1;
}
