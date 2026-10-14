// Observed __new_handler at 0x0044818c, 4 bytes of initialized data. The image holds zero there.
// __builtin_new reads this word on every failed allocation: null raises bad_alloc, anything else
// is called as a function with no arguments. Inferred only: that this is the installed new
// handler, from the symbol name and that use. Unknown: whether the original object was
// emitted in .data or .bss (the image lists it with the initialized data); here it is a
// zero-initialized writable pointer word.
extern "C" {
void (*__new_handler)() = nullptr;
}
