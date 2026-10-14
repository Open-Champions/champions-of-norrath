// Observed: Remove__12VIDictionaryR10VIResource at 0x010bf408 is 100 bytes. Arguments:
// dictionary (a0) and a resource reference (a1). It calls
// Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(dictionary, resource) and then
// End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(dictionary). If the two results are equal
// it returns -1. Otherwise it calls
// Erase__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(dictionary, found) and returns -1 if
// that is negative, else 0. The resource pointer is passed on unchanged as the key
// reference of Find.
// Inferred: the dictionary is a map from an unsigned key to an entry, the key being at the
// start of the resource; removal fails with -1 when the key is absent.
// Unknown: the key's location inside the resource beyond "passed through", and the types
// of the iterator values (treated as plain integers here).
extern "C" {
int Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(void* map, const void* key);
int End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* map);
int Erase__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void* map, int it);

int Remove__12VIDictionaryR10VIResource(void* dictionary, void* resource) {
    int found = Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(dictionary, resource);
    if (found == End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(dictionary)) {
        return -1;
    }
    return Erase__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(dictionary, found) < 0 ? -1 : 0;
}
}
