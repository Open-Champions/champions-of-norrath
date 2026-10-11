/* Stream tables. streamBufferSize and streamVolume at 0x0042de38 and
 * 0x0042de48 have st_size 12. norrath_sound_restore scans three size words.
 * stream at 0x007dd290 is .bss; three records of 452 bytes are 1356. The
 * next symbol, gCd, begins 4 bytes after that. Those 4 bytes are not
 * included. Size words are 0x0004b000, 0x00001000, and 0x0001e000. Volume
 * words are the bits of 1.0f.
 */
#define NORRATH_WORD(value) \
    static_cast<unsigned char>(static_cast<unsigned>(value)), \
    static_cast<unsigned char>(static_cast<unsigned>(value) >> 8), \
    static_cast<unsigned char>(static_cast<unsigned>(value) >> 16), \
    static_cast<unsigned char>(static_cast<unsigned>(value) >> 24)

extern "C" {
alignas(8) unsigned char norrath_sound_stream_buffer_sizes_view[12] = {
    NORRATH_WORD(0x0004b000u),
    NORRATH_WORD(0x00001000u),
    NORRATH_WORD(0x0001e000u)
};
alignas(8) unsigned char norrath_sound_stream_volumes_view[12] = {
    NORRATH_WORD(0x3f800000u),
    NORRATH_WORD(0x3f800000u),
    NORRATH_WORD(0x3f800000u)
};
alignas(16) unsigned char norrath_sound_stream_records_view[1356];
}

#undef NORRATH_WORD
