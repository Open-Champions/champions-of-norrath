// Observed: CopySprite__7VISceneii at 0x01144b30, 88 bytes, in the pinned SLUS-20565 image. Arguments: the scene (S), a sprite index (I) and one more value (V).
// The sprite object P is the first word of record I of the 12-byte records addressed by the word at S+1004 (no range or null test). The word at P+60 points to a table;
// the 8-byte entry at table+72 holds a signed halfword adjustment A and a function word at +4. That function is called with six arguments:
// (P + A, S, word at S+19356, word at S+19360, word at S+19364, V), and its result is returned.
// Inferred: a virtual-style "copy" slot of the sprite class, handed the scene and the three scene context words. Unknown: names of those words and what V selects.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" void* norrath_host_function(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#define NORRATH_WORD_TO_FUNCTION(word) norrath_host_function(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_WORD_TO_FUNCTION(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int CopySprite__7VISceneii(unsigned char* self, int index, int value) {
    unsigned char* records = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 1004));
    unsigned char* sprite = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(records + index * 12));
    unsigned char* slot = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(sprite + 60)) + 72;
    typedef int (*Slot)(unsigned char*, unsigned char*, unsigned, unsigned, unsigned, int);
    Slot function = reinterpret_cast<Slot>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(slot + 4)));
    return function(sprite + *reinterpret_cast<short*>(slot), self, *reinterpret_cast<unsigned*>(self + 19356),
                    *reinterpret_cast<unsigned*>(self + 19360), *reinterpret_cast<unsigned*>(self + 19364), value);
}
}
