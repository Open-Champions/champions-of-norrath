// Observed: __12VIDictionary at 0x010bf1c8 is 20 bytes. It forwards its object
// pointer unchanged to __t5VIMap2ZUiZQ212VIDictionary11VIDictEntry (the
// VIMap<unsigned, VIDictionary::VIDictEntry> constructor) as a tail call.
// Inferred: the map sits at offset 0 of VIDictionary, from the shared pointer.
extern "C" {
void __t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* map);

void __12VIDictionary(void* object) {
    __t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(object);
}
}
