// Observed: the loader and the heap report a failure by storing a word to absolute
// address 0 (sw zero,0(zero) at 0x00165d04 in lumpLoad; cdLoad stores 2 there the same way;
// memAlloc stores 0 on exhaustion). There is no ELF object at that address.
// Authored: a real word of storage under the import name, replacing the absolute-symbol
// unit level-anim-load-loader-failure-word, which the build recipe cannot accept (the link
// imports no absolute game addresses and applies no aliases). Deliberate difference: the
// stores land in this object instead of at address 0, so a debugger watching address 0
// would not see them. Nothing in the reconstructed sources reads the word back.
// Unknown: whether the original code ever reads address 0 after these stores. Plan only
// one of this unit and the absolute-symbol unit.
extern "C" {
unsigned norrath_loader_failure_word_view = 0u;
}
