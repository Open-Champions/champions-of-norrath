/* Observed three .data words: _sce_sdr_transIntr0Hdr at 0x00446038,
 * _sce_sdr_transIntr1Hdr at 0x0044603c, and _sce_sdr_spu2IntrHdr at
 * 0x00446040. Each stored word is zero. sceSdRemote indexes them at
 * 0, 4, and 8, so this view is those 12 bytes.
 */
extern "C" {
alignas(4) unsigned char norrath_sound_remote_interrupt_functions_view[12] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
}
