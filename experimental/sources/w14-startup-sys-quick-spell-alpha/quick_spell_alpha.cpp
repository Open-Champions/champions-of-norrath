/* Observed data object, pinned SLUS_205.65: the file-local array that the image names a.1122, at
 * 0x007d0568 in zero-initialised storage (.bss). The symbol table gives it no size; the next label
 * is at 0x007d0578, so its extent is 16 bytes, and the quick-spell menu routine indexes it by player
 * number (four players), reading and writing one 32-bit fade alpha per player, limited to 0..128.
 * It is declared here as four ints under the image's own label through an asm label, because the
 * compiler's name for a function-local static differs from the image's.
 * Unknown: whether the original declared more than four elements (the extent above is only the
 * distance to the next label), and the array's declared type.
 */
extern "C" {
int norrath_quick_spell_alpha[4] __asm__("a.1122");
}
static_assert(sizeof norrath_quick_spell_alpha == 16u, "extent from the label spacing");
