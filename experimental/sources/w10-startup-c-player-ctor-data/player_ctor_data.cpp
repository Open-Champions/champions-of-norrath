/* Literals and constants read by the Player constructor (__6PlayerG6Point3sii), defined from the
 * executable image. Addresses are in the private reference executable (SLUS_205.65). Each string is
 * stored NUL-terminated at an 8-byte aligned address; the pad bytes between them are not owned.
 *
 *   0x004644c0  "select.lmp"           lump name used for a single player
 *   0x00464ab8  "select2.lmp"          lump name used for two or more players or forced small characters
 *   0x00464ac8  "%s_hair%da.vif"       prefix, hair number
 *   0x00464ad8  "%s_hair%d%c.tex"      prefix, hair number, letter
 *   0x00464ae8  "%s_beard%da.vif"      prefix, beard number
 *   0x00464af8  "%s_beard%d%c.tex"     prefix, beard number, letter
 *   0x00464b10  "%s_bodyv1.vif"        prefix
 *   0x00464b20  "%s_bodyv%d.tex"       prefix, body variant
 *   0x00464b30  "player unnulled\n"    debug text
 *   0x00464b48  "dhs_idled.anm"        idle animation names selected by a small code:
 *   0x00464b58  "dha_idled.anm"        1, 2, 3, 4, 7 and the default below
 *   0x00464b68  "l_idled.anm"
 *   0x00464b78  "b_idled.anm"
 *   0x00464b88  "dw_idled.anm"
 *   0x00464b98  "ohw_idled.anm"
 *   0x004801f8  six single-precision constants, .rodata, 24 bytes:
 *               0x373a69db, 0x37841882, 0x3fb33333, 0x3fcccccc, 0x3ee66666, 0x3f733333
 * Inferred: the roles above come from how the constructor uses each address, not from running it.
 * Unknown: whether other code shares these literals (the linker may have merged them), and what the
 * float constants stand for beyond their use as stored values.
 */
extern "C" {
alignas(8) extern const char norrath_w9_startup_crt_b_view_4644c0[] = "select.lmp";
alignas(8) extern const char norrath_w9_startup_crt_b_view_464ab8[] = "select2.lmp";
alignas(8) extern const char norrath_w9_startup_crt_b_view_464ac8[] = "%s_hair%da.vif";
alignas(8) extern const char norrath_w9_startup_crt_b_view_464ad8[] = "%s_hair%d%c.tex";
alignas(8) extern const char norrath_w9_startup_crt_b_view_464ae8[] = "%s_beard%da.vif";
alignas(8) extern const char norrath_w9_startup_crt_b_view_464af8[] = "%s_beard%d%c.tex";
alignas(8) extern const char norrath_w9_startup_crt_b_view_464b10[] = "%s_bodyv1.vif";
alignas(8) extern const char norrath_w9_startup_crt_b_view_464b20[] = "%s_bodyv%d.tex";
alignas(8) extern const char norrath_w9_startup_crt_b_view_464b30[] = "player unnulled\n";
alignas(8) extern const char norrath_w9_startup_crt_b_view_464b48[] = "dhs_idled.anm";
alignas(8) extern const char norrath_w9_startup_crt_b_view_464b58[] = "dha_idled.anm";
alignas(8) extern const char norrath_w9_startup_crt_b_view_464b68[] = "l_idled.anm";
alignas(8) extern const char norrath_w9_startup_crt_b_view_464b78[] = "b_idled.anm";
alignas(8) extern const char norrath_w9_startup_crt_b_view_464b88[] = "dw_idled.anm";
alignas(8) extern const char norrath_w9_startup_crt_b_view_464b98[] = "ohw_idled.anm";

alignas(8) extern const float norrath_w9_startup_crt_b_view_4801f8[6] = {
    __builtin_bit_cast(float, 0x373a69dbu), __builtin_bit_cast(float, 0x37841882u),
    __builtin_bit_cast(float, 0x3fb33333u), __builtin_bit_cast(float, 0x3fccccccu),
    __builtin_bit_cast(float, 0x3ee66666u), __builtin_bit_cast(float, 0x3f733333u),
};
}
