// Observed: GetActorSpritePtr__C7VIScenei at 0x011457f0, 32 bytes. With the scene
// as this and an actor record address as the second argument: the word at actor +
// 96 is an index; the word at this + 1004 is the base of a table of 12-byte
// entries; the result is the first word of entry [index]. Nothing is written.
// Inferred: the mangled int argument is an address in the image (it is
// dereferenced), and the first word of each table entry is a sprite pointer.
// Unknown: the entry layout beyond the first word and the bound of the index.
extern "C" {
void* GetActorSpritePtr__C7VIScenei(const void* scene, const void* actor) {
    int index = *reinterpret_cast<const int*>(static_cast<const unsigned char*>(actor) + 96);
    const unsigned char* table =
        *reinterpret_cast<const unsigned char* const*>(static_cast<const unsigned char*>(scene) + 1004);
    return *reinterpret_cast<void* const*>(table + index * 12);
}
}
