// Observed heap_ptr.39 at 0x00444954, one 4-byte word in initialised data. The
// file bytes are 0x008ec91c, which is the address of the absolute symbols
// `end` and `_end` in the image. The authored allocator code names this word
// norrath_player_heap_pointer_word and treats it as the program break. It is
// initialised here from `_end`; `_end` is supplied by the link layout, not here.
// Unknown: which of `end` and `_end` the image's initialiser used.
extern "C" unsigned char _end[];
extern "C" unsigned char* norrath_player_heap_pointer_word;
unsigned char* norrath_player_heap_pointer_word = _end;
