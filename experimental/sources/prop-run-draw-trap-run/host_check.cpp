#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include "msg_run.cpp"
using namespace vslots_b;
static int failures = 0, find_calls, find_count, attack_calls;
static float seen_low[3], seen_high[3];
static void* list_to_return[8];
static void* attack_target[8];
static void* attack_source[8];
static float attack_a[8], attack_b[8];
static int attack_ints[8][4];
static void* attack_desc[8];
static bool attack_flag[8];
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* low, const void* high, void** objects, int count) {
    ++find_calls; find_count = count;
    std::memcpy(seen_low, low, 12); std::memcpy(seen_high, high, 12);
    for (int i = 0; i < 8; ++i) objects[i] = list_to_return[i];
    // The reference leaves later list words as the callee wrote them; terminate defensively.
    objects[8] = nullptr;
}
extern "C" void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
    void* target, void* source, float first, float second, int a, int b, int c, int d, void* description, bool flag) {
    const int i = attack_calls++;
    if (i >= 8) return;
    attack_target[i] = target; attack_source[i] = source; attack_a[i] = first; attack_b[i] = second;
    attack_ints[i][0] = a; attack_ints[i][1] = b; attack_ints[i][2] = c; attack_ints[i][3] = d;
    attack_desc[i] = description; attack_flag[i] = flag;
}
static unsigned char object[200], p0[1000], p1[1000], p2[1000], p3[1000], npc[1000], busy[1000];
static void setup(unsigned char* o, unsigned flags, unsigned index) {
    std::memset(o, 0, 1000);
    write_word(address(o) + 12u, flags);
    write_word(address(o) + 960u, index);
}
static void fresh() {
    std::memset(object, 0, sizeof object);
    write_single(address(object) + 16u, 10.0f);
    write_single(address(object) + 20u, 20.0f);
    write_single(address(object) + 24u, 30.0f);
    write_single(address(object) + 28u, 2.5f);
    find_calls = attack_calls = 0;
    std::memset(list_to_return, 0, sizeof list_to_return);
    setup(p0, 0x40u, 0u); setup(p1, 0x40u, 1u); setup(p2, 0x40u, 2u); setup(p3, 0x40u, 3u);
}
static unsigned char seen_byte(unsigned i) { return object[76 + i]; }
int main() {
    fresh();
    msg_run__4Trap(object);
    expect(find_calls == 1 && find_count == 128, "one box query for 128 entries");
    expect(seen_low[0] == 7.5f && seen_low[1] == 17.5f && seen_low[2] == 27.5f, "low corner is point - extent");
    expect(seen_high[0] == 12.5f && seen_high[1] == 22.5f && seen_high[2] == 32.5f, "high corner is point + extent");
    expect(attack_calls == 0 && seen_byte(0) == 0 && seen_byte(3) == 0, "empty list: seen bytes stay clear");

    fresh();
    list_to_return[0] = p1; list_to_return[1] = p3;
    msg_run__4Trap(object);
    expect(attack_calls == 2 && attack_target[0] == p1 && attack_target[1] == p3, "each new player is attacked in list order");
    expect(attack_source[0] == object && attack_a[0] == 20.0f && attack_b[0] == 30.0f, "source, 20.0, 30.0");
    expect(attack_ints[0][0] == 10000 && attack_ints[0][1] == 0 && attack_ints[0][2] == 0 && attack_ints[0][3] == 0 && attack_desc[0] == nullptr && !attack_flag[0], "remaining arguments");
    expect(seen_byte(0) == 0 && seen_byte(1) == 1 && seen_byte(2) == 0 && seen_byte(3) == 1, "seen bytes copied to +76..+79");

    // A player already marked last frame is not attacked again.
    fresh();
    object[76 + 1] = 1;
    list_to_return[0] = p1; list_to_return[1] = p2;
    msg_run__4Trap(object);
    expect(attack_calls == 1 && attack_target[0] == p2, "previous seen byte suppresses the attack");
    expect(seen_byte(1) == 1 && seen_byte(2) == 1, "both are seen now");
    object[76 + 1] = 0x80;  // a negative signed byte is non-zero
    fresh();
    object[76 + 3] = 0x80;
    list_to_return[0] = p3;
    msg_run__4Trap(object);
    expect(attack_calls == 0 && seen_byte(3) == 1, "any non-zero previous byte suppresses");

    // A player that left clears its byte.
    fresh();
    object[76 + 0] = 1; object[76 + 2] = 1;
    list_to_return[0] = p2;
    msg_run__4Trap(object);
    expect(seen_byte(0) == 0 && seen_byte(2) == 1 && attack_calls == 0, "absent players clear");

    // Flag filter: 0x40 set and 0x08 clear.
    fresh();
    setup(npc, 0x00u, 0u); setup(busy, 0x48u, 1u);
    unsigned char other[1000];
    setup(other, 0x4000u | 0x40u | 0x17u, 2u);
    list_to_return[0] = npc; list_to_return[1] = busy; list_to_return[2] = other;
    msg_run__4Trap(object);
    expect(attack_calls == 1 && attack_target[0] == other && seen_byte(0) == 0 && seen_byte(1) == 0 && seen_byte(2) == 1, "only 0x40 without 0x08 counts; other bits ignored");
    fresh();
    setup(npc, 0x08u, 0u);
    list_to_return[0] = npc;
    msg_run__4Trap(object);
    expect(attack_calls == 0 && seen_byte(0) == 0, "0x08 alone does not count");
    fresh();
    setup(npc, 0x48u, 0u);
    list_to_return[0] = npc;
    msg_run__4Trap(object);
    expect(attack_calls == 0 && seen_byte(0) == 0, "0x48 does not count");

    // The list stops at the first null even if later entries exist.
    fresh();
    list_to_return[0] = p0; list_to_return[2] = p1;
    msg_run__4Trap(object);
    expect(attack_calls == 1 && seen_byte(1) == 0, "stops at the first null");
    // Same player twice is attacked once; the second visit sees the previous byte unchanged (still 0), so both attack.
    fresh();
    list_to_return[0] = p0; list_to_return[1] = p0;
    msg_run__4Trap(object);
    expect(attack_calls == 2 && seen_byte(0) == 1, "the previous-frame byte, not the new one, gates the attack");
    return failures == 0 ? 0 : 1;
}
