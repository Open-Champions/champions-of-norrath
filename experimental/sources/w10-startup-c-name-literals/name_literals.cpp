/* Three single-use literals imported by constructors, defined from the executable image.
 *
 *   0x00462260  "game_swish_chest%d.anm"      format taking one int; used by the PhProp constructor with sprintf
 *   0x004708d0  "spel_ew_shockfire.anm"       second string given to lumpFindResource by the Sparks constructor
 *   0x011c3ae8  "VIStation::VIStation\n"      text the VIStation constructor prints at its end
 * Observed: the bytes and the callers' use. Unknown: whether the linker merged any of them with
 * other copies of the same text; only these addresses are defined here.
 */
extern "C" {
alignas(8) extern const char norrath_w9_startup_crt_b_view_462260[] = "game_swish_chest%d.anm";
alignas(8) extern const char norrath_w9_startup_crt_b_view_4708d0[] = "spel_ew_shockfire.anm";
alignas(8) extern const char norrath_w9_startup_crt_b_view_11c3ae8[] = "VIStation::VIStation\n";
}
