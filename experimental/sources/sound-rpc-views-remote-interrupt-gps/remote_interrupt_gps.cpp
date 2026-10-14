/* Observed three .data words: _sce_sdr_transIntr0Gp at 0x00446050,
 * _sce_sdr_transIntr1Gp at 0x00446054, and _sce_sdr_spu2IntrGp at
 * 0x00446058. Each stored word is zero. sceSdRemote indexes them at
 * 0, 4, and 8. The following word, _sce_sdr_cb_thid, is not included.
 */
extern "C" {
alignas(4) unsigned char norrath_sound_remote_interrupt_gps_view[12] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
}
