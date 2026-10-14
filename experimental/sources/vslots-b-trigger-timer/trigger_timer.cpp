#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_trigger__5Timeri at 0x21cc70, 88 bytes.
// The object is added to the run list first. findTagInt then reads the tag
// bytes "time" at occurrence 0. The argument is stored at object+80. The tag
// result times 60, in 32-bit wrap, is stored at object+76.
// norrath_actor_add_run_list is the existing objectAddToRunList body.
// norrath_game_object_find_tag_int is the existing findTagInt body.
extern "C" unsigned norrath_actor_add_run_list(void* object);
extern "C" int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence);
extern "C" void msg_trigger__5Timeri(void* self, int value) {
    using namespace vslots_b;
    norrath_actor_add_run_list(self);
    static char tag[] = {'t', 'i', 'm', 'e', '\0'};
    const unsigned scaled = static_cast<unsigned>(norrath_game_object_find_tag_int(self, tag, 0)) * 60u;
    write_word(address(self) + 80u, static_cast<unsigned>(value));
    write_word(address(self) + 76u, scaled);
}
