// Observed: the loader and the heap report a failure by storing a word to absolute
// address 0 (sw zero,0(zero) at 0x00165d04 in lumpLoad; cdLoad stores 2 there the same way;
// memAlloc stores 0 on exhaustion). There is no ELF object at that address.
// Inferred: address 0 is used as a scratch word that a debugger or watcher can read; the
// stores are not reads of any later state.
// Authored: the import norrath_loader_failure_word_view is satisfied by an absolute symbol
// whose value is 0, so every store resolves to address 0 as in the image, instead of to a
// separate object that would silently change where the word lives. The symbol is not
// attached to a section and is not an alias of any other symbol.
// Unknown: whether anything reads the word back; nothing in the reconstructed sources does.
#if defined(__mips__)
asm(".globl norrath_loader_failure_word_view\n"
    ".set norrath_loader_failure_word_view, 0\n");
#endif
