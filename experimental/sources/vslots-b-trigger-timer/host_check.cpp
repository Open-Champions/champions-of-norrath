#define NORRATH_VSLOTS_B_HOST 1
#include "trigger_timer.cpp"
#include <cstdio>
#include <cstring>
static int adds = 0;
static int tags = 0;
static int tag_result = 2;
static const char* seen_tag = nullptr;
extern "C" unsigned norrath_actor_add_run_list(void* object) {
    (void)object;
    ++adds;
    return 1u;
}
extern "C" int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence) {
    (void)object;
    ++tags;
    seen_tag = tag;
    if (occurrence != 0) tag_result = -999;
    return tag_result;
}
int main() {
    unsigned char object[88];
    std::memset(object, 0x39, sizeof object);
    msg_trigger__5Timeri(object, 5);
    if (adds != 1 || tags != 1) return 1;
    if (std::strcmp(seen_tag, "time") != 0) return 1;
    if (vslots_b::read_word(vslots_b::address(object) + 80u) != 5u) return 1;
    if (vslots_b::read_word(vslots_b::address(object) + 76u) != 120u) return 1;
    tag_result = -1;
    msg_trigger__5Timeri(object, -4);
    if (vslots_b::read_word(vslots_b::address(object) + 80u) != 0xfffffffcu) return 1;
    if (vslots_b::read_word(vslots_b::address(object) + 76u) != 0xffffffc4u) return 1;
    tag_result = static_cast<int>(0x10000000u);
    msg_trigger__5Timeri(object, 0);
    if (vslots_b::read_word(vslots_b::address(object) + 76u) != 0xc0000000u) return 1;
    return 0;
}
