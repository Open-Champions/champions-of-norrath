// Observed C string "12SpellBlessed" in .rodata at 0x4787b8, passed as the
// derived-class name by the Blessed type-information routine. The view ends at
// the terminating NUL; later bytes are placement gap.
extern "C" {
extern const unsigned char norrath_spell_blessed_type_name_view[15];
const unsigned char norrath_spell_blessed_type_name_view[15] = {
    '1', '2', 'S', 'p', 'e', 'l', 'l', 'B', 'l', 'e', 's', 's', 'e', 'd', 0};
}
