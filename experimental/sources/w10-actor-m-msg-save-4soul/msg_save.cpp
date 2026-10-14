#include "../w10-actor-m-common/access.hpp"
// Observed msg_save__4SoulPc at 0x1f4f40, 104 bytes. The text at levelName+81 is compared with strcasecmp against the
// string at 0x485240 (the same one the Soul constructor compares). On a match nothing is
// saved and 0 is returned. Otherwise msg_save__8CreaturePc runs with the same two arguments and its
// byte count is returned. Inferred: the output/input pointer is the second argument, from the base
// routine's signature. Unknown: what level name 0x485240 holds, and why that level skips the base save.
extern "C" {
extern char levelName[];
extern unsigned char norrath_ctors_lava_wraith_view_485240[];
int strcasecmp(const char* left, const char* right);
int msg_save__8CreaturePc(void* self, char* buffer);
}

extern "C" int msg_save__4SoulPc(void* self, char* buffer) {
    if (strcasecmp(levelName + 81, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_485240)) == 0) return 0;
    return msg_save__8CreaturePc(self, buffer);
}
