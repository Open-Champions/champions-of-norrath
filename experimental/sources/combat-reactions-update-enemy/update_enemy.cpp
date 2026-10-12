#include "../combat-reactions-bytes/bytes.hpp"
// Observed updateEnemy__8Creature at 0x10b558, 1212 bytes.
// Fields read: signed byte object+570, signed halfword +796, the 64-bit value at
// +160, the list head words at +576 and +752 (each node's word at +12 is the
// object it names), the home point at +776..+784, the single at +788, the
// word at +812, the signed byte at +569, the single at +28, the point at +16,
// and the word frameNm. Virtual slot byte 160 (isEnemy__8CreatureP10GameObject
// in the SmallSpider vtable) is called as slot(this, other).
// Flow: a nonzero signed byte at +570 returns. A nonzero halfword at +796, or
// bit 0x4000000 in the 64-bit value at +160, destroys the list at +576, clears
// its head word and returns. Otherwise a target starts as none.
// Current-target block: when the head word at +576 is non-null and isEnemy says
// yes for its object, that object becomes the target. Unless frameNm minus 480
// is below the word at +812, the target is dropped only when the squared leash
// (+788 squared) is below the target's squared distance to home, the creature's
// own squared distance to home is below that distance, and the target's single
// at +28 plus this creature's +28 plus 50.0 is below the creature's squared
// distance to the target.
// Attacker block: runs only when no target is held. The head word at +752 is
// treated the same way, except that the target is taken only when 0.707 times the
// squared leash is below the attacker's squared distance to home, and the
// leash-squared test is also made before the drop test.
// Search: runs when there is no target, or when the low five bits of frameNm
// xor the signed byte at +569 are zero. A box centred on the home point with
// half-extent leash * 0.707 is passed to objectFindInBox with a 128-entry
// output array. Each listed object for which isEnemy answers nonzero scores
// the answer plus one when its squared distance to the creature is below the
// best so far (starting at leash squared), plus one when it is the object of
// the +752 head, plus one when it is the object of the +576 head. A higher
// score wins, or an equal score with a smaller distance, and the winner
// replaces the target.
// End: with no target the list at +576 is destroyed and its head cleared.
// With a target, the list is destroyed and cleared when its head names another
// object; then, if the head is empty, the target is linked with listOnewayLink.
// Inferred: norrath_list_destroy is listDestroy, norrath_list_oneway_link is
// listOnewayLink, and the constant 0x3f34fdf3 appears twice in .lit4.
// Unknown: which field is the leash, and the meaning of the fields at +569,
// +812 and +160, and of bit 0x4000000.
extern "C" unsigned frameNm;
extern "C" void norrath_list_destroy(void* head);
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value);
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* minimum, const void* maximum, void** output, int capacity);

namespace {
using namespace combat_reactions;
inline float squares(float a, float b, float c) { return (a * a + b * b) + c * c; }
inline Address object_of(unsigned bits) { return address(pointer(bits)); }
// Word at +12 of the node whose bits are given; zero when there is no node.
inline unsigned node_object(unsigned node_bits) { return read_word(object_of(node_bits) + 12u); }
inline int is_enemy(void* self, unsigned other_bits) {
    void* adjusted = nullptr;
    using Call = int (*)(void*, void*);
    Call call = slot_function<Call>(self, 160u, &adjusted);
    return call(adjusted, pointer(other_bits));
}
struct Reach {
    float home_distance;      // squared distance from the home point to the object
    float own_distance;       // squared distance from the creature to the object
    float own_home_distance;  // squared distance from the creature to the home point
};
__attribute__((always_inline)) inline Reach measure(Address base, Address other) {
    const float x = read_single(base + 16u), y = read_single(base + 20u), z = read_single(base + 24u);
    const float hx = read_single(base + 776u), hy = read_single(base + 780u), hz = read_single(base + 784u);
    const float tx = read_single(other + 16u), ty = read_single(other + 20u), tz = read_single(other + 24u);
    Reach r;
    r.home_distance = squares(hx - tx, hy - ty, hz - tz);
    r.own_distance = squares(tx - x, ty - y, tz - z);
    r.own_home_distance = squares(x - hx, y - hy, z - hz);
    return r;
}
}

