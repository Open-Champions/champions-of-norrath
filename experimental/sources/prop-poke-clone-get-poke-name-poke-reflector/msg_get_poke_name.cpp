#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_getPokeName__13PokeReflectorPUsi at 0x21c7d0, 160 bytes.
// The first halfword of the caller's buffer is cleared. The tag "target" is
// looked up; when present, its value names another object through
// objectFindObject. A found object gets its table slot byte 104 called with
// the buffer and the integer argument, and that call's result is returned.
// A missing tag or object returns zero. Slot byte 104 is msg_getPokeName in
// the published Vehicle table (inferred from that view only).
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence);
extern "C" unsigned char* objectFindObject__FPc(char* name);
extern "C" int msg_getPokeName__13PokeReflectorPUsi(void* self, unsigned short* buffer, int value) {
    using namespace vslots_b;
    write_u16(address(buffer), 0u);
    static char target[] = {'t', 'a', 'r', 'g', 'e', 't', '\0'};
    if (norrath_actor_find_tag_string(self, target, 0) == nullptr) return 0;
    void* other = objectFindObject__FPc(norrath_actor_find_tag_string(self, target, 0));
    if (other == nullptr) return 0;
    void* adjusted;
    using SlotCall = int (*)(void*, unsigned short*, int);
    SlotCall call = slot_function<SlotCall>(other, 104u, &adjusted);
    return call(adjusted, buffer, value);
}
