// Observed msg_collision__5GnomeP10GameObject at 0x00202488, 256 bytes.
// When the signed byte cutMode is zero, the word at +972 is below the word at +808 (signed) and the
// other object is non-null, the other object's type node (class table word +4, a no-argument
// function) is compared with __tf5Gnome(). If equal and the other object's +972 is also below its
// +808 (signed): the list head at +968 is destroyed (listDestroy__FP8ListHead) and re-linked to
// the other object (listOnewayLink__FP8ListHeadT0Pv with the other object as both remaining
// arguments), the table entry at (word +72) + 184 is called with the adjusted object and 13, and
// word +972 becomes word(+808) + 180 + eRand() % 180 (unsigned remainder). In every case the
// Creature collision routine is then called with both arguments and its result returned.
// Inferred: +968 holds a link to the object being followed. Unknown: meaning of +972/+808.
// cutMode is an unresolved import. Nothing here ran on the EE.
#include "../w10-actor-g-common/words.hpp"

extern "C" {
extern signed char cutMode;
void* __tf5Gnome();
int __eq__C9type_infoRC9type_info(const void* a, const void* b);
void listDestroy__FP8ListHead(void* head);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* other, void* data);
unsigned eRand__10GameObject(void* self);
int msg_collision__8CreatureP10GameObject(void* self, void* other);
}

extern "C" int msg_collision__5GnomeP10GameObject(void* self, void* other) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (cutMode == 0) {
        const int mine = *reinterpret_cast<int*>(object + 972);
        if (mine < *reinterpret_cast<int*>(object + 808) && other != nullptr) {
            const void* node = w10g::type_node_of(other);
            if (__eq__C9type_infoRC9type_info(node, __tf5Gnome())) {
                unsigned char* theirs = static_cast<unsigned char*>(other);
                if (*reinterpret_cast<int*>(theirs + 972) < *reinterpret_cast<int*>(theirs + 808)) {
                    void* list = object + 968;
                    listDestroy__FP8ListHead(list);
                    listOnewayLink__FP8ListHeadT0Pv(list, other, other);
                    w10g::fn_from_word<void (*)(void*, int)>(w10g::entry_function(self, 184))(w10g::adjusted(self, 184), 13);
                    const unsigned roll = eRand__10GameObject(self);
                    *reinterpret_cast<unsigned*>(object + 972) = *reinterpret_cast<unsigned*>(object + 808) + roll % 180u + 180u;
                }
            }
        }
    }
    return msg_collision__8CreatureP10GameObject(self, other);
}
