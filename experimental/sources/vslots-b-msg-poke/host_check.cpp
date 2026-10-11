#define NORRATH_VSLOTS_B_HOST 1
#include "msg_poke.cpp"
#include <cstring>
extern "C" unsigned char player[23984u * 2u] = {};
extern "C" int activePlayer = 7;
static int tag_ints = 0;
static int shops = 0;
static int locals = 0;
static int shop_starts = 0;
static int cosines = 0;
static int sines = 0;
static int mutexes = 0;
static int scripts = 0;
static int sine_angle = -1;
static int start_index = -1;
static int start_second = -1;
static int start_third = -1;
static int player_at_find = -1;
static char* script_result = nullptr;
static int shop_tag = 0;
static int someone = 0;
static int local = 0;
static int mutex = 0;
extern "C" int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence) {
    (void)object;
    ++tag_ints;
    if (occurrence != 0 || tag == nullptr || tag[0] != 's' || tag[4] != '\0') return 99;
    return shop_tag;
}
extern "C" int gameSomeoneInShop__Fv() {
    ++shops;
    return someone;
}
extern "C" int netPlayerIsLocal__Fi(int index) {
    ++locals;
    if (index != 1) return 98;
    return local;
}
extern "C" void shopStart__Fiii(int index, int second, int third) {
    ++shop_starts;
    start_index = index;
    start_second = second;
    start_third = third;
}
extern "C" float icos__Fi(int angle) {
    ++cosines;
    if (angle != 7) return 0.0f;
    return 1.0f;
}
extern "C" float isin__Fi(int angle) {
    ++sines;
    sine_angle = angle;
    return 0.0f;
}
extern "C" int DLG_SetMutex__Fv() {
    ++mutexes;
    return mutex;
}
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence) {
    (void)object;
    player_at_find = activePlayer;
    if (occurrence != 0 || tag == nullptr || tag[0] != 'd') return nullptr;
    return script_result;
}
extern "C" void scriptStart__FPc(char* script) {
    ++scripts;
    if (script != script_result) scripts = -10;
}
static void reset() {
    tag_ints = shops = locals = shop_starts = cosines = sines = mutexes = scripts = 0;
    sine_angle = start_index = -1;
    player_at_find = -1;
    activePlayer = 7;
    shop_tag = someone = local = mutex = 0;
}
int main() {
    unsigned char object[900];
    unsigned char entity[32];
    std::memset(object, 0, sizeof object);
    std::memset(player, 0, sizeof player);
    std::memset(entity, 0, sizeof entity);
    vslots_b::host_note(entity);
    vslots_b::write_word(vslots_b::address(player) + 23984u + 21872u, vslots_b::address_bits(entity));
    vslots_b::write_single(vslots_b::address(object) + 16u, 1.0f);
    vslots_b::write_single(vslots_b::address(object) + 20u, 4.0f);
    vslots_b::write_single(vslots_b::address(entity) + 16u, 3.0f);
    vslots_b::write_single(vslots_b::address(entity) + 20u, 9.0f);
    vslots_b::write_u16(vslots_b::address(object) + 56u, 7u);
    script_result = reinterpret_cast<char*>(entity);

    reset();
    msg_poke__8Creaturei(object, 1);
    if (tag_ints != 0 || activePlayer != 7) return 1;

    vslots_b::write_word(vslots_b::address(object) + 892u, 1u);
    vslots_b::write_word(vslots_b::address(player) + 23984u + 21872u, 0u);
    reset();
    msg_poke__8Creaturei(object, 1);
    if (tag_ints != 0 || cosines != 0) return 2;
    vslots_b::write_word(vslots_b::address(player) + 23984u + 21872u, vslots_b::address_bits(entity));

    reset();
    vslots_b::write_single(vslots_b::address(entity) + 16u, 0.0f);
    msg_poke__8Creaturei(object, 1);
    if (tag_ints != 1 || cosines != 1 || sines != 1 || sine_angle != 7 || mutexes != 0 || activePlayer != 7) return 3;
    vslots_b::write_single(vslots_b::address(entity) + 16u, 3.0f);

    reset();
    mutex = 1;
    msg_poke__8Creaturei(object, 1);
    if (mutexes != 1 || scripts != 0 || activePlayer != 7) return 4;

    reset();
    msg_poke__8Creaturei(object, 1);
    if (scripts != 1 || player_at_find != 1 || activePlayer != 1 || cosines != 1) return 5;

    reset();
    shop_tag = 4;
    someone = 1;
    local = 1;
    msg_poke__8Creaturei(object, 1);
    if (shop_starts != 1 || start_index != 1 || start_second != 0 || start_third != 1 || cosines != 0 || activePlayer != 7) return 6;

    reset();
    shop_tag = 4;
    someone = 1;
    local = 0;
    msg_poke__8Creaturei(object, 1);
    if (locals != 1 || shop_starts != 0 || cosines != 0 || scripts != 0) return 7;

    reset();
    shop_tag = 4;
    someone = 0;
    msg_poke__8Creaturei(object, 1);
    if (shops != 1 || scripts != 1 || cosines != 1) return 8;
    return 0;
}
