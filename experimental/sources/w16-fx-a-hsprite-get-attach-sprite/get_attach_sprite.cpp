// Observed: GetAttachSprite__C9VIHSpritei at 0x01063a48, 20 bytes, no calls. With (this, i) it reads the word at this+104, adds i * 16
// to it, and returns the word stored at that address. Nothing is written.
// Inferred: this+104 holds the base of an array of 16-byte attachment records and the first word of a record is what the weapon-trail caller uses as a scene sprite id (it passes it to GetSpriteType__7VIScenei). No bounds test
// is made on i.
// Unknown: the contents of the other 12 bytes of a record and the type of the count that bounds i elsewhere.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int GetAttachSprite__C9VIHSpritei(const unsigned char* self, int index) {
    unsigned base = *reinterpret_cast<const unsigned*>(self + 104);
    return *reinterpret_cast<const int*>(NORRATH_WORD_TO_POINTER(base + index * 16));
}
}
