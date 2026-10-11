#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_poke__8Creaturei at 0x111df0, 372 bytes.
// A zero word at object+892 returns. The argument selects a 23984-byte player
// record. A zero word at byte 21872 of that record returns. The tag "shop"
// then goes through the existing findTagInt body. A nonzero tag, someone in
// the shop, and a local player calls shopStart with 0 and 1 and returns.
// A nonzero tag with nobody in the shop falls through. Otherwise the entity
// point at byte 16 is subtracted from this object's point, and icos/isin of
// the halfword at object+56 form dx*cos + dy*sin. Unless that is greater than
// zero, the function returns. DLG_SetMutex then runs. A zero result stores the
// argument in activePlayer, finds "dialogScript", and passes that pointer to
// scriptStart. The third single difference is computed in the image and never
// read.
extern "C" unsigned char player[];
extern "C" int activePlayer;
extern "C" int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence);
extern "C" int gameSomeoneInShop__Fv();
extern "C" int netPlayerIsLocal__Fi(int index);
extern "C" void shopStart__Fiii(int index, int second, int third);
extern "C" float icos__Fi(int angle);
extern "C" float isin__Fi(int angle);
extern "C" int DLG_SetMutex__Fv();
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence);
extern "C" void scriptStart__FPc(char* script);
extern "C" void msg_poke__8Creaturei(void* self, int index) {
    using namespace vslots_b;
    if (read_word(address(self) + 892u) == 0u) return;
    const unsigned slot = static_cast<unsigned>(index) * 23984u + 21872u;
    if (read_word(address(player) + slot) == 0u) return;
    static char shop[] = {'s', 'h', 'o', 'p', '\0'};
    if (norrath_game_object_find_tag_int(self, shop, 0) != 0) {
        if (gameSomeoneInShop__Fv() != 0) {
            if (netPlayerIsLocal__Fi(index) != 0) shopStart__Fiii(index, 0, 1);
            return;
        }
    }
    const unsigned entity_bits = read_word(address(player) + slot);
    const Address entity = address(pointer(entity_bits));
    const float dx = read_single(entity + 16u) - read_single(address(self) + 16u);
    const float dy = read_single(entity + 20u) - read_single(address(self) + 20u);
    const float cosine = icos__Fi(read_i16(address(self) + 56u));
    const float sine = isin__Fi(read_i16(address(self) + 56u));
    if (!(0.0f < dx * cosine + dy * sine)) return;
    if (DLG_SetMutex__Fv() != 0) return;
    activePlayer = index;
    static char dialog[] = {'d', 'i', 'a', 'l', 'o', 'g', 'S', 'c', 'r', 'i', 'p', 't', '\0'};
    scriptStart__FPc(norrath_actor_find_tag_string(self, dialog, 0));
}
