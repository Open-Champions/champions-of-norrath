/* Observed seqIdSpinner at 0x00485940. st_size is 2. The .sdata bytes
 * are zero. soundFrameNm begins at 0x00485944, so the two bytes between
 * this halfword and that word are not included.
 */
extern "C" {
alignas(2) unsigned char norrath_sound_sequence_id_spinner_view[2] = {0, 0};
}
