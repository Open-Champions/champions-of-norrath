// Observed: Clear__12VIDictionary at 0x010bf288 is 20 bytes. It forwards its
// object pointer unchanged to Clear__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry, the
// VIMap<unsigned, VIDictionary::VIDictEntry> member, as a tail call.
// Inferred: VIDictionary derives from or holds that map at offset 0, because
// the same pointer is passed; the mangled template name is the only evidence.
extern "C" {
void Clear__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* map);

void Clear__12VIDictionary(void* object) {
    Clear__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(object);
}
}
