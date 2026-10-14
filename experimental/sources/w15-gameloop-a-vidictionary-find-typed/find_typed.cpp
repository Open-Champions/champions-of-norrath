#include "../w9-animation-object-c-common/mem.hpp"
// Observed FindTyped__12VIDictionaryUi14VIResourceTypeRi at 0x010bf370, 148 bytes, 64-byte frame. With
// (dictionary, id, type, out) it calls Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(dictionary, &key) with a
// local word key = id, then End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(dictionary). If the two results are equal
// it returns 0. Otherwise the found index selects a 28-byte node of the array whose pointer is the word at
// dictionary + 8; the node's word at +20 is compared with type and, when equal, its word at +24 is stored through out
// and 1 is returned; if it differs 0 is returned and out is not written.
// Inferred: a (type, handle) entry sits at node + 20 and the lookup succeeds only for the requested resource type.
// Unknown: the layout of the nodes before +20 (the key and tree links) and of the dictionary beyond the word at +8.
extern "C" {
int Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(void* map, const unsigned* key);
int End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* map);

int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* dictionary, unsigned id, int type, int* out) {
    using namespace norrath_aoc;
    unsigned key = id;
    int found = Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(dictionary, &key);
    if (found == End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(dictionary)) return 0;
    const unsigned char* entry = load_pointer(static_cast<unsigned char*>(dictionary) + 8) + found * 28 + 20;
    if (static_cast<int>(load_word(entry)) != type) return 0;
    *out = static_cast<int>(load_word(entry + 4));
    return 1;
}
}
