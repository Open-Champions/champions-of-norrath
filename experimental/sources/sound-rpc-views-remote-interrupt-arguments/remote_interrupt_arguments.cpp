/* Observed three .data words: _sce_sdr_transIntr0Arg at 0x00446044,
 * _sce_sdr_transIntr1Arg at 0x00446048, and _sce_sdr_spu2IntrArg at
 * 0x0044604c. Each stored word is zero. sceSdRemote indexes them at
 * 0, 4, and 8.
 */
extern "C" {
alignas(4) unsigned char norrath_sound_remote_interrupt_arguments_view[12] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
}
