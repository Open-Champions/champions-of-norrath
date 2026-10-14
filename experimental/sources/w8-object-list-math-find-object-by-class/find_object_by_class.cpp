// Observed objectFindObjectByClass__FPcPP10GameObjecti at 0x1407d8, extent 676.
// Collects the objects whose class name equals the given text. Arguments: the
// class name, an output array, and a limit. The walk visits, in this order, the
// run list (runList), the fifteen medium run-list heads, then the sixty slow
// run-list heads, each with a ListSafeIterator whose mask is objectIgnoreFlags.
// For each object the class name is strcmp(typeid-name, text) == 0, where the
// type record comes from the function at word 1 of the object's virtual table
// (+72); a null payload would call __throw_bad_typeid (the iterator never returns
// one). Each match is stored at out[count] and count grows; the walk ends early
// when count equals limit - 1 (an equality test, not a bound: a limit of 0 or 1
// never stops it). The iterator is destroyed with flags 2 on every exit. The
// array is terminated with a zero at out[count]. The original leaves the address
// of that terminator in v0; callers treat the function as returning nothing.
#include "../w8-object-list-math-common/safe_iterator.hpp"

extern "C" int strcmp(const char*, const char*);
extern "C" void __throw_bad_typeid();
extern "C" void __16ListSafeIteratorP8ListHeadi(ListSafeIterator*, ListHead*, int);
extern "C" void next__16ListSafeIterator(ListSafeIterator*);
extern "C" void _$_16ListSafeIterator(ListSafeIterator*, int);
extern "C" int objectIgnoreFlags;
struct HeadSlot {
    ListHead head;
    unsigned unobserved_word;
};
extern "C" HeadSlot runList;
extern "C" ListHead mediumRunList[16];
extern "C" ListHead slowRunList[60];

namespace {
bool class_matches(const GameObjectVirtual* object, const char* class_name) {
    if (object == nullptr) __throw_bad_typeid();
    const TypeRecordGetter getter = reinterpret_cast<TypeRecordGetter>(const_cast<void*>(object->vtable[1]));
    return strcmp(getter()->name, class_name) == 0;
}

// Returns true when the output limit was reached.
bool collect_from(ListHead* head, const char* class_name, GameObjectVirtual** out, int limit, int& count) {
    ListSafeIterator iterator;
    __16ListSafeIteratorP8ListHeadi(&iterator, head, objectIgnoreFlags);
    for (ListNode* node = iterator.link.prev; node != nullptr; node = iterator.link.prev) {
        GameObjectVirtual* object = static_cast<GameObjectVirtual*>(node->value);
        if (class_matches(object, class_name)) {
            out[count++] = object;
            if (count == limit - 1) {
                _$_16ListSafeIterator(&iterator, 2);
                return true;
            }
        }
        next__16ListSafeIterator(&iterator);
    }
    _$_16ListSafeIterator(&iterator, 2);
    return false;
}
}  // namespace

extern "C" void objectFindObjectByClass__FPcPP10GameObjecti(char* class_name, GameObjectVirtual** out, int limit) {
    int count = 0;
    bool done = collect_from(&runList.head, class_name, out, limit, count);
    for (int i = 0; !done && i < 15; ++i) done = collect_from(&mediumRunList[i], class_name, out, limit, count);
    for (int i = 0; !done && i < 60; ++i) done = collect_from(&slowRunList[i], class_name, out, limit, count);
    out[count] = nullptr;
}
