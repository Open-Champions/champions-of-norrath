// Observed _sceCd_cd_read_intr at 0x002e2638, 184 bytes.
// The argument is read through the uncached alias. A positive word at +0 copies
// that many bytes from uncached+16 to both the destination at +8 and its
// uncached alias. A positive word at +4 copies from uncached+80 to the
// destination at +12 and its uncached alias. The count is reread each byte.
// It then tails into _sceCd_cd_callback with sceCdCbfunc_num.
extern "C" {
extern int sceCdCbfunc_num;
void _sceCd_cd_callback(void* argument);
#if defined(NORRATH_HOST_CONTRACT)
unsigned norrath_sony_cd_load_word(unsigned address);
unsigned char norrath_sony_cd_load_byte(unsigned address);
void norrath_sony_cd_store_byte(unsigned address, unsigned char value);
static unsigned load_word(unsigned address) { return norrath_sony_cd_load_word(address); }
static unsigned char load_byte(unsigned address) { return norrath_sony_cd_load_byte(address); }
static void store_byte(unsigned address, unsigned char value) { norrath_sony_cd_store_byte(address, value); }
#else
static unsigned load_word(unsigned address) {
    return *reinterpret_cast<volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(address));
}
static unsigned char load_byte(unsigned address) {
    return *reinterpret_cast<volatile unsigned char*>(static_cast<__UINTPTR_TYPE__>(address));
}
static void store_byte(unsigned address, unsigned char value) {
    *reinterpret_cast<volatile unsigned char*>(static_cast<__UINTPTR_TYPE__>(address)) = value;
}
#endif
static void copy_region(unsigned header, unsigned count_offset, unsigned dest_offset, unsigned source_offset) {
    if (static_cast<int>(load_word(header + count_offset)) <= 0) return;
    const unsigned destination = load_word(header + dest_offset);
    const unsigned destination_uncached = destination | 0x20000000u;
    const unsigned source = header + source_offset;
    int index = 0;
    while (index < static_cast<int>(load_word(header + count_offset))) {
        const unsigned char byte = load_byte(source + static_cast<unsigned>(index));
        store_byte(destination_uncached + static_cast<unsigned>(index), byte);
        store_byte(destination + static_cast<unsigned>(index), byte);
        index += 1;
    }
}
void _sceCd_cd_read_intr(void* argument) {
    const unsigned header = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(argument)) | 0x20000000u;
    copy_region(header, 0u, 8u, 16u);
    copy_region(header, 4u, 12u, 80u);
    _sceCd_cd_callback(&sceCdCbfunc_num);
}
}
