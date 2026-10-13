// Observed msg_moveCollision__5GnomePP10GameObject at 0x00202588, 236 bytes.
// Takes a null-terminated array of object pointers; only its first entry is used. When the word at
// +972 is below the word at +808 (signed), the first entry is non-null and its type node (class
// table word +4, no-argument function) equals __tf5Gnome(), and that object's signed halfword at
// +572 is 13: the list head at +968 is destroyed and re-linked to the first entry (listOnewayLink
// with the entry as both remaining arguments), the table entry at (word +72) + 184 is called with
// the adjusted object and 13, and if the list head's first word (the first link) is non-null,
// word +972 becomes the +972 word of the object stored at link+12, plus 60. In every case the
// Creature routine is then called with both arguments and its result returned.
// Inferred: link+12 holds the linked object. Unknown: meaning of the fields. Nothing here ran on
// the EE.
#include "../w10-actor-g-common/words.hpp"

extern "C" {
void* __tf5Gnome();
int __eq__C9type_infoRC9type_info(const void* a, const void* b);
void listDestroy__FP8ListHead(void* head);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* other, void* data);
int msg_moveCollision__8CreaturePP10GameObject(void* self, void** others);
}

extern "C" int msg_moveCollision__5GnomePP10GameObject(void* self, void** others) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (*reinterpret_cast<int*>(object + 972) < *reinterpret_cast<int*>(object + 808) && others[0] != nullptr) {
        const void* node = w10g::type_node_of(others[0]);
        if (__eq__C9type_infoRC9type_info(node, __tf5Gnome())) {
            if (*reinterpret_cast<short*>(static_cast<unsigned char*>(others[0]) + 572) == 13) {
                void* list = object + 968;
                listDestroy__FP8ListHead(list);
                listOnewayLink__FP8ListHeadT0Pv(list, others[0], others[0]);
                w10g::fn_from_word<void (*)(void*, int)>(w10g::entry_function(self, 184))(w10g::adjusted(self, 184), 13);
                const unsigned link = *reinterpret_cast<unsigned*>(list);
                if (link != 0) {
                    unsigned char* linked = w10g::from_word(*reinterpret_cast<unsigned*>(w10g::from_word(link) + 12));
                    *reinterpret_cast<unsigned*>(object + 972) = *reinterpret_cast<unsigned*>(linked + 972) + 60u;
                }
            }
        }
    }
    return msg_moveCollision__8CreaturePP10GameObject(self, others);
}
