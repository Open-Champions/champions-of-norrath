// Selection, level and formatting words, the user filter word, and a float constant.
//   selectLumpName (0x00484548, 4 bytes, .sdata): a pointer, file-backed to a literal in
//     .rodata whose text is "select.lmp" (a lump file name).
//   selectMode (0x00486cf8), thisLevel (0x00486d94) and wasFormatting (0x00486c98): 4 bytes
//     each in .bss. The ELF size is 0; the next object starts 4 bytes later in each case, so
//     the size is inferred as one int.
//   stage (0x004848f4, 4 bytes, .sdata, file-backed zero): a pointer-sized word.
//   userFilterFunction (0x00483b00, 4 bytes, .sdata, file-backed zero): a function pointer
//     slot. norrath_restore_filter_word is the same address under an authored name; this
//     original-name definition exists because callers reference the original symbol.
//   scale (0x0046ccc0, 12 bytes, .rodata): the floats 0.9, 0.6 and 0.0 (bit patterns
//     0x3f666666, 0x3f199999, 0x00000000). Neighbours are other constant arrays.
// Unknown: what each of these words is for beyond their names.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
char* selectLumpName NORRATH_LOADED(".sdata") = const_cast<char*>("select.lmp");
int selectMode NORRATH_LOADED(".bss") = 0;
int thisLevel NORRATH_LOADED(".bss") = 0;
int wasFormatting NORRATH_LOADED(".bss") = 0;
void* stage NORRATH_LOADED(".sdata") = 0;
void (*userFilterFunction)(void*) NORRATH_LOADED(".sdata") = 0;
extern const float scale[3] NORRATH_LOADED(".rodata") = {
    __builtin_bit_cast(float, 0x3f666666u), __builtin_bit_cast(float, 0x3f199999u), 0.0f};
}
