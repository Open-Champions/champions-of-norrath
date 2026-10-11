/* Observed topArg at 0x008de0c8. st_size is 0. The next .bss symbol is
 * q.12 at 0x008de500, 1080 bytes later. iWakeupThread uses this address
 * as the ring: the word at offset 4 is the cursor, masked with 511, and
 * a byte is stored at offset 8 of ring+(index*2). The highest byte that
 * formula reaches is offset 1030. Bytes 1031 through 1079 are the rest
 * of the span. The image bytes are zeros.
 */
extern "C" {
alignas(8) unsigned char norrath_sound_wake_ring_view[1080];
}
