/* Observed storage used by sceFsInit, beyond the objects the other file-system units hold
 * (_send_data, _rcv_data_rpc, _iob, _cd, _fs_init, _fsversion, _fs_iob_semid).
 *
 *   rcv_adr.39       0x008e0480, local .bss, symbol size 0. The next symbol, ip0.64, is 8
 *                    bytes later: two words, the receive-buffer addresses sent to the server.
 *                    The original name has a dot, so the published name is the view below.
 *   _rcv_data_cmd    0x008e1140, .bss, symbol size 0. The next symbol, _iob, is 0x880 bytes
 *                    later. sceFsInit sends the address of this object and the address 0x440
 *                    bytes after it, which divides the span into two halves.
 *   _sif_FsRcv_Data  0x008e1c00, .bss, 64 bytes to _sif_FsPoff_Data. Passed as the argument of
 *                    the handler for command 0x80000011.
 *   _sif_FsPoff_Data 0x008e1c40, .bss, 64 bytes to sceCbPoff_Gp. Its first two words are cleared
 *                    by sceFsInit and it is passed as the argument of command 0x80000013.
 *   _fs_rcv_bufdbl   0x004449f8, .data word, image value 0. Set by sceFsInit from the second
 *                    word the server returns.
 * The image bytes of the .bss objects are zero. What the contents mean is not established here.
 */
extern "C" {
alignas(8) unsigned norrath_fs_rcv_adr_view[2];
alignas(64) unsigned char _rcv_data_cmd[0x880];
alignas(64) unsigned char _sif_FsRcv_Data[64];
alignas(64) unsigned char _sif_FsPoff_Data[64];
int _fs_rcv_bufdbl = 0;
}
