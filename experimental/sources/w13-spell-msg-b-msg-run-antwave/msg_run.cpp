// Observed msg_run__7AntWave at 0x002085e8, 332 bytes. O is the object, T the word at O+968 (an object holding, at its
// +12, the target), n the signed word at O+808 and "entry k" the entry at table byte k of the table whose address is the
// word at an object's +72, called on that object (adjusted by the entry's signed halfword). Steps in order:
//  1. everyFrame__8CreatureP12_modelHeader(O, null) runs (its result is ignored).
//  2. When T is zero: entry 8 of O is called with 3, and that ends it.
//  3. Otherwise persue2__8CreatureP10GameObjectfff(O, the word at T+12, 500.0, 1.0, 0.0) runs. When n has remainder zero
//     by 60 (signed) and the squared distance from O+16 to the position at (word at T+12) + 16 is below the square of
//     the float at O+28, entry 16 of the target is called on it with (1.0, O, 256) (that slot takes a float amount,
//     a source object and flags, as in the other hurt calls). Then, when the signed word at O+964 is below n (read
//     again), entry 8 of O is called with 3.
// The image tests O for null before each entry 8 call; that test cannot be true here and is not reproduced.
// Inferred: a swarm that chases a target and hurts it periodically, and removes itself when its time is up (entry 8
// with 3 is not named here). Unknown: what the words at +964 and +968 are exactly. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
void persue2__8CreatureP10GameObjectfff(void* self, void* target, float a, float b, float c);

void msg_run__7AntWave(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    everyFrame__8CreatureP12_modelHeader(o, nullptr);
    if (word(o, 968) == 0u) {
        call_entry(o, 8, 3);
        return;
    }
    persue2__8CreatureP10GameObjectfff(o, from_word(word(from_word(word(o, 968)), 12)), bits(0x43fa0000u), 1.0f, 0.0f);
    if (sword(o, 808) % 60 == 0) {
        unsigned char* const target = from_word(word(from_word(word(o, 968)), 12));
        const float dx = real(target, 16) - real(o, 16);
        const float dy = real(target, 20) - real(o, 20);
        const float dz = real(target, 24) - real(o, 24);
        const float r = real(o, 28);
        if (dx * dx + dy * dy + dz * dz < r * r) call_hurt(target, 1.0f, o, 256);
    }
    if (sword(o, 964) < sword(o, 808)) call_entry(o, 8, 3);
}
}
