/* Observed sequence at 0x007d8110. The span to globalDialogDir is 2432
 * bytes, which is 32 records of 76. soundPlaySequence at 0x0024d308 and the
 * static initializer both walk 32 records. Loader zeros. The original
 * address is 16-byte aligned.
 */
extern "C" {
alignas(16) unsigned char norrath_sound_sequence_records_view[2432];
}
