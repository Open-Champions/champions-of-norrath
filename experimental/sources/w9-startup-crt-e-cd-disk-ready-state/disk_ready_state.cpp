/* Observed storage used by sceCdDiskReady.
 *   _sceCd_rcmdrdata 0x00447b00, symbol size 0x100, initialised data that is all zero in
 *                    the file. The RPC receive area of the call.
 *   cddr             0x008e8390, .bss, symbol size 0. The next symbol, _dr_mode, is 0x40
 *                    bytes later; the SIF RPC client that sceSifBindRpc fills for server
 *                    0x8000059c. Its inner layout is not claimed here.
 *   _dr_mode         0x008e83d0, .bss, symbol size 0. The next symbol, dum_mode, is 8
 *                    bytes later; a single word, the mode argument sent to the server.
 */
extern "C" {
alignas(4) unsigned char _sceCd_rcmdrdata[0x100] = {};
alignas(16) int cddr[16];
alignas(8) int _dr_mode;
}
