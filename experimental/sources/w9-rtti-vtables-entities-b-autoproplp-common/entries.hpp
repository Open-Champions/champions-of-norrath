// Shared by msg_run__10AutoPropLP and msg_draw__10AutoPropLP. Both walk four pointer
// words at object+424 and, for each non-null entry that is "in use", run the same
// update on it before their own work. Observed layout (byte offsets):
//  - object+376: a signed word (call it the row base), object+380: a signed word;
//  - pairs of signed words, two per row, at object+456+8*m (low, high) and at
//    object+776+8*m, where m = 10 * (entry index) + row base;
//  - entry+80, +84, +88: three words; the entry is skipped when all three are zero;
//  - entry+92, +96, +252: words written here; entry+256: a light structure.
// Unknown: what the rows, the pairs and the entry fields mean.
#pragma once

#include "../w8-actor-virtuals-b-common/virtual_call.hpp"

extern "C" {
void lightActivate__FP5Light(void* light);
void lightDeactivate__FP5Light(void* light);
}

namespace autoproplp {

using w8avb::UWord;
using w8avb::Half;
typedef int SWord __attribute__((may_alias));

inline unsigned char* entry_at(unsigned char* object, int index) {
    unsigned word = *reinterpret_cast<UWord*>(object + 424 + 4 * index);
    return word == 0 ? 0 : const_cast<unsigned char*>(w8avb::address_of(word));
}

inline bool entry_idle(const unsigned char* entry) {
    return *reinterpret_cast<const UWord*>(entry + 80) == 0 && *reinterpret_cast<const UWord*>(entry + 84) == 0 &&
           *reinterpret_cast<const UWord*>(entry + 88) == 0;
}

// True when low/high bracket the reference word the way the image tests it: high < low,
// or low < reference < high.
inline bool outside_or_between(int low, int high, int reference) {
    return high < low || (low < reference && reference < high);
}

inline void update_entry(unsigned char* object, unsigned char* entry, int index) {
    const int row = 10 * index + *reinterpret_cast<SWord*>(object + 376);
    {
        const int low = *reinterpret_cast<SWord*>(object + 456 + 8 * row);
        const int high = *reinterpret_cast<SWord*>(object + 460 + 8 * row);
        if (outside_or_between(low, high, *reinterpret_cast<SWord*>(object + 380))) {
            *reinterpret_cast<UWord*>(entry + 96) = 0;
            *reinterpret_cast<UWord*>(entry + 92) = 1;
        } else {
            *reinterpret_cast<UWord*>(entry + 92) = 0;
        }
    }
    {
        const int low = *reinterpret_cast<SWord*>(object + 776 + 8 * row);
        const int high = *reinterpret_cast<SWord*>(object + 780 + 8 * row);
        if (outside_or_between(low, high, *reinterpret_cast<SWord*>(object + 380))) {
            if (*reinterpret_cast<UWord*>(entry + 252) == 0) *reinterpret_cast<UWord*>(entry + 252) = 1;
            if (*reinterpret_cast<UWord*>(entry + 84) != 0) lightActivate__FP5Light(entry + 256);
        } else {
            if (*reinterpret_cast<UWord*>(entry + 252) != 0) *reinterpret_cast<UWord*>(entry + 252) = 0;
            if (*reinterpret_cast<UWord*>(entry + 84) != 0) lightDeactivate__FP5Light(entry + 256);
        }
    }
}

}  // namespace autoproplp
