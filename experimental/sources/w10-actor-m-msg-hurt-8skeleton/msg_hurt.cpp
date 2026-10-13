#include "../w10-actor-m-common/access.hpp"
// Observed msg_hurt__8SkeletonfP10GameObjecti at 0x2470c8, 148 bytes. Arguments: object, float amount,
// source object, int. Nothing is returned. With a null source nothing happens. Otherwise the function
// word at byte 4 of the source's table (word at source+72, slot 0) is called with the first argument
// left as the receiving object (not the source), and its result is compared with the result of
// __tf8Skeleton() through __eq__C9type_infoRC9type_info. Only when that comparison does not return 1
// does msg_hurt__8CreaturefP10GameObjecti run with the same four arguments. Inferred: slot 0 is the
// type-info getter, which the Skeleton table view at 0x44fee8 confirms (its slot 0 is __tf8Skeleton), so
// a Skeleton is not hurt by another object of exactly its own type. Unknown: the int argument.
extern "C" {
void* __tf8Skeleton();
int __eq__C9type_infoRC9type_info(const void* a, const void* b);
void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, void* source, int flag);
}

extern "C" void msg_hurt__8SkeletonfP10GameObjecti(void* self, float amount, void* source, int flag) {
    unsigned char* s = static_cast<unsigned char*>(source);
    if (s == nullptr) return;
    const unsigned type_word = w10m::call_entry_plain(s, 0, self);
    const void* other_type = reinterpret_cast<const void*>(static_cast<__UINTPTR_TYPE__>(type_word));
    const int same = __eq__C9type_infoRC9type_info(other_type, __tf8Skeleton());
    if ((same ^ 1) != 0) msg_hurt__8CreaturefP10GameObjecti(self, amount, source, flag);
}
