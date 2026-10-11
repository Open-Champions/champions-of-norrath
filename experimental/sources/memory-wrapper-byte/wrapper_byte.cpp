// enableMallocWrapper at 0x00483b11, one file-backed zero byte.
// memFree's wrapper takes the game-heap path only when this byte is nonzero.
extern "C" unsigned char norrath_enable_malloc_wrapper_byte_view = 0;
