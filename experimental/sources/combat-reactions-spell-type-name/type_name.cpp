// Observed C string "5Spell" in .sdata at 0x486318, the base name the Blessed
// type-information routine passes. The view ends at the terminating NUL; later
// bytes are placement gap. The image holds several other identical strings
// (for example at 0x485280, used by the Spell type-information routine); this
// view is only the one at 0x486318.
extern "C" {
extern const unsigned char norrath_spell_type_name_view[7];
const unsigned char norrath_spell_type_name_view[7] = {'5', 'S', 'p', 'e', 'l', 'l', 0};
}
