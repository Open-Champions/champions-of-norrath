// Observed updateEnemy__8Innoruuk at 0x001fc5b8, 772 bytes.
// Chooses the object kept in the list head at o+576 (o is the Innoruuk object). "accept(x)" below is
// the virtual call through the object's table: the halfword at table+160 is added to o and the
// function word at table+164 is called with that pointer and x; a nonzero result accepts x.
// A "list target" is the word at (the word at a head)+12.
//  1. A signed byte at o+570 that is nonzero returns at once.
//  2. s starts null. When the head at o+576 is set and accept(its target) holds, s is that target.
//  3. When s is still null and the head at o+752 is set and accept(its target) holds: t is that
//     target and, with q = t+16 and d = the position at o+776 minus q, s becomes t when
//     (float at o+788)^2 * 0.707 < (d.x^2 + d.y^2) + d.z^2.
//  4. A search runs when s is null, or when ((word at o+808) xor (signed byte at o+569)) & 31 is
//     zero. The box is the position at o+776 plus and minus r = (float at o+788) * 0.707 on each
//     axis; objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 128) fills a null-ended
//     list of objects. Starting with best distance (float at o+788)^2 and best score 0, each
//     listed object c that accept(c) takes is scored: the accept result, plus 1 when its squared
//     distance d from the position at o+16 is below the best distance, plus 1 when c is the
//     target of the head at o+752, plus 1 when c is the target of the head at o+576. A candidate
//     wins when its score is higher, or equal with a smaller distance; the winner becomes s and
//     sets the best score and distance.
//  5. When s is null, or the head at o+576 is set to a different target than s, the head at o+576
//     is destroyed with listDestroy__FP8ListHead and its first word is cleared. Then, when s is
//     not null and the head's first word is zero, listOnewayLink__FP8ListHeadT0Pv(o+576, s, s)
//     links it.
// Inferred: this picks a target for the enemy; the heads at +576 and +752 hold the current and a
// preferred target. Unknown: what the bytes at +569, +570 and the word at +808 mean (the latter
// seems to be a frame counter and the byte a per-object phase). Nothing here ran on the EE.
#include "../w10-actor-h-common/words.hpp"

extern "C" {
void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* low, const void* high, void** out, int capacity);
void listDestroy__FP8ListHead(void* head);
void* listOnewayLink__FP8ListHeadT0Pv(void* first_head, void* second_head, void* value);
}

namespace {
// The target held by a head: the word at (the word at the head) + 12, or null when the head is empty.
unsigned char* head_node(unsigned char* object, unsigned offset) {
    return w10h::from_word(w10h::word_at(object, offset));
}

unsigned char* target_of(unsigned char* node) {
    return w10h::from_word(*reinterpret_cast<unsigned*>(node + 12));
}

int accept(unsigned char* object, unsigned char* candidate) {
    typedef int (*Check)(void*, void*);
    return w10h::fn_from_word<Check>(w10h::entry_function(object, 160))(w10h::adjusted(object, 160), candidate);
}

float squared_distance(const unsigned char* from, const unsigned char* to) {
    const float* a = reinterpret_cast<const float*>(from);
    const float* b = reinterpret_cast<const float*>(to);
    float dx = a[0] - b[0];
    float dy = a[1] - b[1];
    float dz = a[2] - b[2];
    float sum = dx * dx + dy * dy;
    return sum + dz * dz;
}
}  // namespace

extern "C" void updateEnemy__8Innoruuk(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    if (static_cast<signed char>(o[570]) != 0) return;
    unsigned char* chosen = nullptr;
    if (w10h::word_at(self, 576) != 0) {
        if (accept(o, target_of(head_node(o, 576)))) chosen = target_of(head_node(o, 576));
    }
    if (chosen == nullptr) {
        if (w10h::word_at(self, 752) != 0 && accept(o, target_of(head_node(o, 752)))) {
            unsigned char* target = target_of(head_node(o, 752));
            const float range = *reinterpret_cast<float*>(o + 788);
            const float limit = (range * range) * 0x1.69fbe6p-1f;
            if (limit < squared_distance(o + 776, target + 16)) chosen = target;
        }
    }
    if (chosen == nullptr || (((w10h::word_at(self, 808) ^ static_cast<unsigned>(static_cast<int>(static_cast<signed char>(o[569])))) & 31u) == 0)) {
        const float range = *reinterpret_cast<float*>(o + 788);
        const float radius = range * 0x1.69fbe6p-1f;
        const float* position = reinterpret_cast<const float*>(o + 776);
        float low[3] = {position[0] - radius, position[1] - radius, position[2] - radius};
        float high[3] = {position[0] + radius, position[1] + radius, position[2] + radius};
        void* found[128];
        objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 128);
        if (found[0] != nullptr) {
            float best_distance = range * range;
            int best_score = 0;
            for (void** cursor = found; *cursor != nullptr; ++cursor) {
                unsigned char* candidate = static_cast<unsigned char*>(*cursor);
                int score = accept(o, candidate);
                if (score == 0) continue;
                const float distance = squared_distance(o + 16, candidate + 16);
                if (distance < best_distance) score = score + 1;
                if (w10h::word_at(self, 752) != 0 && candidate == target_of(head_node(o, 752))) score = score + 1;
                if (w10h::word_at(self, 576) != 0 && candidate == target_of(head_node(o, 576))) score = score + 1;
                const bool better = best_score < score || (score == best_score && distance < best_distance);
                if (better) {
                    chosen = candidate;
                    best_distance = distance;
                    best_score = score;
                }
            }
        }
    }
    if (chosen == nullptr || (w10h::word_at(self, 576) != 0 && target_of(head_node(o, 576)) != chosen)) {
        listDestroy__FP8ListHead(o + 576);
        w10h::word_at(self, 576) = 0;
    }
    if (chosen == nullptr) return;
    if (w10h::word_at(self, 576) != 0) return;
    listOnewayLink__FP8ListHeadT0Pv(o + 576, chosen, chosen);
}
