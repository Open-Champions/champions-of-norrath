// Observed Add__12VIDictionaryR10VIResourceUi14VIResourceTypei at 0x010bf2a0, 60 bytes, 48-byte frame. With
// (dictionary, resource, id, type, handle) it stores id into the first word of the resource, then calls
// Insert__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUiRCQ212VIDictionary11VIDictEntry(dictionary, &key, &entry) where
// key is a local word holding id and entry is an eight-byte local whose first word is type and second word is handle.
// The result is -1 when Insert returned a negative value and 0 otherwise.
// Inferred: the dictionary is the map from resource id to a (type, handle) entry that FindTyped reads back.
// Unknown: the meaning of a negative result from Insert, and whether the resource word at +0 is documented elsewhere.
extern "C" {
int Insert__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUiRCQ212VIDictionary11VIDictEntry(void* map, const unsigned* key,
                                                                                       const void* entry);

int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type,
                                                         int handle) {
    struct Entry {
        int type;
        int handle;
    };
    unsigned key = id;
    Entry entry;
    *static_cast<unsigned*>(resource) = id;
    entry.type = type;
    entry.handle = handle;
    int result = Insert__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUiRCQ212VIDictionary11VIDictEntry(dictionary, &key, &entry);
    return result < 0 ? -1 : 0;
}
}
