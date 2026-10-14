/* Observed memory-card library storage used by sceMcGetInfo. All four
 * original symbols are local .bss with st_size 0, so the sizes are the spans
 * to the next symbol where that span is a word, and are otherwise noted.
 *
 *   typeAddr     0x008e6b68, next symbol freeAddr 4 bytes later.
 *   freeAddr     0x008e6b6c, next symbol formAddr 4 bytes later.
 *   formAddr     0x008e6b70, next symbol buffFileInfo 16 bytes later. Only
 *                its first word is used by sceMcGetInfo; the 16-byte span is
 *                not claimed as one object.
 *   sifParamNext 0x008e7040, next symbol currentDir 192 bytes later; the
 *                same 192 bytes sceMcGetInfo writes back from the data cache.
 *
 * The views are named for the observed use only. The image bytes are zero.
 */
extern "C" {
alignas(4) int norrath_libmc_info_type_pointer_word_view[1];
alignas(4) int norrath_libmc_info_free_pointer_word_view[1];
alignas(16) int norrath_libmc_info_format_pointer_word_view[1];
alignas(64) int norrath_libmc_param_next_view[48];
}
