// enableMallocWrapper at 0x00483b11, one file-backed zero byte.
// memFree's wrapper takes the game-heap path only when this byte is nonzero.
// The original name is this same byte, not a second flag.
// The host compiler used for the byte check rejects alias attributes, so that
// check defines NORRATH_WRAPPER_BYTE_HOST_CHECK and the EE object carries the name.
extern "C" unsigned char norrath_enable_malloc_wrapper_byte_view = 0;
#if !defined(NORRATH_WRAPPER_BYTE_HOST_CHECK)
extern "C" unsigned char enableMallocWrapper
    __attribute__((alias("norrath_enable_malloc_wrapper_byte_view")));
#endif
