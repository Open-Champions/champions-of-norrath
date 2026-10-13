// Observed: _$_12VIDictionary at 0x010bf1e0 is 92 bytes and returns nothing.
// It calls Clear__12VIDictionary(this), then _$_t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(this, 2)
// (the map destructor with the not-in-charge flag value 2), then if bit 0 of
// the second argument is set tail-calls __builtin_delete(this).
extern "C" {
void Clear__12VIDictionary(void* object);
void _$_t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* map, int flags);
void __builtin_delete(void* pointer);

void _$_12VIDictionary(void* object, int flags) {
    Clear__12VIDictionary(object);
    _$_t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(object, 2);
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
