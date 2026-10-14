/* Observed soundRestore line 1230 loads the condition at 0x00485970.
 * The .sdata bytes are the C string "buffer". The extra zero at
 * 0x00485977 is the gap before the format slots at 0x00485978 and is
 * not part of this string.
 */
extern "C" {
alignas(16) unsigned char norrath_sound_restore_allocation_condition_view[] = {'b', 'u', 'f', 'f', 'e', 'r', 0};
}
