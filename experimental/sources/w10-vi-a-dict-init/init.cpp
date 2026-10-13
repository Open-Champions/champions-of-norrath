// Observed: Init__12VIDictionaryi at 0x010bf240 is 44 bytes. It calls
// Init__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(this, count) on the VIMap<unsigned, VIDictionary::VIDictEntry> base
// and returns -1 if that result is negative, otherwise 0.
// Unknown: what the count argument means to the map.
extern "C" {
int Init__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void* map, int count);

int Init__12VIDictionaryi(void* object, int count) {
    return Init__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(object, count) < 0 ? -1 : 0;
}
}
