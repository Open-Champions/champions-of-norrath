// The kernel break helper reads and writes one word through this name:
// the old break is returned and the word advances by the increment unless
// the end limit is crossed. No ELF symbol names this word; the nearby
// heap_ptr.39 at 0x444954 is a different static and is deliberately not
// aliased here.
// Inferred: a zero-initialized word, matching the nobits convention for
// words the image never initializes. The first break call establishes the
// real value at runtime.
// Unknown: the original section and address of this word, and its value
// before the first break call.
extern "C" {
unsigned char norrath_player_heap_pointer_word[4] = {};
}
