// Observed findCommander__3Orc at 0x00229c20, 216 bytes. If the word at object+1128 is non-zero it first
// calls leaveCommander__3Orc(object). It then scans the 20 words of the static table _3Orc$m_commanderList
// (0x0042ce10, 0x50 bytes) in order. A zero entry is skipped. Otherwise word 12 of the entry is a
// candidate object; a candidate whose word at +12 has bit 3 (value 8) set is skipped. The squared
// distance between the positions at +16 of the object and of the candidate, summed x then y then z, is
// compared with the best so far, which starts at 360000.0; a strictly smaller value becomes the new best.
// Returns the nearest candidate or zero.
// Inferred: the table lists the Orcs that currently lead a group (the name and the join/leave callers
// suggest this); 360000 would be a squared radius of 600. Unknown: what bit 3 of the +12 word means.
// Nothing here ran on the EE.
#include "../w11-actor-b-common/access.hpp"

extern "C" {
void leaveCommander__3Orc(void* self);
extern unsigned _3Orc_commanderList[20] asm("_3Orc$m_commanderList");

void* findCommander__3Orc(void* self) {
    using namespace w11b;
    unsigned char* o = static_cast<unsigned char*>(self);
    if (word(o, 1128)) leaveCommander__3Orc(self);
    float best = 360000.0f;
    unsigned char* found = nullptr;
    const unsigned char* mine = o + 16;
    for (int i = 0; i < 20; ++i) {
        const unsigned entry = _3Orc_commanderList[i];
        if (!entry) continue;
        unsigned char* candidate = from_word(word(from_word(entry), 12));
        if (word(candidate, 12) & 8u) continue;
        const float* theirs = reinterpret_cast<const float*>(candidate + 16);
        const float* here = reinterpret_cast<const float*>(mine);
        float dx = here[0] - theirs[0];
        float dy = here[1] - theirs[1];
        float dz = here[2] - theirs[2];
        float d = dx * dx + dy * dy + dz * dz;
        if (d < best) {
            found = candidate;
            best = d;
        }
    }
    return found;
}
}