extern "C" void updateEnemy__8Creature(void* self) {
    const Address base = address(self);
    if (static_cast<signed char>(read_u8(base + 570u)) != 0) return;
    void* const list = static_cast<unsigned char*>(self) + 576u;
    if (read_i16(base + 796u) != 0 || (read_word(base + 160u) & 0x4000000u) != 0u) {
        norrath_list_destroy(list);
        write_word(base + 576u, 0u);
        return;
    }
    unsigned target = 0u;
    const float leash = read_single(base + 788u);
    const float leash_squared = leash * leash;
    const bool hurt_recently = signed_word(frameNm) - 480 < signed_word(read_word(base + 812u));
    if (read_word(base + 576u) != 0u && is_enemy(self, node_object(read_word(base + 576u))) != 0) {
        target = node_object(read_word(base + 576u));
        const Address other = object_of(target);
        const Reach r = measure(base, other);
        if (!hurt_recently && leash_squared < r.home_distance && r.own_home_distance < r.home_distance &&
            (read_single(other + 28u) + read_single(base + 28u)) + 50.0f < r.own_distance)
            target = 0u;
    }
    if (target == 0u && read_word(base + 752u) != 0u && is_enemy(self, node_object(read_word(base + 752u))) != 0) {
        const unsigned candidate = node_object(read_word(base + 752u));
        const Address other = object_of(candidate);
        const Reach r = measure(base, other);
        if (leash_squared * single_from_bits(0x3f34fdf3u) < r.home_distance) {
            target = candidate;
            if (!hurt_recently && leash_squared < r.home_distance && r.own_home_distance < r.home_distance &&
                (read_single(other + 28u) + read_single(base + 28u)) + 50.0f < r.own_distance)
                target = 0u;
        }
    }
    if (target == 0u || ((frameNm ^ static_cast<unsigned>(static_cast<signed char>(read_u8(base + 569u)))) & 0x1fu) == 0u) {
        const float reach = leash * single_from_bits(0x3f34fdf3u);
        const float hx = read_single(base + 776u), hy = read_single(base + 780u), hz = read_single(base + 784u);
        unsigned char low[16];
        unsigned char high[16];
        write_single(address(low), hx - reach);
        write_single(address(low) + 4u, hy - reach);
        write_single(address(low) + 8u, hz - reach);
        write_single(address(high), hx + reach);
        write_single(address(high) + 4u, hy + reach);
        write_single(address(high) + 8u, hz + reach);
        void* found[128];
        found[0] = nullptr;
        objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 128);
        int best_score = 0;
        float best_distance = leash_squared;
        if (found[0] != nullptr) {
            for (unsigned index = 0u; found[index] != nullptr; ++index) {
                const unsigned candidate = address_bits(found[index]);
                int score = is_enemy(self, candidate);
                if (score == 0) continue;
                const Address other = object_of(candidate);
                const float distance = squares(read_single(base + 16u) - read_single(other + 16u),
                                               read_single(base + 20u) - read_single(other + 20u),
                                               read_single(base + 24u) - read_single(other + 24u));
                if (distance < best_distance) score += 1;
                if (read_word(base + 752u) != 0u && candidate == node_object(read_word(base + 752u))) score += 1;
                if (read_word(base + 576u) != 0u && candidate == node_object(read_word(base + 576u))) score += 1;
                if (best_score < score || (score == best_score && distance < best_distance)) {
                    target = candidate;
                    best_distance = distance;
                    best_score = score;
                }
            }
        }
    }
    if (target == 0u || (read_word(base + 576u) != 0u && node_object(read_word(base + 576u)) != target)) {
        norrath_list_destroy(list);
        write_word(base + 576u, 0u);
    }
    if (target != 0u && read_word(base + 576u) == 0u)
        norrath_list_oneway_link(list, pointer(target), pointer(target));
}
