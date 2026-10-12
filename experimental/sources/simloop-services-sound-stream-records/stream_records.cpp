/* stream is .bss at 0x007dd290: three 452-byte records, 1356 bytes. The next
 * symbol, gCd at 0x007dd7e0, is 4 bytes after the end of the third record.
 * The audio package defines norrath_sound_stream_records_view at this same
 * address as a separate object; the integrator must alias or merge the two.
 */
extern "C" {
alignas(16) unsigned char stream[1356];
}
