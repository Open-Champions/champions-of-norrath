// Observed _GLOBAL_$I$animRemoveNonpermanentAnimations__FP14AnimationState at 0x00164180, 32 bytes. It calls a file-local helper with arguments 1 and 0xffff and returns.
// The file-local helper at 0x00163740 is 104 bytes. It returns at once unless its two arguments are 1 and 0xffff; for those it
// runs nested counting loops (150 passes of 64 passes) whose bodies are only zero words (no-operation instructions), writes no memory and
// calls nothing, then returns.
// The listed global constructor wrapper is therefore a function with no memory effect. It is reproduced as an
// empty function: a modern compiler removes both the helper and its empty loops, so the body here is smaller.
// Inferred: the helper is the compiler-generated static initialization of that source file; the loops look
//   like the inlined constructor of an array whose constructor has no effect.
// Unknown: which global objects the original file declared, and why the constructors were retained.
extern "C" void _GLOBAL_$I$animRemoveNonpermanentAnimations__FP14AnimationState() {}
