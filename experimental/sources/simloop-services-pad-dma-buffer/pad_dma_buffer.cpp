/* pad_dma_buf is .bss at 0x006e1240. The next symbol, stateData, is at
 * 0x006e1c40, so the span is 2560 bytes. padInit gives scePadPortOpen
 * 256-byte buffers: for each of the two ports it starts at a 1280-byte stride
 * and passes slots 0 through 4 at 256-byte steps (0x500 per port). The
 * contents are written by the pad library and are unknown here.
 */
extern "C" {
alignas(64) unsigned char pad_dma_buf[2560];
}
