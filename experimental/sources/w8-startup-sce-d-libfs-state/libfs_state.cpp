/* Observed storage of the file-system RPC layer used by sceOpen, sceRead and
 * sceClose. The names are the original local symbol names, published global
 * so those units can resolve them. Where the original is .bss with st_size 0,
 * the size is the span to the next symbol in that section.
 *
 *   _send_data     0x008e04c0, .bss. The next symbol, _rcv_data_rpc, is 3136
 *                  bytes later. sceOpen uses its first 1048 bytes.
 *   _rcv_data_rpc  0x008e1100, .bss, 64 bytes to _rcv_data_cmd.
 *   _iob           0x008e19c0, .bss, 512 bytes to _cd: thirty-two 16-byte
 *                  slots (the stride is the 4-bit shift used for indices).
 *   _cd            0x008e1bc0, .bss, 40 bytes to _fsversion: an RPC client
 *                  record (the word at offset 36 is the bound-server word in
 *                  the sibling libraries).
 *   _fs_fsq_semid  0x00444a04, .data word, image value -1.
 *   _fs_iob_semid  0x00444a00, .data word, image value -1.
 *   _sceFs_q       0x00444970, .data, 128 bytes (global in the original),
 *                  thirty-two words, image value -1 in all of them.
 *
 * _fs_init (0x004449f4) is defined by the iop-module-boot unit, not here.
 * What the slots, queue and records mean to the IOP is not established.
 */
extern "C" {
alignas(64) unsigned char _send_data[3136];
alignas(64) unsigned char _rcv_data_rpc[64];
alignas(64) unsigned char _iob[512];
alignas(8) unsigned char _cd[40];
int _fs_fsq_semid = -1;
int _fs_iob_semid = -1;
int _sceFs_q[32] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
}
